#pragma once

#include "scene_node.h"

/**
 * A scene group, to group objects together.
 *
 * Do not use the base class scene_node for this task.
 */
class group : public scene_node
{
public:
#pragma mark - Drawing

    /// The draw method for OpenGL display. Override it.
    void draw(void);

#pragma mark - Raytracing

    /// The hit method, to detect ray hits.
    hit_result hit(Ray ray, shared_ptr<scene_node> skip = nullptr);

    // aabb
};