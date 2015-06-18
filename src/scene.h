#pragma once

#include <vector>
#include <memory>

#include "vector3.h"
#include "scene_node.h"

class Light
{
public:

    Light(Vector3f position) : position(position) {
        ambient = Vector3f(1,1,1);
        diffuse = Vector3f(.5f,.5f,.5f);
        specular = Vector3f(1,1,1);
    }

#pragma mark - Properties
    Vector3f position;

    Vector3f ambient;
    Vector3f diffuse;
    Vector3f specular;
};

class Scene : public SceneNode
{
public:
    Scene() : SceneNode("scene") {};
#pragma mark - Drawing

    void draw(void);

#pragma mark - Raytracing

    void prepare();

    // Unused
    void createBoundingBox(void) {};
    Vector3f apply(unsigned int level [[gnu::unused]], hit_result hit_info [[gnu::unused]]) {
        return Vector3f(1,1,1);
    };

#pragma mark - Properties

    /// Camera location
    Vector3f camera;

    Vector3f background_color;

    /// Lights within the scene
    // TODO: special light object in the graph
    vector<shared_ptr<Light>> lights;

    /// Whether to draw the bounding boxes of objects and scene.
    bool showBoundingBoxes = true;
    bool showNormals = false;

private:
    void drawLights(void);
};
