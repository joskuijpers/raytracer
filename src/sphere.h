#pragma once

#include "scene_node.h"
#include "material.h"

using namespace std;

class Sphere : public scene_node
{
public:
    Sphere(const char *name) : scene_node(name) {};
    
#pragma mark - Drawing

    void draw(void);

#pragma mark - Raytracing

    void createBoundingBox();
    hit_result hit(Ray ray, shared_ptr<scene_node> skip = nullptr);
    Vector3f apply(unsigned int level, hit_result hit_info);

#pragma mark - Properties
    float radius;

    Material material;
};