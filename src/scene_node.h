#pragma once

#include <vector>
#include <set>
#include <string>
#include <cfloat>

#include "vector3.h"
#include "ray.h"
#include "color.h"
#include "aabb.h"

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

    Vector3f hitPosition, normal;
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
    scene_node(const char *name) : name(name), translation(Vector3f(0,0,0)), scale(Vector3f(1,1,1)), rotation(Vector3f(1,1,1)), rotationAngle(0.f) {}

    // abstract calculateBoundingBox()

#pragma mark - Drawing

    /// The draw method for OpenGL display. Override it.
    virtual void draw(void);
    virtual void drawBoundingBox(void);

#pragma mark - Raytracing

    /// Create the bounding box
    virtual void createBoundingBox() = 0;

    /// The hit method, to detect ray hits.
    virtual hit_result hit(Ray ray, shared_ptr<scene_node> skip = nullptr) = 0;

    // Apply method: applies the hit.
    virtual Vector3f apply(unsigned int level, hit_result hit_info) = 0;

#pragma mark - Properties
    const char *name;

    vector<shared_ptr<scene_node>> children;
//    weak_ptr<scene_node> parent;

    Vector3f translation;
    Vector3f scale;
    Vector3f rotation;
    float rotationAngle;

    AABoundingBox boundingBox;
};
