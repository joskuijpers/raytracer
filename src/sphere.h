#pragma once

#include "scene_node.h"
#include "material.h"

using namespace std;

class Sphere : public SceneNode
{
public:
    Sphere(const char *name) : SceneNode(name) {};
    
#pragma mark - Drawing

    void draw(void);

#pragma mark - Raytracing

    void createBoundingBox();
    hit_result hit(Ray ray, shared_ptr<SceneNode> skip = nullptr, size_t triangleSkip = SIZE_T_MAX);

#pragma mark - Properties
    float radius;

    Material material;

    bool isConvex() {
        return true;
    }

private:
    Vector3f scale [[deprecated]];
    Vector3f rotation [[deprecated]];
    float rotationAngle [[deprecated]];
};