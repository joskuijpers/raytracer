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

/// @note http://www.cs.utah.edu/~awilliam/box/box.pdf
bool AABoundingBox::intersection(Ray ray, float t [[gnu::unused]]) {
    Vector3f params[2] = {min,max};

    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    // param[0] = min, param[1] = max

    tmin = (params[ray.sign[0]][0] - ray.origin[0]) * ray.direction_inv[0];
    tmax = (params[1-ray.sign[0]][0] - ray.origin[0]) * ray.direction_inv[0];

    tymin = (params[ray.sign[1]][1] - ray.origin[1]) * ray.direction_inv[1];
    tymax = (params[1-ray.sign[1]][1] - ray.origin[1]) * ray.direction_inv[1];

    if((tmin > tymax) || (tymin > tmax))
        return false;

    tmin = MAX(tmin,tymin);
    tmax = MIN(tmax,tymax);

    tzmin = (params[ray.sign[2]][2] - ray.origin[2]) * ray.direction_inv[2];
    tzmax = (params[1-ray.sign[2]][2] - ray.origin[2]) * ray.direction_inv[2];

    if((tmin > tzmax) || (tzmin > tmax))
        return false;

    tmin = MAX(tmin,tzmin);
    tmax = MIN(tmax,tzmax);

    return tmin < FLT_MAX && tmax > 0;
}