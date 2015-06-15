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
    hit_result hit(Ray ray, shared_ptr<SceneNode> skip = nullptr);

#pragma mark - Properties
    float radius;

    Material material;

private:
    Vector3f scale [[deprecated]];
    Vector3f rotation [[deprecated]];
    float rotationAngle [[deprecated]];
};