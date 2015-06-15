#pragma once

#include "vector3.h"

class Light
{
public:

    Light(Vector3f position) : position(position) {
        ambient = Vector3f(0.5f,0.5f,0.5f);
        diffuse = Vector3f(0.5f,0.5f,0.5f);
        specular = Vector3f(0.8f,0.8f,0.8f);
    }

#pragma mark - Properties
    Vector3f position;

    Vector3f ambient;
    Vector3f diffuse;
    Vector3f specular;

    bool selected;
#pragma mark - Methods
    void draw(void);
    void move(Vector3f);
};
