//
// Created by bernard on 19-6-15.
//

#include "testray.h"
#include "platform.h"

void TestRay::draw() {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glBegin(GL_LINES);
    {
        Vector3f hitpoint = ray.origin + ray.direction *length;
        glColor3fv(color.pointer());
        glVertex3fv(ray.origin.pointer());
        glVertex3fv(hitpoint.pointer());
    }
    glEnd();
    glPointSize(12);
    glBegin(GL_POINTS);
    {
        Vector3f hitpoint = ray.origin + ray.direction *(length-0.001);
        glVertex3fv(hitpoint.pointer());
    }
    glEnd();
    glPopAttrib();

}


void writeText(char *text) {
    char *p = nullptr;
    for (p = text; *p != '\0'; p++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
}

void TestRay::drawInfo() {
        //draw the text box
        const double corners[4][3] = {{0.0,   0.0,   0.0},
                                      {100.0, 0.0,   0.0},
                                      {100.0, 100.0, 0.0},
                                      {0.0,   100.0, 0.0}};

        Vector3f textpos = ray.origin + ray.direction * 2;
        glPushMatrix();
        glColor3f(0.0f, 1.0f, 0.0f);
        //glRotatef(g_raytracer->scene->rotationAngle, g_raytracer->scene->rotation[0], g_raytracer->scene->rotation[1], g_raytracer->scene->rotation[2]);
        //glLoadIdentity();
        glTranslatef(textpos[0], textpos[1], textpos[2]);
        glScalef(0.8, 0.8, 0.8);
        glPushMatrix();
        glScalef(0.005, 0.005, 0.005);
        glTranslatef(0.0, -85.0, -5.0);
        glColor4f(1.0, 1.0, 1.0, 0.1);
        glBegin(GL_QUADS);
        {
            for (int i = 0; i < 4; i++) {
                glVertex3dv(corners[i]);
            }

        }
        glEnd();
        glPopMatrix();
        glColor3f(0.2, 0.2, 0.2);

        glScalef(0.0008f, 0.0008f, 0.0008f);
        glPushMatrix();
        {
            Vector3f tmp = applyResult.ambiantColor * 2; //cheat a bit by making it brighter
            glColor3fv(tmp.pointer());
            writeText("ambiant:");
            glBegin(GL_QUADS);
            for (int i = 0; i < 5; i++) {
                glVertex3dv(corners[i]);
            }
            glEnd();
        }
        glPopMatrix();
        glTranslatef(0.0f, -120.0f, 0.0f);
        glPushMatrix();
        {
            Vector3f tmp = applyResult.diffuseColor * 2; //cheat a bit by making it brighter
            glColor3fv(tmp.pointer());
            writeText("diffuse:");
            glBegin(GL_QUADS);
            for (int i = 0; i < 5; i++) {
                glVertex3dv(corners[i]);
            }
            glEnd();
        }
        glPopMatrix();
        glTranslatef(0.0f, -120.0f, 0.0f);
        glPushMatrix();
        {
            Vector3f tmp = applyResult.specularColor * 2; //cheat a bit by making it brighter
            glColor3fv(tmp.pointer());
            writeText("specular:");
            glBegin(GL_QUADS);
            for (int i = 0; i < 5; i++) {
                glVertex3dv(corners[i]);
            }
            glEnd();
        }
        glPopMatrix();
        glTranslatef(0.0f, -120.0f, 0.0f);
        glPushMatrix();
        {
            Vector3f tmp = applyResult.reflectedColor * 2;
            glColor3fv(tmp.pointer());
            writeText("reflection:");
            glBegin(GL_QUADS);
            for (int i = 0; i < 5; i++) {
                glVertex3dv(corners[i]);
            }
            glEnd();
        }
        glPopMatrix();

        glPopMatrix();
}
