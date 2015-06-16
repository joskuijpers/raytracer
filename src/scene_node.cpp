#include "scene_node.h"
#include "platform.h"
#include "matrix4.h"
#include "raytracing.h"

/**
 * Basic code used for the drawing of every object:
 * applying the translation, rotation and scaling.
 *
 * TODO: use a matrix instead.
 */
void SceneNode::draw() {
    glMultMatrixf(ws_transformationMatrix.get());
}

void SceneNode::drawNormals() {
    for (auto& obj : children) {
        obj->drawNormals();
    }
}

void SceneNode::drawBoundingBox() {
    for(auto &obj : children) {
        obj->drawBoundingBox();
    }

    ws_boundingBox.draw();
}

void SceneNode::createBoundingBox() {
    AABoundingBox box;

    // Create bounding boxes for all children
    for(auto& obj : children) {
        AABoundingBox child;

        obj->createBoundingBox();

        box.extend(obj->boundingBox);
    }
}

void SceneNode::updateTransformationMatrix(void) {
    Matrix4f trans, rot, scal;

    trans.makeTranslation(translation);
    rot.makeRotation(rotationAngle, rotation);
    scal.makeScale(scale);

    // TODO: make sure this is the right order!
    // Rotate around certain point: Scale * point translation * rotation * object translation
    // Normally: scale * rotation * translation
    transformationMatrix = scal * rot * trans;

    // If parent is available, using the parent worldspace matrix, create our own ws matrix
    if(auto p = parent.lock()) {
        ws_transformationMatrix = p->ws_transformationMatrix * transformationMatrix;
    } else
        ws_transformationMatrix = transformationMatrix;

    // If we have children, update them too
    for (auto& obj : children) {
        obj->updateTransformationMatrix();
    }
}

void SceneNode::createWsBoundingBox(void) {
    ws_boundingBox.min = ws_transformationMatrix * boundingBox.min;
    ws_boundingBox.max = ws_transformationMatrix * boundingBox.max;
    ws_boundingBox.color = boundingBox.color;
}

#pragma mark - Raytracing

hit_result SceneNode::hit(Ray ray, shared_ptr<SceneNode> skip) {
    hit_result result;

    // check against bounding box
    if(skip == nullptr && !ws_boundingBox.intersection(ray, FLT_MAX)) {
        return result;
    }

    // Either 0, and this method is called by child class
    // or >0 children and this is a collection.
    for(auto& node : children) {
        hit_result nodeResult;

        if(skip == node)
            continue;

        // Try to hit the node
        nodeResult = node->hit(ray, skip);

        if(!nodeResult.is_hit())
            continue;

        // If nearer, store result
        if(nodeResult.depth < result.depth) {
            result = nodeResult;
        }
    }
    
    return result;
}

Vector3f SceneNode::apply(unsigned int level [[gnu::unused]], hit_result hit_info)
{
    Vector3f color, ws_hitPosition;
    Material mat = hit_info.material;

    ws_hitPosition = hit_info.node->ws_transformationMatrix * hit_info.hitPosition;

    // Make this average of all light sources the ambient light
    Vector3f Ia = Vector3f(0,0,0);
    for(auto& light : g_raytracer->scene->lights) {
        Ia += light->ambient;
    }
    Ia /= g_raytracer->scene->lights.size();

    // Calculate the color using Phong shading
    color = Ia * mat.getKa();

    // Calculate contribution of every light
    /// @note https://en.wikipedia.org/wiki/Phong_reflection_model#Description
    for(auto& light : g_raytracer->scene->lights) {
        hit_result shadowRes;
        Vector3f Lm, Rm, V;

        // Get the direction to the light source
        Lm = light->position - ws_hitPosition;
        Lm.normalize();

        // Direction of a perfectly reflected ray
        Rm = 2.f * (Lm.dot(hit_info.normal) * hit_info.normal) - Lm;

        // Direction towards the viewer
        V = hit_info.viewer - ws_hitPosition;
        V.normalize();

        // See if this point is in shadow. If it is, do not apply diffuse and specular.
        Ray shadowRay(ws_hitPosition, light->position);

        // Offset shadow ray to prevent hit the same hitpoint again
        shadowRay.origin += 0.00001f * shadowRay.direction;
        shadowRes = g_raytracer->scene->hit(shadowRay);

        if(!shadowRes.is_hit() || shadowRes.depth < 0.f) {
            // Diffuse
            color += mat.getKd() * hit_info.normal.dot(Lm) * light->diffuse;

            // Specular
            Vector3f spec = mat.getKs() * powf(Rm.dot(V), mat.getNs()) * light->specular;
            if(spec.length() > 0.f) // It can only contribute
                color += spec;
        }
    }

    return color;
}
