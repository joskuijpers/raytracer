#pragma once

#include "scene_node.h"
#include "material.h"

using namespace std;

class Sphere : public scene_node
{
public:
    Sphere(const char *name) : scene_node(name) {};
    
#pragma mark - Drawing

    void draw();

#pragma mark - Raytracing

    hit_result hit(Ray ray);
    color3 apply(unsigned int level, hit_result hit_info);

#pragma mark - Properties
    float radius;

    Material material;
};