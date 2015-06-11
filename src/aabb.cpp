#include "aabb.h"

#include <iostream>

#include "platform.h"
#include "scene_node.h"
#include "scene.h"

using namespace std;

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

void AABoundingBox::draw() {
    // All corners
    Vector3f mmm = min;
    Vector3f Mmm = Vector3f(max[0],min[1],min[2]);
    Vector3f mMm = Vector3f(min[0],max[1],min[2]);
    Vector3f MMm = Vector3f(max[0],max[1],min[2]);
    Vector3f mmM = Vector3f(min[0],min[1],max[2]);
    Vector3f MmM = Vector3f(max[0],min[1],max[2]);
    Vector3f mMM = Vector3f(min[0],max[1],max[2]);
    Vector3f MMM = max;

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_LIGHTING);
    glColor3fv(color.pointer());
    glLineWidth(1);

    glBegin(GL_LINES);
    {
        // Bottom
        glVertex3fv(mmm.pointer());
        glVertex3fv(Mmm.pointer());

        glVertex3fv(Mmm.pointer());
        glVertex3fv(MmM.pointer());

        glVertex3fv(MmM.pointer());
        glVertex3fv(mmM.pointer());

        glVertex3fv(mmM.pointer());
        glVertex3fv(mmm.pointer());

        // Top
        glVertex3fv(mMm.pointer());
        glVertex3fv(MMm.pointer());

        glVertex3fv(MMm.pointer());
        glVertex3fv(MMM.pointer());

        glVertex3fv(MMM.pointer());
        glVertex3fv(mMM.pointer());

        glVertex3fv(mMM.pointer());
        glVertex3fv(mMm.pointer());

        // Styles
        glVertex3fv(mmm.pointer());
        glVertex3fv(mMm.pointer());

        glVertex3fv(Mmm.pointer());
        glVertex3fv(MMm.pointer());
        
        glVertex3fv(MmM.pointer());
        glVertex3fv(MMM.pointer());
        
        glVertex3fv(mmM.pointer());
        glVertex3fv(mMM.pointer());
    }
    glEnd();

    // Draw min and max
    glPointSize(8);
    glBegin(GL_POINTS);
        glVertex3fv(min.pointer());
        glVertex3fv(max.pointer());
    glEnd();

    glPopAttrib();
}

/// @note http://tavianator.com/cgit/dimension.git/tree/libdimension/bvh.c#n191
bool AABoundingBox::intersection(Ray ray, float t [[gnu::unused]]) {

    float txm = (min[0] - ray.origin[0]) * ray.direction_inv[0];
    float txM = (max[0] - ray.origin[0]) * ray.direction_inv[0];

    float tmin = MIN(txm, txM);
    float tmax = MAX(txm, txM);

    float tym = (min[1] - ray.origin[1]) * ray.direction_inv[1];
    float tyM = (max[1] - ray.origin[1]) * ray.direction_inv[1];

    tmin = MAX(tmin, MIN(tym, tyM));
    tmax = MIN(tmax, MAX(tym, tyM));

    float tzm = (min[2] - ray.origin[2]) * ray.direction_inv[2];
    float tzM = (max[2] - ray.origin[2]) * ray.direction_inv[2];

    tmin = MAX(tmin, MIN(tzm, tzM));
    tmax = MIN(tmax, MAX(tzm, tzM));

    // tmin: global entry point of box
    // tmax: global exit point of box

//    if(tmin <= tmax && tmax > 0.f) {
//        Vector3f in, out;
//        in = ray.origin + ray.direction * tmin;
//        out = ray.origin + ray.direction * tmax;
//        cout << "in " << in << " out " << out << endl;
//    }

//    return tmin <= tmax && tmax > 0.f;
    return tmax >= MAX(0.f, tmin);// && tmin < t;

//    return true;
}