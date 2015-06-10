#include "scene_node.h"
#include "platform.h"

/**
 * Basic code used for the drawing of every object:
 * applying the translation, rotation and scaling.
 *
 * TODO: use a matrix instead.
 */
void SceneNode::draw() {
    glTranslatef(this->translation[0], this->translation[1], this->translation[2]);
    glRotatef(this->rotationAngle, this->rotation[0], this->rotation[1], this->rotation[2]);
    glScalef(this->scale[0], this->scale[1], this->scale[2]);
}

void SceneNode::drawBoundingBox() {
    boundingBox.draw();
}

void SceneNode::createTransformationMatrix(void) {
    
}

void SceneNode::createWsBoundingBox(void) {

}

void SceneNode::createWsTransformationMatrix(void) {

}