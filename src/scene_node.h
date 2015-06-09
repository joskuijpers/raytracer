#pragma once

#include <vector>
#include <string>
#include <cfloat>

#include "vector3.h"
#include "ray.h"
#include "color.h"
//#include "aabb.h"

using namespace std;

class scene_node;

/**
 * Hit result information.
 *
 * @todo Move this to the raytracing file if possible?
 */
class hit_result {
public:
#pragma mark - Constructor
    hit_result() : depth(FLT_MAX), hit(false) {}

#pragma mark - Operators

#pragma mark - Methods

    bool is_hit(void) const {
        return hit;
    }

#pragma mark - Properties

    /// Depth of the hit
    float depth;

    /// Intersection result
    bool hit;

    /// The hit node
    shared_ptr<scene_node> node;

    /// size_t sized information for apply method.
    size_t sInfo;
};

/**
 * A node in the scene.
 *
 * @note For grouping objects together, use the group class.
 * @warning Can't be used from stack!
 */
class scene_node : public enable_shared_from_this<scene_node>
{
public:
    scene_node(const char *name) : name(name), translation(vector3f(0,0,0)), scale(vector3f(1,1,1)), rotation(vector3f(1,1,1)), rotationAngle(0.f) {}

    // abstract calculateBoundingBox()

#pragma mark - Drawing

    /// The draw method for OpenGL display. Override it.
    virtual void draw(void);

#pragma mark - Raytracing

    /// The hit method, to detect ray hits.
    virtual hit_result hit(ray ray) = 0;

    // Apply method: applies the hit.
    virtual color3 apply(unsigned int level, hit_result hit_info) = 0;

#pragma mark - Properties
    const char *name;

    vector<shared_ptr<scene_node>> children;
    weak_ptr<scene_node> parent;

    vector3f translation;
    vector3f scale;
    vector3f rotation;
    float rotationAngle;

    // aabb boundingBox;
};
