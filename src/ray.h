#pragma once

#include "Vector3.h"

class Ray {
public:
#pragma mark - Constructor
    Ray(Vector3f origin, Vector3f dest) : origin(origin), dest(dest) {
        direction = dest - origin;

        // make unit vector
        direction /= direction.length();
    }

    inline Ray() {}

public:

#pragma mark - Methods

    /**
     * Move ray from world space to object space.
     *
     * A ray is in world space by default. Applying transformations to
     * objects is same as inverse applying the transformations to the ray.
     * P_ws = M * Pos, P_os = M^-1 * P_ws
     */
    Ray transform(Vector3f translation, Vector3f scale [[gnu::unused]], Vector3f rotation [[gnu::unused]], float rotationAngle [[gnu::unused]]) const {
        Ray r;

        r.origin = origin - translation;
        r.direction = direction;

        // origin_os = m^-1 origin_ws
        // dir_os = m^-1 direction_ws

        // Step1: build 4x4 transformation matrix
        // Step2: invert the matrix
        // Step3: make two homogenous vectors, origin and direction
        // Step4: multiply by the inv matrix
        // Step5: Make Vector3 and store

        return r;
    }

#pragma mark - Properties

    Vector3f origin, dest;
    Vector3f direction; // should be unit vector
};
