#include "scene_node.h"
#include "platform.h"
#include "matrix4.h"
#include "raytracing.h"

#define MAX_TRACE_LEVELS 3

/**
 * Basic code used for the drawing of every object:
 * applying the translation, rotation and scaling.
 *
 * TODO: use a matrix instead.
 */
void SceneNode::draw() {
    glMultMatrixf(ws_transformationMatrix.get());
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
    Vector3f directColor, reflectedColor, refractedColor;
    Material mat = hit_info.material;

#pragma mark Direct light

    // Make this average of all light sources the ambient light
    Vector3f Ia = Vector3f(0,0,0);
    for(auto& light : g_raytracer->scene->lights) {
        Ia += light->ambient;
    }
    Ia /= g_raytracer->scene->lights.size();

    // Calculate the color using Phong shading
    directColor = Ia * mat.getKa();

    // Calculate contribution of every light
    /// @note https://en.wikipedia.org/wiki/Phong_reflection_model#Description
    for(auto& light : g_raytracer->scene->lights) {
        hit_result shadowRes;

        // See if this point is in shadow. If it is, do not apply diffuse and specular.
        Ray shadowRay(hit_info.hitPosition, light->position);

        // Offset shadow ray to prevent hit the same hitpoint again
        shadowRes = g_raytracer->scene->hit(shadowRay, hit_info.node);

        if(!shadowRes.is_hit() || shadowRes.depth < 0.f) {
            Vector3f lightDir;

            // Get the direction to the light source
            lightDir = light->position - hit_info.hitPosition;
            lightDir.normalize();

            // Diffuse shading
            if(mat.getIl() == 0) // no shading
                directColor += mat.getKd();
            else
                directColor += mat.getKd() * hit_info.normal.dot(lightDir) * light->diffuse;

            // Specular only if specular component and if illum model required specular
            if(mat.hasKs() && mat.getIl() >= 2) {
                Vector3f phongDir, viewerDir;
                float phongTerm;

                // Direction of a perfectly reflected ray
                phongDir = lightDir - 2.f * (lightDir.dot(hit_info.normal) * hit_info.normal);

                // Direction towards the viewer
                viewerDir = hit_info.viewer - hit_info.hitPosition;
                viewerDir.normalize();

                phongTerm = phongDir.dot(viewerDir);
                if(phongTerm < 0.f)
                    phongTerm = 0.f;

                directColor += mat.getKs() * powf(phongTerm, mat.getNs()) * light->specular;
            }
        }
    }

#pragma mark Reflection

    // Add relection only if in the illumination model
    if(mat.getIl() >= 3 && level < MAX_TRACE_LEVELS) {
        Ray reflectionRay;
        hit_result reflResult;

        reflectionRay.origin = hit_info.hitPosition;

        float reflet = 2.f * (hit_info.lightDirection.dot(hit_info.normal));
        reflectionRay.updateDirection(hit_info.lightDirection - reflet * hit_info.normal);

        // Hit the scene with our ray
        reflResult = g_raytracer->scene->hit(reflectionRay, hit_info.node);

        // If we hit something, add color
        if(reflResult.is_hit() && reflResult.depth >= 0.f) {
            // Get the hit color
            reflectedColor = reflResult.node->apply(level + 1, reflResult);

            reflectedColor *= mat.getKs();
        }
    }

#pragma mark Refraction

    // Add relection only if in the illumination model
    if(mat.getIl() >= 6 && level < MAX_TRACE_LEVELS) {
        Vector3f It;
        refractedColor = (1.f - mat.getKs()) * mat.getTf() * It;
    }

    return directColor + reflectedColor + refractedColor;
}
