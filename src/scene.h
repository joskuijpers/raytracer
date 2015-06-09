#pragma once

#include <vector>
#include <memory>

#include "vector3.h"
#include "scene_node.h"
#include "color.h"

using namespace std;

class aabb
{
    // boolean hit(ray, out float distance, out triangle t, )
};

class light
{
public:

    light(vector3f position) : position(position) {
    }

#pragma mark - Properties
    vector3f position;

    // size
    // etc
};

class scene
{
public:
#pragma mark - Drawing

    void draw(void);

#pragma mark - Raytracing

    void prepare();
    hit_result hit(Ray ray);

#pragma mark - Properties

    /// Camera location
    vector3f camera;

    color3 background_color;

    /// Lights within the scene
    // TODO: special light object in the graph
    vector<unique_ptr<light>> lights;

    /// Nodes within the scene
    vector<shared_ptr<scene_node>> nodes;

    /// Whether to draw the bounding boxes of objects and scene.
    bool showBoundingBoxes = true;

private:
    void drawLights(void);
};
