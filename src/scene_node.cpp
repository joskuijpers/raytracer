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

        // If a shadow ray, stop on first hit
        if(ray.isShadowRay())
            break;
    }

    return result;
}


inline ApplyResult applyDirect(shared_ptr<Scene> scene, hit_result hit, unsigned int level, shared_ptr<Light> light);
inline Vector3f applyReflection(shared_ptr<Scene> scene, hit_result hit, unsigned int level, bool testray);
inline Vector3f applyRefraction(shared_ptr<Scene> scene, hit_result hit, unsigned int level);
inline float fresnel(Vector3f direction, Vector3f normal, float n);

ApplyResult SceneNode::apply(shared_ptr<Scene> scene, unsigned int level, hit_result hit, bool testray) {
    ApplyResult result;

    // Sum the ambiant
    Vector3f Ia = Vector3f(0,0,0);
    for(auto& light : scene->lights) {
        Ia += light->ambient;
    }

    result.ambiantColor = Ia * hit.material.getKa();

    // Calculate contribution of every light
    for(auto& light : scene->lights) {
        auto direct_result = applyDirect(scene, hit, level, light);
        result.diffuseColor += direct_result.diffuseColor;
        result.specularColor += direct_result.specularColor;
    }

    // Add relection only if in the illumination model
    if(hit.material.getIl() >= 3 && level < MAX_TRACE_LEVELS) {
        result.reflectedColor = applyReflection(scene, hit, level, testray);
    }

    // Add refraction only if in the illumination model
    if(hit.material.getIl() >= 6 && level < MAX_TRACE_LEVELS) {
        result.refractedColor = applyRefraction(scene, hit, level);
    }

    if (testray) {
        g_raytracer->testrays.push_back(TestRay(Ray(hit.viewer, hit.hitPosition), hit.depth, result));
    }
    return result;
}

/**
 * Calculate the contribution of direct light using Phong shading
 */
inline ApplyResult applyDirect(shared_ptr<Scene> scene, hit_result hit, unsigned int level [[gnu::unused]], shared_ptr<Light> light) {
    hit_result shadowRes;
    ApplyResult result;

    Vector3f Lm, V;

    // Get the direction to the light source
    Lm = light->position - hit.hitPosition;
    float distance = Lm.normalize();

    // See if this point is in shadow. If it is, do not apply diffuse and specular.

    // If Object is convex, no need to do self shadowing
    Vector3f shadowRayOrigin = hit.hitPosition;
    if(!hit.node->isConvex())
        shadowRayOrigin -= 0.001f * hit.lightDirection;

    Ray shadowRay(shadowRayOrigin, light->position);
    shadowRay.shadow_ray = true; // for faster tracing

    // Offset shadow ray to prevent hit the same hitpoint again
    shadowRes = scene->hit(shadowRay, hit.node->isConvex() ? hit.node : nullptr);

    // If hit by shadow, do not draw anything other than ambient
    if(shadowRes.is_hit() && shadowRes.depth >= 0.1f)
       return result;

    // No shading
    if(hit.material.getIl() == 0) {
        result.diffuseColor = hit.material.getKd();
        return result;
    }

    // Diffuse shading
    result.diffuseColor += hit.material.getKd() * hit.normal.dot(Lm) * light->diffuse;
    if(result.diffuseColor[0] < 0.0 || result.diffuseColor[1] < 0.0 || result.diffuseColor[2] < 0.0)
        result.diffuseColor = Vector3f();

    // Specular only if specular component and if illum model required specular
    if(hit.material.hasKs() && hit.material.getIl() >= 2) {
        // Direction towards the viewer
        V = hit.viewer - hit.hitPosition;
        V.normalize();

        Vector3f half = (Lm + V);
        half.normalize();
        float phongTerm = half.dot(hit.normal);

        if(phongTerm < 0.f)
            phongTerm = 0.f;

        result.specularColor += hit.material.getKs() * powf(phongTerm, hit.material.getNs()) * light->specular;
    }

    // Calculate the fresnel paramater
    result.fresnel = fresnel(hit.lightDirection, hit.normal, hit.material.getNi());

    return result;
}

inline Vector3f applyReflection(shared_ptr<Scene> scene, hit_result hit, unsigned int level, bool testray) {
    Ray reflectionRay;
    hit_result reflResult;

    reflectionRay.origin = hit.hitPosition;

    float reflet = 2.f * (hit.lightDirection.dot(hit.normal));
    reflectionRay.updateDirection(hit.lightDirection - reflet * hit.normal);

    // Hit the scene with our ray
    reflResult = scene->hit(reflectionRay, hit.node);

    // If we hit something, add color
    if(reflResult.is_hit() && reflResult.depth >= 0.f)
        return reflResult.node->apply(scene, level + 1, reflResult, testray).sum() * hit.material.getKs();

    return Vector3f(0,0,0);
}

inline Vector3f applyRefraction(shared_ptr<Scene> scene [[gnu::unused]], hit_result hit, unsigned int level [[gnu::unused]]) {
    Vector3f It;
    return (1.f - hit.material.getKs()) * hit.material.getTf() * It;
}

inline float clamp(float min, float max, float value) {
    return std::max(min, std::min(max, value));
}

// Kr = return value, Kt = 1 - Kr
inline float fresnel(Vector3f direction, Vector3f normal, float n) {
    float cosi = clamp(-1.f, 1.f, normal.dot(direction));
    float etai = 1, etat = n;

    if(cosi > 0)
        swap(etai,etat);

    float sint = etai / etat * sqrtf(max(0.f, 1 - cosi * cosi));
    if(sint >= 1)
        return 1;
    else {
        float cost = sqrtf(max(0.f, 1.f - sint * sint));
        cosi = fabsf(cosi);

        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        return (Rs * Rs + Rp * Rp) / 2.0;
    }
}
