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

void Group::drawBoundingBox() {

    for(auto &obj : children) {
        obj->drawBoundingBox();
    }

    boundingBox.draw();
}

void Group::createBoundingBox() {
    AABoundingBox box;

    // Create bounding boxes for all children
    for(auto& obj : children) {
        AABoundingBox child;

        obj->createBoundingBox();
        child = obj->boundingBox;

        box.extendMin(child.min);
        box.extendMax(child.max);
    }
}

hit_result Group::hit(Ray ray, shared_ptr<SceneNode> skip) {
    hit_result result;

    // check against bounding box
    // if(!hit_aabb)
    //  result.int_res = DISJOINT;
    //  return;

    // Either 0, and this method is called by child class
    // or >0 children and this is a collection.
    for(auto& node : this->children) {
        Ray transfRay;
        hit_result nodeResult;

        if(skip == node)
            continue;

        // Transform ray by applying translation of object
        transfRay = ray.transform(node->translation, node->scale, node->rotation, node->rotationAngle);

        // Try to hit the node
        nodeResult = node->hit(transfRay, skip);

        if(!nodeResult.is_hit())
            continue;

        // If nearer, store result
        if(nodeResult.depth < result.depth) {
            result = nodeResult;
        }
    }
    
    return result;
}
