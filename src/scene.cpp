#include "scene.h"

#include "platform.h"
#include "mesh.h"

/// Draw the lights in OpenGL
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

/// Draw the scene in OpenGL
void scene::draw(void) {
    drawLights();

    if(showBoundingBoxes)
        boundingBox.draw();

    // Draw all sub-objects
    for(auto &obj : nodes) {
        glPushMatrix();

        obj->draw();

        glPopMatrix();
    }

    if(showBoundingBoxes) {
        for(auto& obj : nodes) {
            glPushMatrix();

            obj->scene_node::draw(); // transform
            obj->drawBoundingBox();
            
            glPopMatrix();
        }
    }
}

/// Prepare the scene, recursively
void scene::prepare() {
    boundingBox.color = vector3f(.851f,.604f,.302f);

    // Create children bounding boxes, and expand our own to contain them
    for(auto& obj : nodes) {
        obj->createBoundingBox();

        boundingBox.extend(obj->boundingBox);

        cout << "found BB for node " << obj->name << ": " << obj->boundingBox.min <<"," << obj->boundingBox.max << endl;
        cout << boundingBox << endl;
    }
}

/// Discover a hit
hit_result scene::hit(Ray ray, shared_ptr<scene_node> skip) {
    hit_result result;

    // check against bounding box
    // if(!hit_aabb)
    //  result.int_res = DISJOINT;
    //  return;

    for(auto& node : this->nodes) {
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