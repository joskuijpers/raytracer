#pragma once

#include "scene_node.h"

/**
 * A scene group, to group objects together.
 *
 * Do not use the base class scene_node for this task.
 */
class Group : public scene_node
{
public:
#pragma mark - Drawing

    void draw(void);
    void drawBoundingBox(void);

#pragma mark - Raytracing

    void createBoundingBox();

    /// The hit method, to detect ray hits.
    hit_result hit(Ray ray, shared_ptr<scene_node> skip = nullptr);
};