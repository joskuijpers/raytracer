#include "scene.h"

#include "platform.h"
#include "mesh.h"
#include "raytracing.h"

/// Draw the lights in OpenGL
void Scene::drawLights(void)
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
void Scene::draw(void) {

    vector<shared_ptr<Light>>& lights = g_raytracer->scene->lights;
    for(unsigned int i = 0; i < 8; ++i){
        if((size_t)i < lights.size()) {
            glEnable(GL_LIGHT0+i);
            glLightfv(GL_LIGHT0 + i, GL_POSITION, lights[i]->position.pointer());
            glLightfv(GL_LIGHT0 + i, GL_AMBIENT, lights[i]->ambient.pointer());
            glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, lights[i]->diffuse.pointer());
            glLightfv(GL_LIGHT0 + i, GL_SPECULAR, lights[i]->specular.pointer());
        } else {
            glDisable(GL_LIGHT0 + i);
        }
    }

    drawLights();

    if(showBoundingBoxes)
        ws_boundingBox.draw();

    // Draw all sub-objects
    for(auto &obj : children) {
        glPushMatrix();

        obj->draw();

        glPopMatrix();
    }

    if(showBoundingBoxes) {
        for(auto& obj : children) {
            obj->drawStructure();
        }
    }

    if (showNormals) {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glDisable(GL_LIGHTING);

        for (auto& obj : children)
            obj->drawNormals();

        glPopAttrib();
    }
}

/// Prepare the scene, recursively
void Scene::prepare() {
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

    createWsBoundingBox();
}
