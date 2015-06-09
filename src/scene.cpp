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
        obj->draw();
    }
}

void scene::prepare() {

}

hit_result scene::hit(ray ray) {
    hit_result result;

    // check against bounding box
    // if(!hit_aabb)
    //  result.int_res = DISJOINT;
    //  return;

    for(auto& node : this->nodes) {
        hit_result nodeResult;

        // Transform ray by applying translation of object
        ray.origin += node->translation;
        // TODO: rotation, scale

        // Try to hit the node
        nodeResult = node->hit(ray);

        if(!nodeResult.is_hit())
            continue;

        // If nearer, store result
        if(nodeResult.depth < result.depth) {
            result = nodeResult;
        }
    }

    return result;
}