#pragma once

#include <vector>
#include <memory>

#include "mesh.h"
#include "color.h"
#include "scene.h"
#include "ray.h"
#include "testray.h"

/// Transform a mouse position to a source/destination pair
extern void produceRay(int x_I, int y_I, Vector3f &origin, Vector3f &dest);


class Raytracer;
extern shared_ptr<Raytracer> g_raytracer;

class Raytracer : public enable_shared_from_this<Raytracer>
{
public:
    Raytracer() : windowSizeX(800), windowSizeY(600) {
        scene = move(unique_ptr<Scene>(new Scene()));
    };

#pragma mark - Events

public:
    void init(void);

    /**
     * OpenGL draw for debugging and quick visualizing the render.
     */
    void draw(void);

    void drawDebugRay();
    /**
     * Additional keyboard functionality.
     */
    void keyboard(char t, int x, int y, const Vector3f& rayOrigin, const Vector3f& rayDest);

#pragma mark - Raytracing

    /**
     * Perform a raytrace for given ray.
     *
     * @return Color of the pixel by the ray.
     */
    Vector3f performRayTracing(const Vector3f &origin, const Vector3f &dest, bool testray = false);

#pragma mark - Properties

    /// Raytracer render size and resolution
    unsigned int windowSizeX, windowSizeY;
    unsigned int horizontalResolution, verticalResolution;

    /// The raytracer scene
    shared_ptr<Scene> scene;

    /// Test ray
    vector<TestRay> testrays;
};
