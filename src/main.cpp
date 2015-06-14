#include "platform.h"

#include <cstdlib>
#include <cmath>
#include <cassert>

#include "raytracing.h"
#include "mesh.h"
#include "trackball.h"
#include "image_writer.h"
#include "scene.h"

using namespace std;

void display(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);

/// Our raytracer
shared_ptr<Raytracer> g_raytracer(new Raytracer());

/**
 * Animation tick function.
 *
 * animation is called for every image on the screen once
 */
void animate() {
    g_raytracer->scene->camera = getCameraPosition();

    glutPostRedisplay();
}

/**
 * Application entry point.
 */
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    // framebuffer setup
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    // positioning and size of window
    glutInitWindowPosition(200, 100);
    glutInitWindowSize(g_raytracer->windowSizeX, g_raytracer->windowSizeY);
    glutCreateWindow(argv[0]);

    // initialize viewpoint
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,0,-4);

    // Setup trackball
    tbInitTransform();
    tbHelp();

    g_raytracer->scene->camera = getCameraPosition();

    // activate the light following the camera
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    // Set light position
    int lightPosition[4] = {0,0,2,0};
    int lightMaterial[4] __attribute__((unused)) = {1,1,1,1};
    glLightiv(GL_LIGHT0, GL_POSITION, lightPosition);

    // (Missing) normals will be normalized in the graphics pipeline
    glEnable(GL_NORMALIZE);

    // Clear color of the background is black.
    glClearColor (0.0, 0.0, 0.0, 0.0);

    // Activate rendering modes
    // - Depth test
    glEnable( GL_DEPTH_TEST );

    // draw front-facing triangles filled and back-facing triangles as wires
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);

    // Interpolate vertex colors over the triangles
    glShadeModel(GL_SMOOTH);

    // GLUT setup
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutMouseFunc(tbMouseFunc);    // trackball
    glutMotionFunc(tbMotionFunc);  // uses mouse
    glutIdleFunc(animate);

    g_raytracer->init();

    glutMainLoop();

    return 0;
}

/**
 * GLUT display function.
 *
 * Each display tick.
 */
void display(void)
{
    // Store GL state
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Start with fresh matrix
    glLoadIdentity();

    // Apply trackball
    tbVisuTransform();

    g_raytracer->draw();

    glutSwapBuffers();

    // Restore state
    glPopAttrib();
}

/**
 * GLUT event: when window changed size
 */
void reshape(int w, int h)
{
    // Update viewport
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    // Switch to projection mode
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    //glOrtho (-1.1, 1.1, -1.1,1.1, -1000.0, 1000.0);
    gluPerspective(50, (float)w/h, 0.01, 10);

    // Siwtch back to model mode
    glMatrixMode(GL_MODELVIEW);
}

/**
 * Product a ray
 *
 * transform the x, y position on the screen into the corresponding 3D world position
 */
void produceRay(int pX, int pY, Vector3f *origin, Vector3f *dest)
{
    int viewport[4];
    double modelview[16], projection[16];
    double x, y, z;
    int yNew;

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //recuperer matrices
    glGetDoublev(GL_PROJECTION_MATRIX, projection); //recuperer matrices
    glGetIntegerv(GL_VIEWPORT, viewport);//viewport
    yNew = viewport[3] - pY;

    gluUnProject(pX, yNew, 0, modelview, projection, viewport, &x, &y, &z);
    origin->set(x, y, z);

    gluUnProject(pX, yNew, 1, modelview, projection, viewport, &x, &y, &z);
    dest->set(x, y, z);
}

/**
 * GLUT keyboard key event handler.
 */
void keyboard(unsigned char key, int x, int y)
{
    printf("key %d pressed at %d,%d\n",key,x,y);
    fflush(stdout);

    switch (key)
    {
            // add/update a light based on the camera position.
        case 'L':
            g_raytracer->scene->lights.push_back(unique_ptr<Light>(new Light(getCameraPosition())));
            break;
        case 'l':
            g_raytracer->scene->lights[g_raytracer->scene->lights.size() - 1] = unique_ptr<Light>(new Light(getCameraPosition()));
            break;
        case 'r':
        {
            unsigned int winSizeX, winSizeY;
            winSizeX = g_raytracer->windowSizeX;
            winSizeY = g_raytracer->windowSizeY;

            // Pressing r will launch the raytracing.
            cout << "Raytracing" << endl;

            // Setup an image with the size of the current image.
            Image result(winSizeX, winSizeY);

            // produce the rays for each pixel, by first computing
            // the rays for the corners of the frustum.
            Vector3f origin00, dest00;
            Vector3f origin01, dest01;
            Vector3f origin10, dest10;
            Vector3f origin11, dest11;
            Vector3f origin, dest;

            produceRay(0, 0, &origin00, &dest00);
            produceRay(0, winSizeY - 1, &origin01, &dest01);
            produceRay(winSizeX - 1, 0, &origin10, &dest10);
            produceRay(winSizeX - 1, winSizeY - 1, &origin11, &dest11);

            for (unsigned int y = 0; y < winSizeY;++y) {
                for (unsigned int x = 0; x < winSizeX;++x) {
                    float xscale, yscale;
                    Vector3f rgb;

                    // produce the rays for each pixel, by interpolating
                    // the four rays of the frustum corners.
                    xscale = 1.0f - float(x) / (winSizeX - 1);
                    yscale = 1.0f - float(y) / (winSizeY - 1);

                    origin = yscale * (xscale * origin00 + (1 - xscale) * origin10) +
                    (1 - yscale) * (xscale * origin01 + (1 - xscale) * origin11);

                    dest = yscale * (xscale * dest00 + (1 - xscale) * dest10) +
                    (1 - yscale) * (xscale * dest01 + (1 - xscale) * dest11);

                    // launch raytracing for the given ray.
                    rgb = g_raytracer->performRayTracing(origin, dest);

                    // store the result in an image
                    result.setPixel(x,y, Color3(rgb));
                }

                cout << "Finished scanline " << y << endl;
            }

            cout << "Finished raytracing!" << endl;
            result.write("result.ppm");

            break;
        }
        case 27:     // touche ESC
            exit(0);
    }
    
    
    //produce the ray for the current mouse position
    Vector3f testRayOrigin, testRayDestination;
    produceRay(x, y, &testRayOrigin, &testRayDestination);

    g_raytracer->keyboard(key, x, y, testRayOrigin, testRayDestination);
}

