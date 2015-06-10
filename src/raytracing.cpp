#include "raytracing.h"

#include <cstdio>
#include <cfloat>
#include <cassert>
#include <memory>

#include "platform.h"
#include "compiler_opt.h"

#include "mesh.h"
#include "sphere.h"

using std::endl;
using std::cout;

//temporary variables
//these are only used to illustrate
//a simple debug drawing. A ray
Vector3f testRayOrigin;
Vector3f testRayDestination;

/**
 * Initialize the scene.
 */
void init(void)
{
    g_scene->background_color = Vector3f(.6f,.2f,.1f);


    unique_ptr<mesh> cube(new mesh("cube"));
    unique_ptr<mesh> cube2(new mesh("cube2"));
    unique_ptr<Sphere> sphere(new Sphere("sphere"));

    cube->loadMesh("resource/cube.obj", true);
    cube->computeVertexNormals();
    cube->parent = g_scene;

    cube2->loadMesh("resource/cube.obj", true);
    cube2->computeVertexNormals();
    cube2->translation = Vector3f(-2,0,0);
    cube2->scale = Vector3f(.8f,.8f,.8f);
    cube2->parent = g_scene;

    sphere->translation = Vector3f(0, .5f, -1.f);
    sphere->radius = .5f;
    sphere->parent = g_scene;

    Material mat;
    mat.setKd(.9f, .9f, .9f);
    mat.setKa(.1f, .1f, .1f);

    sphere->material = mat;

    g_scene->children.push_back(move(cube));
    g_scene->children.push_back(move(cube2));
    g_scene->children.push_back(move(sphere));


//    unique_ptr<mesh> car(new mesh("car"));
//    car->loadMesh("resource/dodgeColorTest.obj", true);
//    car->computeVertexNormals();
//    g_scene->nodes.push_back(move(car));

//    unique_ptr<mesh> strawberry(new mesh("strawberry"));
//    strawberry->loadMesh("resource/strawberry.obj", true);
//    g_scene->nodes.push_back(move(strawberry));

    // Create a single lighblendert
    g_scene->lights.push_back(unique_ptr<Light>(new Light(g_scene->camera)));

    // Prepare the scene for raytracing: create bounding boxes,
    // and possibly transformation matrices
    g_scene->prepare();
}

/**
 * @return return the color of the pixel
 */
Vector3f performRayTracing(const Vector3f &origin, const Vector3f &dest)
{
    Ray ray(origin, dest);
    Vector3f col;

    // Hit the scene with the first ray
    hit_result result = g_scene->hit(ray);

    if(!result.is_hit()) {
        // Check screen shadows. Darken the background if current pixel
        // is not lit by light.

        // Intersect with light(s)
        auto& light = g_scene->lights[0];

        hit_result shadowRes = g_scene->hit(Ray(origin, light->position), nullptr);

        if(shadowRes.is_hit())
            return .5f * g_scene->background_color;
        else
            return g_scene->background_color;
    }

    // If hit, apply the ray:
        // shading
        // reflection
        // reflection

    // Apply the hit, this is recursive.
    assert(result.node != nullptr && "A positive hit result must have node info");

    col = result.node->apply(1, result);

    return col;
}

void yourDebugDraw(void)
{
    //draw open gl debug stuff
    //this function is called every frame

    // Draw the scene
    g_scene->draw();

    //as an example: we draw the test ray, which is set by the keyboard function
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);
        glColor3f(0,1,1);
        glVertex3f(testRayOrigin[0], testRayOrigin[1], testRayOrigin[2]);

        glColor3f(0,0,1);
        glVertex3f(testRayDestination[0], testRayDestination[1], testRayDestination[2]);
    glEnd();

    glPointSize(10);

    glBegin(GL_POINTS);
        glVertex3fv(g_scene->lights[0]->position.pointer());
    glEnd();

    glPopAttrib();
}

void yourKeyboardFunc(char t, int mouseX, int mouseY, const Vector3f &rayOrigin, const Vector3f &rayDestination)
{
    //here, as an example, I use the ray to fill in the values for my upper global ray variable
    //I use these variables in the debugDraw function to draw the corresponding ray.
    //try it: Press a key, move the camera, see the ray that was launched as a line.
    testRayOrigin = rayOrigin;
    testRayDestination = rayDestination;

    // do here, whatever you want with the keyboard input t.

    //...


    std::cout << t << " pressed! The mouse was in location " << mouseX << ", " << mouseY << "!" << std::endl;
}
