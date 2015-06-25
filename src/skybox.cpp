
#include "skybox.h"


float max(Vector3f x){
    float max = 0;
    if(fabsf(x[0]) > max) max = fabsf(x[0]);

    if(fabs(x[1]) > max) max = fabsf(x[1]);
    if(fabsf(x[2]) > max) max = fabsf(x[2]);

    return max;
}

Vector3f Skybox::getColorFromFace(Texture::Face face , float x, float y){
    return texture->getFromFace(face, x, y);
}

Vector3f Skybox::getColor(Ray &ray) {
    Vector3f map = ray.direction / max(ray.direction);

    if(map[2] > 0.999f)
        return getColorFromFace(Texture::Face::FACE_FRONT, map[0], -map[1]);
    else if(map[2] < -0.999f)
        return getColorFromFace(Texture::Face::FACE_BACK, -map[0], -map[1]);
    else if(map[1] > 0.999f)
        return getColorFromFace(Texture::Face::FACE_TOP, map[0], map[2]);
    else if(map[1] < -0.999f)
        return getColorFromFace(Texture::Face::FACE_BOTTOM, map[0], -map[2]);
    else if(map[0] > 0.999f)
        return getColorFromFace(Texture::Face::FACE_RIGHT, -map[2], -map[1]);
    else if(map[0] < -0.999f)
        return getColorFromFace(Texture::Face::FACE_LEFT, map[2], -map[1]);

    return Vector3f(0.1,0.1,0.1);
}

