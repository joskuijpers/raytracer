#pragma once

#include <vector>
#include <set>
#include <string>
#include <cfloat>

#include "vector3.h"
#include "matrix4.h"
#include "ray.h"
#include "color.h"
#include "aabb.h"

using namespace std;

class SceneNode;

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
    shared_ptr<SceneNode> node;

    /// size_t sized information for apply method.
    size_t sInfo;

    /// Position of the hit. OBJECT SPACE? WORLD SPACE?
    Vector3f hitPosition;

    /// Normal at that location.
    Vector3f normal;
};

/**
 * A node in the scene.
 *
 * @note For grouping objects together, use the group class.
 * @warning Can't be used from stack!
 */
class SceneNode : public enable_shared_from_this<SceneNode>
{
public:
    SceneNode(const char *name) : name(name), translation(Vector3f(0,0,0)), scale(Vector3f(1,1,1)), rotation(Vector3f(1,1,1)), rotationAngle(0.f) {}

    // abstract calculateBoundingBox()

#pragma mark - Drawing

    /// The draw method for OpenGL display. Override it.
    virtual void draw(void);
    virtual void drawBoundingBox(void);

#pragma mark - Raytracing

    /// Create the bounding box
    virtual void createBoundingBox(void);

    /// Create world space BB
    void createWsBoundingBox(void);

    // Update OS transform matrix, and the WS matrix (using parent)
    void updateTransformationMatrix(void);

    /// The hit method, to detect ray hits.
    virtual hit_result hit(Ray ray, shared_ptr<SceneNode> skip = nullptr);

    // Apply method: applies the hit.
    virtual Vector3f apply(unsigned int level, hit_result hit_info) = 0;

#pragma mark - Properties
    const char *name;

    vector<shared_ptr<SceneNode>> children;
    weak_ptr<SceneNode> parent;

    AABoundingBox boundingBox;

    /// Same as above bounding box, but in world space.
    /// Min and Max are multiplied by the ws_transformationMatrix.
    AABoundingBox ws_boundingBox;

#pragma mark - Transformation properties

    Vector3f translation;
    Vector3f scale;
    Vector3f rotation;
    float rotationAngle;

    /// Transformation matrix. Must be updated if you change the above values.
    Matrix4f transformationMatrix;

    /// Transformation matrix, in world space.
    Matrix4f ws_transformationMatrix;
};
