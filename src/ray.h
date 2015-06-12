#pragma once

#include "vector3.h"
#include "matrix4.h"

class Ray {
public:
#pragma mark - Constructor
    Ray(Vector3f origin, Vector3f dest) : origin(origin), dest(dest) {
        direction = dest - origin;

        // make unit vector
        direction.normalize();

        direction_inv = Vector3f(1.f / direction[0], 1.f / direction[1], 1.f / direction[2]);

        sign[0] = direction_inv[0] < 0;
        sign[1] = direction_inv[1] < 0;
        sign[2] = direction_inv[2] < 0;
    }

    inline Ray() {}

#pragma mark - Methods

    /**
     * Move ray from world space to object space.
     *
     * A ray is in world space by default. Applying transformations to
     * objects is same as inverse applying the transformations to the ray.
     * P_ws = M * Pos, P_os = M^-1 * P_ws
     */
    Ray transform(Matrix4f transformationMatrix) const {
        Ray r;

        // origin_os = m^-1 origin_ws
        // dir_os = m^-1 direction_ws

        // Step1: build 4x4 transformation matrix (param)
        // Step2: invert the matrix
        transformationMatrix.invert(); // TODO: cache

        // Step3: make two homogenous vectors, origin and direction (auto)
        // Step4: multiply by the inv matrix
        // Step5: Make Vector3 and store (auto)
        r.origin = transformationMatrix * origin;
        r.direction = direction;
        r.dest = dest;

        return r;
    }

#pragma mark - Properties

    Vector3f origin, dest;
    Vector3f direction, direction_inv;
    int sign[3];
};
