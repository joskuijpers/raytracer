#pragma once

#include <memory>

#include "texture.h"
#include "vector3.h"
#include "ray.h"

class Skybox {
    Texture *texture;


    Vector3f getColorFromFace(Texture::Face face, float x, float y);
    public:
    Skybox(Texture *texture) : texture(texture) {}

    Vector3f getColor(Ray &ray);

};
