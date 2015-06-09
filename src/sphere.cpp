#include "sphere.h"

#include "platform.h"

void sphere::draw()
{
    vector3f color;

    color = this->material.getKd();

    glTranslatef(this->translation[0], this->translation[1], this->translation[2]);
    glScalef(this->scale[0], this->scale[1], this->scale[2]);
    glRotatef(this->rotationAngle, this->rotation[0], this->rotation[1], this->rotation[2]);
    glColor3f(color[0], color[1], color[2]);

    glutSolidSphere(this->radius, 5, 5);
}

hit_result sphere::hit(ray ray [[gnu::unused]])
{
    return hit_result();
}