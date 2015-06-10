#include "aabb.h"

#include <iostream>

#include "platform.h"

using namespace std;

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

std::ostream& operator<< (std::ostream &output, const AABoundingBox &bb) {
    output << "(" << bb.min << "," << bb.max << ")";

    return output;
}
