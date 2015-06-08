#include "raytracing.h"

#include <cstdio>
#include <cfloat>

#include "platform.h"
#include "compiler_opt.h"
#include "mesh.h"

using std::endl;
using std::cout;

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
    g_mainMesh.loadMesh("resource/cube.obj", true);
    g_mainMesh.computeVertexNormals();

    //one first move: initialize the first light source
    //at least ONE light source has to be in the scene!!!
    //here, we set it to the current location of the camera
    g_lightPositions.push_back(g_cameraPosition);
}

enum intersection_result : int {
    DEGENERATE = -1, // triangle is a line or point
    DISJOINT = 0, // no intersection
    INTERSECT = 1,
    PARALLEL = 2,
    OUTSIDE = 3
};

intersection_result rayTriangleIntersect(ray ray, triangle triangle, vector3f &point, float &hitDistance)
{
    vector3f v0, v1, v2;
    vector3f u, v, n;
    vector3f w0, w; // a thing?
    float a, b;

    // get the vertices
    v0 = g_mainMesh.vertices[triangle.v[0]].p;
    v1 = g_mainMesh.vertices[triangle.v[1]].p;
    v2 = g_mainMesh.vertices[triangle.v[2]].p;

    u = v1 - v0;
    v = v2 - v0;

    n = u.cross(v);
    if(n.getLength() == 0)
        return DEGENERATE;

    w0 = ray.origin - v0;
    a = -(n.dot(w0));
    b = n.dot(ray.direction);

    // Angle is much much tiny: parallel
    if (unlikely(fabs(b) < 0.00000001f)) {
        if(a == 0.f)
            return PARALLEL;
        else
            return DISJOINT;
    }

    // get intersection point
    hitDistance = a / b;
    if (hitDistance < 0.f)
        return DISJOINT;

    point = ray.origin + hitDistance * ray.direction;

    // test if the point lies inside the triangle, not only in its plane

    // these dot products are used multiple times. Precompute them
    float uu, uv, vv, wu, wv, denominator;

    uu = u.dot(u);
    uv = u.dot(v);
    vv = v.dot(v);
    w = point - v0;
    wu = w.dot(u);
    wv = w.dot(v);
    denominator = uv * uv - uu * vv;

    float s, t;

    s = (uv * wv - vv * wu) / denominator;
    if (s < 0.0 || s > 1.0)
        return OUTSIDE;

    t = (uv * wu - uu * wv) / denominator;
    if (t < 0.0 || (s + t) > 1.0)
        return OUTSIDE;

    return INTERSECT;
}

/**
 * @return return the color of the pixel
 */
rgb_value performRayTracing(const vector3f &origin, const vector3f &dest)
{
    ray ray(origin, dest);
    size_t triangleIndex = 0;
    triangle nearest;
    float near = FLT_MAX;
    material m;

    for(size_t it = 0; it < g_mainMesh.triangles.size(); ++it) {
        intersection_result result;
        vector3f intPoint;
        float hit;
        triangle t = g_mainMesh.triangles[it];

        result = rayTriangleIntersect(ray, t, intPoint, hit);

        if (likely(result != INTERSECT))
            continue;

        if(hit >= near)
            continue;

        near = hit;
        nearest = t;
        triangleIndex = it;
    }

    // No hit
    if(near == FLT_MAX)
        return rgb_value(0,0,0);

    // Get material
    m = g_mainMesh.materials[g_mainMesh.triangleMaterials[triangleIndex]];

    vector3f diffuse = m.getKd();

    return rgb_value(diffuse);
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
void yourKeyboardFunc(char t, int mouseX, int mouseY, const vector3f &rayOrigin, const vector3f &rayDestination)
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
