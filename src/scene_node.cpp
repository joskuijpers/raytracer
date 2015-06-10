#include "scene_node.h"
#include "platform.h"

/**
 * Basic code used for the drawing of every object:
 * applying the translation, rotation and scaling.
 *
 * TODO: use a matrix instead.
 */
void scene_node::draw() {
    glTranslatef(this->translation[0], this->translation[1], this->translation[2]);
    glRotatef(this->rotationAngle, this->rotation[0], this->rotation[1], this->rotation[2]);
    glScalef(this->scale[0], this->scale[1], this->scale[2]);
}

void scene_node::drawBoundingBox() {
    boundingBox.draw();
}