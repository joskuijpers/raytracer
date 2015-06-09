#include "scene.h"

#include "platform.h"
#include "mesh.h"

void scene::drawLights(void)
{
    // Draw the lights as white dits
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    // Disable lighting
    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);
    glPointSize(10);
    glBegin(GL_POINTS);

    for (auto &light : lights) {
        glVertex3fv(light->position.pointer());
    }

    glEnd();
    glPopAttrib();
}

void scene::draw(void) {
    drawLights();

    // Draw all sub-objects
    for(auto &obj : nodes) {
        glPushMatrix();

        obj->draw();

        glPopMatrix();
    }
}

void scene::prepare() {

}

hit_result scene::hit(Ray ray) {
    hit_result result;

    // check against bounding box
    // if(!hit_aabb)
    //  result.int_res = DISJOINT;
    //  return;

    for(auto& node : this->nodes) {
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