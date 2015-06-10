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
    for(auto &obj : children) {
        glPushMatrix();

        obj->draw();

        glPopMatrix();
    }

    if(showBoundingBoxes) {
        for(auto& obj : children) {
            obj->drawBoundingBox();
        }
    }
}

/// Prepare the scene, recursively
void scene::prepare() {
    boundingBox.color = Vector3f(.851f,.604f,.302f);

    // Create transformation matrices
    // Create WS transformation matrix (top down)
    // Create bounding boxes (bottom up)
        // + Create WS bounding boxes

    // Create our own transformation matrix. Also updates WS matrix, and all our children.
    updateTransformationMatrix();

    // Create children bounding boxes, and expand our own to contain them
    for(auto& obj : children) {
        obj->createBoundingBox();

        // At this point, the ws matrix is filled
        obj->createWsBoundingBox();

        // Extend our BB to cover this node
        boundingBox.extend(obj->ws_boundingBox);
    }
}
