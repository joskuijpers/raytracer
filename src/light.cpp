#include "platform.h"
#include "light.h"

void Light::draw(void) {
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    // Disable lighting
    glDisable(GL_LIGHTING);
    glColor3f(1, 1, selected ? 0 : 1);
    glPointSize(10);
    glBegin(GL_POINTS);
    glVertex3fv(position.pointer());

    glEnd();
    glPopAttrib();
}

void Light::move(Vector3f f) {
    position = f;
}
