#include "raytracing.h"

#include <cstdio>
#include <cfloat>
#include <cassert>
#include <memory>

#include "platform.h"
#include "compiler_opt.h"
#include <cstring>
#include "mesh.h"
#include "sphere.h"
#include "trackball.h"
#include "texture.h"
#include "skybox.h"

using namespace std;

#define TESTSET 4

void Raytracer::init(void) {
    scene->background_color = Vector3f(.4f,.4f,.8f);

#if TESTSET == 1
    unique_ptr<Mesh> cube(new Mesh("cube"));
    unique_ptr<Mesh> cube2(new Mesh("cube2"));
    unique_ptr<Sphere> sphere(new Sphere("sphere"));
    unique_ptr<Sphere> sphere2(new Sphere("sphere2"));

    cube->loadMesh("resource/cube.obj", true);
    cube->computeVertexNormals();
    cube->parent = scene;

    cube2->loadMesh("resource/cube.obj", true);
    cube2->computeVertexNormals();
    cube2->translation = Vector3f(-2,0,0);
    cube2->scale = Vector3f(.8f,.8f,.8f);
    cube2->parent = scene;
    cube2->rotation = Vector3f(0,1,0);

    sphere->translation = Vector3f(0, .5f, -1.f);
    sphere->radius = .5f;
    sphere->parent = scene;

    sphere2->translation = Vector3f(-1.f, .2f, 2.f);
    sphere2->radius = .2f;
    sphere2->parent = scene;

    Material mat;
    mat.setKd(.9f, .3f, .9f);
    mat.setKa(.2f, .02f, .2f);
    mat.setKs(1.f, 1.f, 1.f);
    mat.setNs(27);
    mat.setIl(4);

    sphere->material = mat;
    sphere2->material = mat;

    scene->children.push_back(move(cube));
    scene->children.push_back(move(cube2));
    scene->children.push_back(move(sphere));
    scene->children.push_back(move(sphere2));
#elif TESTSET == 2
    unique_ptr<Mesh> car(new Mesh("car"));
    car->loadMesh("resource/dodgeColorTest.obj", true);
    car->computeVertexNormals();
    car->parent = scene;
    scene->children.push_back(move(car));
#elif TESTSET == 3
    unique_ptr<Mesh> strawberry(new Mesh("strawberry"));
    strawberry->loadMesh("resource/strawberry.obj", true);
    strawberry->parent = scene;
    scene->children.push_back(move(strawberry));
#elif TESTSET == 4
    unique_ptr<Mesh> teapot(new Mesh("teapot"));
    teapot->loadMesh("resource/teapot2.obj", true);
    teapot->computeVertexNormals();
    teapot->parent = scene;
    //teapot->rotation = Vector3f(1.f, 2.f, 3.f);
    //teapot->rotationAngle = 10.f;
    scene->children.push_back(move(teapot));

    unique_ptr<Mesh> teapot2(new Mesh("teapot"));
    teapot2->loadMesh("resource/teapot2.obj", true);
    teapot2->computeVertexNormals();
    teapot2->parent = scene;
    teapot2->scale = Vector3f(1,1,1) * .4f;
    teapot2->translation = Vector3f(-1.3f, 0, 0);
    scene->children.push_back(move(teapot2));

    unique_ptr<Sphere> sphere(new Sphere("sphere"));
    sphere->parent = scene;
    sphere->translation = Vector3f(-.4f,0.2f,1.f);
    sphere->radius = 0.2f;

    Material mat;
    mat.setKd(.9f, .3f, .9f);
    mat.setKa(.2f, .02f, .2f);
    mat.setKs(0.5f, 0.5f, 0.5f);
    mat.setNs(27);
    mat.setIl(6);
    mat.setNi(1.5);

    sphere->material = mat;

    scene->children.push_back(move(sphere));


    shared_ptr<Mesh> floor(new Mesh("floor"));
    floor->loadMesh("resource/cube.obj", true);
    floor->computeVertexNormals();
    floor->parent = scene;
    floor->translation = Vector3f(-4.f,-.1f,-4.f);
    floor->scale = Vector3f(8,.1f,8);
//    scene->children.push_back(move(floor));
#elif TESTSET == 5
    unique_ptr<Mesh> teapot(new Mesh("teapot"));
    teapot->loadMesh("resource/teapot.obj", true);
    teapot->computeVertexNormals();
    teapot->parent = scene;
    scene->children.push_back(move(teapot));
#elif TESTSET == 6
    unique_ptr<Mesh> cube(new Mesh("cube"));
    cube->loadMesh("resource/cube.obj", true);
    cube->computeVertexNormals();
    cube->parent = scene;
    scene->children.push_back(move(cube));
#endif

    scene->lights.push_back(unique_ptr<Light>(new Light(Vector3f(4,5,0))));
    scene->lights[0]->selected = true;
    scene->lights.push_back(unique_ptr<Light>(new Light(scene->camera + Vector3f(0,2,0))));

    // Prepare the scene for raytracing: create bounding boxes,
    // and possibly transformation matrices
    scene->prepare();

    texture = new Texture("resource/Skybox.png", 1024+1024+512, 1024+512, 1024);
    skybox = new Skybox(texture);
}

#pragma mark - Events

void Raytracer::drawDebugRay() {
    for(auto ray = testrays.begin(); ray != testrays.end(); ++ray){
        ray->draw();
    }

    if(!testrays.empty()) {
        TestRay lastray = testrays.back();
        lastray.drawInfo();
    }
}

void Raytracer::draw(void) {
    // Draw the scene
    scene->draw();

    // Draw the debug ray
    drawDebugRay();

    glPopAttrib();
}

void Raytracer::keyboard(char t, int x, int y) {
    switch (t) {
        case 'n':
            scene->showNormals = !scene->showNormals;
            break;
        case 'b':
            scene->showBoundingBoxes = !scene->showBoundingBoxes;
            break;
        case 's':
            scene->showTree = !scene->showTree;
            break;
        case 'c':
            testrays.clear();
            break;
        default:
            std::cout << t << " pressed! The mouse was in location " << x << ", " << y << "!" << std::endl;
            break;
    }
}


#pragma mark - Raytracing

Vector3f Raytracer::performRayTracing(const Vector3f &origin, const Vector3f &dest, bool testray) {
    Ray ray(origin, dest);
    shared_ptr<Scene> scene = g_raytracer->scene;


    // Hit the scene with the first ray
    hit_result result = scene->hit(ray);

    if (!result.is_hit()) {
        // Check screen shadows. Darken the background if current pixel
        // is not lit by light.

        // Intersect with light(s)
//        auto& light = scene->lights[0];
//
//        hit_result shadowRes = scene->hit(Ray(origin, light->position));
//
//
//        if(shadowRes.is_hit())
//            return .5f * scene->background_color;
//        else
            //return scene->background_color;
            return skybox->getColor(ray);
        //return Vector3f(0.1f, 0.1f, 0.1f);
    }

    // Apply the hit, this is recursive.
    ApplyResult col = result.node->apply(scene, 1, result, testray);

    return col.sum();
}

