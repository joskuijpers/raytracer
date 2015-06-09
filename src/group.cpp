#include "group.h"
#include "platform.h"

void group::draw() {
    scene_node::draw();

    // Draw all sub-objects
    for(auto &obj : children) {
        glPushMatrix();

        obj->draw();

        glPopMatrix();
    }
}

hit_result group::hit(Ray ray) {
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

        // Transform ray by applying translation of object
        transfRay = ray.transform(node->translation, node->scale, node->rotation, node->rotationAngle);

        // Try to hit the node
        nodeResult = node->hit(transfRay);

        if(!nodeResult.is_hit())
            continue;

        // If nearer, store result
        if(nodeResult.depth < result.depth) {
            result = nodeResult;
        }
    }
    
    return result;
}
