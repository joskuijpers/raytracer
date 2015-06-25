#pragma once

#include <vector>
#include <memory>

#include "vector3.h"
#include "scene_node.h"
#include "light.h"

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
    ApplyResult apply(unsigned int level [[gnu::unused]], hit_result hit_info [[gnu::unused]]) {
        return ApplyResult();
    };

#pragma mark - Properties

    /// Camera location
    Vector3f camera;

    Vector3f background_color;

    /// Lights within the scene
    // TODO: special light object in the graph
    vector<shared_ptr<Light>> lights;
    int selectedLightIndex = 0;

    /// Whether to draw the bounding boxes of objects and scene.
    bool showBoundingBoxes = true;
    bool showNormals = false;
    bool showTree = false;

#pragma mark - Methods
    void selectNextLight();
    Light* getSelectedLight();

private:
    void drawLights(void);

};
