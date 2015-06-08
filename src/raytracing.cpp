#include "raytracing.h"

#include <cstdio>

#include "platform.h"


//temporary variables
//these are only used to illustrate
//a simple debug drawing. A ray
vector3f testRayOrigin;
vector3f testRayDestination;


//use this function for any preprocessing of the mesh.
void init(void)
{
    //load the mesh file
    //please realize that not all OBJ files will successfully load.
    //Nonetheless, if they come from Blender, they should, if they
    //are exported as WavefrontOBJ.
    //PLEASE ADAPT THE LINE BELOW TO THE FULL PATH OF THE dodgeColorTest.obj
    //model, e.g., "C:/temp/myData/GraphicsIsFun/dodgeColorTest.obj",
    //otherwise the application will not load properly
    g_mainMesh.loadMesh("resource/dodgeColorTest.obj", true);
    g_mainMesh.computeVertexNormals();

    //one first move: initialize the first light source
    //at least ONE light source has to be in the scene!!!
    //here, we set it to the current location of the camera
    g_lightPositions.push_back(g_cameraPosition);
}

/**
 * @return return the color of the pixel
 */
vector3f performRayTracing(const vector3f &origin __attribute__((unused)), const vector3f &dest)
{
    return vector3f(dest[0], dest[1], dest[2]);
}

void drawLights(void)
{
    // Draw the lights as white dits
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    // Disable lighting
    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);
    glPointSize(10);
    glBegin(GL_POINTS);

    for (size_t i = 0; i < g_lightPositions.size(); ++i) {
        glVertex3fv(g_lightPositions[i].pointer());
    }

    glEnd();
    glPopAttrib();
}

void yourDebugDraw(void)
{
    //draw open gl debug stuff
    //this function is called every frame

    //let's draw the mesh
    g_mainMesh.draw();

    drawLights();

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
    glVertex3fv(g_lightPositions[0].pointer());
    glEnd();
    glPopAttrib();

    //draw whatever else you want...
    ////glutSolidSphere(1,10,10);
    ////allows you to draw a sphere at the origin.
    ////using a glTranslate, it can be shifted to whereever you want
    ////if you produce a sphere renderer, this
    ////triangulated sphere is nice for the preview
}


//yourKeyboardFunc is used to deal with keyboard input.
//t is the character that was pressed
//x,y is the mouse position in pixels
//rayOrigin, rayDestination is the ray that is going in the view direction UNDERNEATH your mouse position.
//
//A few keys are already reserved:
//'L' adds a light positioned at the camera location to the MyLightPositions vector
//'l' modifies the last added light to the current
//    camera position (by default, there is only one light, so move it with l)
//    ATTENTION These lights do NOT affect the real-time rendering.
//    You should use them for the raytracing.
//'r' calls the function performRaytracing on EVERY pixel, using the correct associated ray.
//    It then stores the result in an image "result.ppm".
//    Initially, this function is fast (performRaytracing simply returns
//    the target of the ray - see the code above), but once you replaced
//    this function and raytracing is in place, it might take a
//    while to complete...
void yourKeyboardFunc(char t, int x, int y, const vector3f &rayOrigin, const vector3f &rayDestination)
{

    //here, as an example, I use the ray to fill in the values for my upper global ray variable
    //I use these variables in the debugDraw function to draw the corresponding ray.
    //try it: Press a key, move the camera, see the ray that was launched as a line.
    testRayOrigin = rayOrigin;
    testRayDestination = rayDestination;

    // do here, whatever you want with the keyboard input t.

    //...


    std::cout << t << " pressed! The mouse was in location " << x << ", " << y << "!" <<std::endl;
}
