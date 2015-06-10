#pragma once

#include "scene_node.h"

/**
 * A scene group, to group objects together.
 *
 * Do not use the base class SceneNode for this task.
 */
class Group : public SceneNode
{
public:
#pragma mark - Drawing

    void draw(void);
};