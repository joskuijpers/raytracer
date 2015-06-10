#pragma once

#include <cfloat>

#include "vector3.h"
#include "ray.h"

/**
 * Axis Aligned Bounding Box
 */
class AABoundingBox
{
public:
#pragma mark - Constructors
    AABoundingBox()
    : min(vector3f(FLT_MAX,FLT_MAX,FLT_MAX)),
    max(vector3f(FLT_MIN,FLT_MIN,FLT_MIN)),
    color(vector3f(.302f,.824f,.851f)){}

#pragma mark - Methods

    /// Extend the boundix boxes' minimum vector to contain given vector.
    inline void extendMin(vector3f other) {
        if(other[0] < min[0])
            min[0] = other[0];

        if(other[1] < min[1])
            min[1] = other[1];

        if(other[2] < min[2])
            min[2] = other[2];
    };

    /// Extend the boundix boxes' maximum vector to contain given vector.
    inline void extendMax(vector3f other) {
        if(other[0] > max[0])
            max[0] = other[0];

        if(other[1] > max[1])
            max[1] = other[1];

        if(other[2] > max[2])
            max[2] = other[2];
    };

    /// Extend the boundix boxes' bounds to contain given vector.
    inline void extend(vector3f other) {
        if(other[0] > max[0])
            max[0] = other[0];
        else if(other[0] < min[0])
            min[0] = other[0];

        if(other[1] > max[1])
            max[1] = other[1];
        else if(other[1] < min[1])
            min[1] = other[1];

        if(other[2] > max[2])
            max[2] = other[2];
        else if(other[2] < min[2])
            min[2] = other[2];
    };

    // Extend this bounding box to contain given bounding box.
    inline void extend(AABoundingBox other) {
        extendMin(other.min);
        extendMax(other.max);
    };

#pragma mark - Drawing

    void draw();

#pragma mark - Raytracing

//    void hit(Ray ray);

#pragma mark - Properties

    vector3f min, max;
    vector3f color;
};

std::ostream& operator<< (std::ostream &output, const AABoundingBox &bb);
