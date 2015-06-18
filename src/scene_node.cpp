#include "scene_node.h"
#include "platform.h"
#include "matrix4.h"
#include "raytracing.h"
#include "config.h"

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

inline Vector3f applyDirect(shared_ptr<Scene> scene, hit_result hit, unsigned int level, shared_ptr<Light> light);
inline Vector3f applyReflection(shared_ptr<Scene> scene, hit_result hit, unsigned int level);
inline Vector3f applyRefraction(shared_ptr<Scene> scene, hit_result hit, unsigned int level);

Vector3f SceneNode::apply(shared_ptr<Scene> scene, unsigned int level, hit_result hit) {
    Vector3f directColor, reflectedColor, refractedColor;

    // Make this average of all light sources the ambient light
    Vector3f Ia = Vector3f(0,0,0);
    for(auto& light : scene->lights) {
        Ia += light->ambient;
    }
    Ia /= scene->lights.size();

    directColor = Ia * hit.material.getKa();

    // Calculate contribution of every light
    for(auto& light : scene->lights) {
        directColor += applyDirect(scene, hit, level, light);
    }

    // Add relection only if in the illumination model
    if(hit.material.getIl() >= 3 && level < MAX_TRACE_LEVELS) {
        reflectedColor = applyReflection(scene, hit, level);
    }

    // Add refraction only if in the illumination model
    if(hit.material.getIl() >= 6 && level < MAX_TRACE_LEVELS) {
        refractedColor = applyRefraction(scene, hit, level);
    }

    return directColor + reflectedColor + refractedColor;
}

/**
 * Calculate the contribution of direct light using Phong shading
 */
inline Vector3f applyDirect(shared_ptr<Scene> scene, hit_result hit, unsigned int level [[gnu::unused]], shared_ptr<Light> light) {
    hit_result shadowRes;
    Vector3f lightDir;
    Vector3f color;

    // See if this point is in shadow. If it is, do not apply diffuse and specular.
    Ray shadowRay(hit.hitPosition, light->position);

    // Offset shadow ray to prevent hit the same hitpoint again
    // TODO: if convex, always skip self-shadowing. If not convex, find another way.
    shadowRes = scene->hit(shadowRay, hit.node);

    // If hit by shadow, do not draw anything other than ambient
    if(shadowRes.is_hit() && shadowRes.depth >= 0.f)
        return Vector3f(0,0,0);

    // No shading
    if(hit.material.getIl() == 0)
        return hit.material.getKd();

    // Get the direction to the light source
    lightDir = light->position - hit.hitPosition;
    lightDir.normalize();

    // Diffuse shading
    color += hit.material.getKd() * hit.normal.dot(lightDir) * light->diffuse;

    // Specular only if specular component and if illum model required specular
    if(hit.material.hasKs() && hit.material.getIl() >= 2) {
        Vector3f phongDir, viewerDir;
        float phongTerm;

        // Direction of a perfectly reflected ray
        phongDir = lightDir - 2.f * (lightDir.dot(hit.normal) * hit.normal);

        // Direction towards the viewer
        viewerDir = hit.viewer - hit.hitPosition;
        viewerDir.normalize();

        phongTerm = phongDir.dot(viewerDir);
        if(phongTerm < 0.f)
            phongTerm = 0.f;

        color += hit.material.getKs() * powf(phongTerm, hit.material.getNs()) * light->specular;
    }

    return color;
}

inline Vector3f applyReflection(shared_ptr<Scene> scene, hit_result hit, unsigned int level) {
    Ray reflectionRay;
    hit_result reflResult;

    reflectionRay.origin = hit.hitPosition;

    float reflet = 2.f * (hit.lightDirection.dot(hit.normal));
    reflectionRay.updateDirection(hit.lightDirection - reflet * hit.normal);

    // Hit the scene with our ray
    reflResult = scene->hit(reflectionRay, hit.node);

    // If we hit something, add color
    if(reflResult.is_hit() && reflResult.depth >= 0.f)
        return reflResult.node->apply(scene, level + 1, reflResult) * hit.material.getKs();

    return Vector3f(0,0,0);
}

inline Vector3f applyRefraction(shared_ptr<Scene> scene [[gnu::unused]], hit_result hit, unsigned int level [[gnu::unused]]) {
    Vector3f It;


    return (1.f - hit.material.getKs()) * hit.material.getTf() * It;
}
