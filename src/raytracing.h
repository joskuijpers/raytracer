#pragma once

#include <vector>

#include "mesh.h"
#include "color.h"
#include "scene.h"
#include "ray.h"

extern scene g_scene;

extern unsigned int g_windowSizeX; // window resolution width
extern unsigned int g_windowSizeY; // window resolution height

extern unsigned int RayTracingResolutionX; // larger window
extern unsigned int RayTracingResolutionY; // larger window

// use this function for any preprocessing of the mesh.
void init();

// you can use this function to transform a click to an origin and destination
// the last two values will be changed. There is no need to define this function.
// it is defined elsewhere
void produceRay(int x_I, int y_I, vector3f &origin, vector3f &dest);

// your main function to rewrite
vector3f performRayTracing(const vector3f &origin, const vector3f &dest);

// a function to debug --- you can draw in OpenGL here
void yourDebugDraw();

// want keyboard interaction? Here it is...
void yourKeyboardFunc(char t, int x, int y, const vector3f &rayOrigin, const vector3f &rayDestination);
