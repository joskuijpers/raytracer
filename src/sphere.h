#pragma once

#include "scene_node.h"
#include "material.h"

using namespace std;

class sphere : public scene_node
{
public:

#pragma mark - Drawing

    void draw();

#pragma mark - Raytracing

    hit_result hit(ray ray);

#pragma mark - Properties
    float radius;

    material material;
};