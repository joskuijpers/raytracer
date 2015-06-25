#pragma once

#include "vector3.h"

class Light
{
public:

    Light(Vector3f position) : position(position) {
        ambient = Vector3f(.1f, .1f, .1f);
        diffuse = Vector3f(.8f, .8f, .8f);
        specular = Vector3f(.8f, .8f, .8f);
        intensity = 1.f;
    }

#pragma mark - Properties
    Vector3f position;

    Vector3f ambient;
    Vector3f diffuse;
    Vector3f specular;
    float intensity;

    bool selected;
#pragma mark - Methods
    void draw(void);
    void move(Vector3f);
};
