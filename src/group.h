#pragma once

#include "scene_node.h"

class group : public scene_node
{
public:
#pragma mark - Drawing

    /// The draw method for OpenGL display. Override it.
    void draw(void);

#pragma mark - Raytracing

    /// The hit method, to detect ray hits.
    hit_result hit(ray ray);
};