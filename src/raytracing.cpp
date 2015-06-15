#include "raytracing.h"

#include <cstdio>
#include <cfloat>
#include <cassert>
#include <memory>

#include "platform.h"
#include "compiler_opt.h"

#include "mesh.h"
#include "sphere.h"

using namespace std;

void Raytracer::init(void) {
    scene->background_color = Vector3f(.6f,.2f,.1f);

//
//    unique_ptr<mesh> cube(new mesh("cube"));
//    unique_ptr<mesh> cube2(new mesh("cube2"));
//    unique_ptr<Sphere> sphere(new Sphere("sphere"));
//    unique_ptr<Sphere> sphere2(new Sphere("sphere2"));
//
//    cube->loadMesh("resource/cube.obj", true);
//    cube->computeVertexNormals();
//    cube->parent = scene;
//
//    cube2->loadMesh("resource/cube.obj", true);
//    cube2->computeVertexNormals();
//    cube2->translation = Vector3f(-2,0,0);
//    cube2->scale = Vector3f(.8f,.8f,.8f);
//    cube2->parent = scene;
//    cube2->rotation = Vector3f(0,1,0);
//
//    sphere->translation = Vector3f(0, .5f, -1.f);
//    sphere->radius = .5f;
//    sphere->parent = scene;
//
//    sphere2->translation = Vector3f(-1.f, .2f, 2.f);
//    sphere2->radius = .2f;
//    sphere2->parent = scene;
//
//    Material mat;
//    mat.setKd(.9f, .9f, .9f);
//    mat.setKa(.2f, .2f, .2f);
//
//    sphere->material = mat;
//    sphere2->material = mat;
//
//    scene->children.push_back(move(cube));
//    scene->children.push_back(move(cube2));
//    scene->children.push_back(move(sphere));
//    scene->children.push_back(move(sphere2));


    //    unique_ptr<mesh> car(new mesh("car"));
    //    car->loadMesh("resource/dodgeColorTest.obj", true);
    //    car->computeVertexNormals();
    //    car->parent = g_scene;
    //    g_scene->children.push_back(move(car));

    //    unique_ptr<mesh> strawberry(new mesh("strawberry"));
    //    strawberry->loadMesh("resource/strawberry.obj", true);
    //    g_scene->nodes.push_back(move(strawberry));



    unique_ptr<mesh> teapot(new mesh("teapot"));
    teapot->loadMesh("resource/teapot.obj", true);
    teapot->computeVertexNormals();
    teapot->parent = scene;
    scene->children.push_back(move(teapot));

    // Create a single lighblendert
    scene->lights.push_back(unique_ptr<Light>(new Light(scene->camera)));

    // Prepare the scene for raytracing: create bounding boxes,
    // and possibly transformation matrices
    scene->prepare();
}

#pragma mark - Events

void Raytracer::draw(void) {
    //draw open gl debug stuff
    //this function is called every frame

    // Draw the scene
    scene->draw();

    //as an example: we draw the test ray, which is set by the keyboard function
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);
    glColor3f(0,1,1);
    glVertex3f(testRay.origin[0], testRay.origin[1], testRay.origin[2]);

    glColor3f(0,0,1);
    glVertex3f(testRay.dest[0], testRay.dest[1], testRay.dest[2]);
    glEnd();

    glPointSize(10);

    glBegin(GL_POINTS);
    glVertex3fv(scene->lights[0]->position.pointer());
    glEnd();

    glPopAttrib();
}

void Raytracer::keyboard(char t [[gnu::unused]], int mouseX [[gnu::unused]], int mouseY [[gnu::unused]], const Vector3f& rayOrigin, const Vector3f& rayDest) {
    testRay.update(rayOrigin, rayDest);

//    std::cout << t << " pressed! The mouse was in location " << mouseX << ", " << mouseY << "!" << std::endl;
}

#pragma mark - Raytracing

Vector3f Raytracer::performRayTracing(const Vector3f &origin, const Vector3f &dest) {
    Ray ray(origin, dest);
    Vector3f col;

    // Hit the scene with the first ray
    hit_result result = g_raytracer->scene->hit(ray);

    if(!result.is_hit()) {
        // Check screen shadows. Darken the background if current pixel
        // is not lit by light.

        // Intersect with light(s)
        auto& light = g_raytracer->scene->lights[0];

        hit_result shadowRes = g_raytracer->scene->hit(Ray(origin, light->position), nullptr);

        if(shadowRes.is_hit())
            return .5f * g_raytracer->scene->background_color;
        else
            return g_raytracer->scene->background_color;
    }

    // Apply the hit, this is recursive.
    col = result.node->apply(1, result);
    
    return col;
}

