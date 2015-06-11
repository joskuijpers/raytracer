#include "group.h"
#include "platform.h"

void Group::draw() {
    SceneNode::draw();

    // Draw all sub-objects
    for(auto &obj : children) {
        glPushMatrix();

        obj->draw();

        glPopMatrix();
    }
}
