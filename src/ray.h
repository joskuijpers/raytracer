#pragma once

#include "vector3.h"

class ray {
public:
    ray(vector3f origin, vector3f dest) : origin(origin) {
        direction = dest - origin;

        // make unit vector
        direction /= direction.getLength();
    }

    vector3f origin;
    vector3f direction; // should be unit vector
};
