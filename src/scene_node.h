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
#include "material.h"

class SceneNode;

/**
 * Hit result information.
 *
 * @todo Move this to the raytracing file if possible?
 */
struct hit_result {
public:
#pragma mark - Constructor
    hit_result() : depth(FLT_MAX), hit(false), triangle(SIZE_T_MAX) {}

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

    /// Position of the hit. In World Space
    Vector3f hitPosition;

    /// Normal at that location.
    Vector3f normal;

    /// Material at hit location.
    Material material;

    /// Viewer position (ray origin, ws)
    Vector3f viewer;

    /// Direction the light came from
    Vector3f lightDirection;

    /// Triangle ID for meshes. SIZE_T_MAX is no mesh or no hit.
    size_t triangle;
};

struct ApplyResult{
    Vector3f ambiantColor, diffuseColor, specularColor, reflectedColor, refractedColor;
    float fresnel;

#pragma mark - Constructors
    ApplyResult() { }

    ApplyResult(Vector3f &ambiantColor, Vector3f &diffuseColor, Vector3f &specularColor, Vector3f &reflectedColor,
                Vector3f &refractedColor, float fresnel) : ambiantColor(ambiantColor), diffuseColor(diffuseColor),
                                            specularColor(specularColor), reflectedColor(reflectedColor),
                                            refractedColor(refractedColor), fresnel(fresnel) { }

    void update(Vector3f &ambiantColor, Vector3f &diffuseColor, Vector3f &specularColor, Vector3f &reflectedColor,
                Vector3f &refractedColor, float fresnel){
        this->ambiantColor = ambiantColor;
        this->diffuseColor = diffuseColor;
        this->specularColor = specularColor;
        this->reflectedColor = reflectedColor;
        this->refractedColor = refractedColor;
        this->fresnel = fresnel;
    }

    Vector3f sum() {
//        cout << "fresnel " << fresnel << endl;
        return ambiantColor + diffuseColor + specularColor + reflectedColor + refractedColor;
    }
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
    virtual void drawNormals(void);
    virtual void drawStructure(void);

#pragma mark - Raytracing

    ApplyResult extended_result;
    /// Create the bounding box
    virtual void createBoundingBox(void);

    /// Create world space BB
    void createWsBoundingBox(void);

    // Update OS transform matrix, and the WS matrix (using parent)
    void updateTransformationMatrix(void);

    /// The hit method, to detect ray hits.
    virtual hit_result hit(Ray ray, shared_ptr<SceneNode> skip = nullptr, size_t triangleSkip = SIZE_T_MAX);

    // Apply method: applies the hit.
    virtual ApplyResult apply(shared_ptr<Scene> scene, unsigned int level, hit_result hit_info, bool testray = false);

#pragma mark - Properties

    const char *name;

    vector<shared_ptr<SceneNode>> children;
    weak_ptr<SceneNode> parent;

    AABoundingBox boundingBox;

    /// Same as above bounding box, but in world space.
    /// Min and Max are multiplied by the ws_transformationMatrix.
    AABoundingBox ws_boundingBox;

    virtual bool isConvex() {
        return false;
    }

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
