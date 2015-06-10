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

    Light(vector3f position) : position(position) {
        ambient = vector3f(0.5f,0.5f,0.5f);
        diffuse = vector3f(0.5f,0.5f,0.5f);
    }

#pragma mark - Properties
    vector3f position;

    vector3f ambient;
    vector3f diffuse;
};

class scene
{
public:
#pragma mark - Drawing

    void draw(void);

#pragma mark - Raytracing

    void prepare();
    hit_result hit(Ray ray, shared_ptr<scene_node> skip = nullptr);

#pragma mark - Properties

    /// Camera location
    vector3f camera;

    vector3f background_color;

    /// Lights within the scene
    // TODO: special light object in the graph
    vector<unique_ptr<Light>> lights;

    /// Nodes within the scene
    vector<shared_ptr<scene_node>> nodes;

    /// Scene bounding box
    AABoundingBox boundingBox;

    /// Whether to draw the bounding boxes of objects and scene.
    bool showBoundingBoxes = true;

private:
    void drawLights(void);
};
