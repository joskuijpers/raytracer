#pragma once

#include "vector3.h"
#include "ray.h"
#include "scene_node.h"

class TestRay {
public:
    Ray ray;
    float length;
    ApplyResult applyResult;
    Vector3f color;


TestRay(Ray ray, float length, ApplyResult applyResult) : ray(ray), length(length), applyResult(applyResult) {
    color = applyResult.sum();
}

    void draw();
    void drawInfo();

};
