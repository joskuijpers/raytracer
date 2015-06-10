#pragma once

#include <vector>
#include <memory>

#include "vector3.h"
#include "scene_node.h"

using namespace std;

class aabb
{
    // boolean hit(ray, out float distance, out triangle t, )
};

class Light
{
public:

    Light(Vector3f position) : position(position) {
        ambient = Vector3f(0.5f,0.5f,0.5f);
        diffuse = Vector3f(0.5f,0.5f,0.5f);
    }

#pragma mark - Properties
    Vector3f position;

    Vector3f ambient;
    Vector3f diffuse;
};

class scene
{
public:
#pragma mark - Drawing

    void draw(void);

#pragma mark - Raytracing

    void prepare();
    hit_result hit(Ray ray, shared_ptr<SceneNode> skip = nullptr);

#pragma mark - Properties

    /// Camera location
    Vector3f camera;

    Vector3f background_color;

    /// Lights within the scene
    // TODO: special light object in the graph
    vector<unique_ptr<Light>> lights;

    /// Nodes within the scene
    vector<shared_ptr<SceneNode>> nodes;

    /// Scene bounding box
    AABoundingBox boundingBox;

    /// Whether to draw the bounding boxes of objects and scene.
    bool showBoundingBoxes = true;

private:
    void drawLights(void);
};
