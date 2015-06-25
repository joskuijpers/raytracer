#pragma once

#include <cfloat>

#include "vector3.h"
#include "ray.h"
#include <memory>

using namespace std;

class Scene;
class SceneNode;
class hit_result;

/**
 * Axis Aligned Bounding Box
 */
class AABoundingBox
{
public:
#pragma mark - Constructors
    AABoundingBox()
    : min(Vector3f(FLT_MAX,FLT_MAX,FLT_MAX)),
    max(Vector3f(FLT_MIN,FLT_MIN,FLT_MIN)),
    color(Vector3f(.302f,.824f,.851f)){}

    AABoundingBox(Vector3f min, Vector3f max)
    : min(min), max(max), color(Vector3f(.302f,.824f,.851f)) {
        assert(min <= max);
    }

#pragma mark - Methods

    /// Extend the boundix boxes' minimum vector to contain given vector.
    inline void extendMin(Vector3f other) {
        if(other[0] < min[0])
            min[0] = other[0];

        if(other[1] < min[1])
            min[1] = other[1];

        if(other[2] < min[2])
            min[2] = other[2];
    };

    /// Extend the boundix boxes' maximum vector to contain given vector.
    inline void extendMax(Vector3f other) {
        if(other[0] > max[0])
            max[0] = other[0];

        if(other[1] > max[1])
            max[1] = other[1];

        if(other[2] > max[2])
            max[2] = other[2];
    };

    /// Extend the boundix boxes' bounds to contain given vector.
    inline void extend(Vector3f other) {
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

    int findLongestAxis();

#pragma mark - Drawing

    void draw();

#pragma mark - Raytracing

    // Hit the box with a ray.
    bool intersection(Ray ray, float t);

#pragma mark - Properties

    Vector3f min, max;
    Vector3f color;
};

inline std::ostream& operator<< (std::ostream &output, const AABoundingBox &bb) {
    output << "(" << bb.min << "," << bb.max << ")";

    return output;
}
