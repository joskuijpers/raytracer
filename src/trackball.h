#pragma once

/** \file traqueboule.h
Utilitaires pour appliquer une traqueboule a une fenetre OpenGL.
*/

#include "platform.h"

#include <cmath>
#include <cstdio>

#include "matrix.h"
#include "vector3.h"

static const float speedfact = 0.2;

/** votre fonction d'affichage */
void display();

/** Placement de la scene par rapport a la camera */
extern GLdouble tb_matrix[16];
extern GLdouble tb_inverse[16];

/** Gestion de la souris */
extern int tb_ancienX, tb_ancienY, tb_tournerXY, tb_translaterXY, tb_bougerZ;


/** Lit dans la matrice courante la position initiale du point de vue */
inline void tbInitTransform()
{
    glGetDoublev( GL_MODELVIEW_MATRIX, tb_matrix );
    inverse( tb_matrix, tb_inverse );
}

/** Applique la transformation de point de vue */
inline void tbVisuTransform()
{
    glMultMatrixd( tb_matrix );
};

/** Affiche l'aide */
inline void tbHelp()
{
    printf("Left button: turn in XY,\n");
    printf("Right button: translate in XY,\n");
    printf("Middle button: move along Z.\n");
}

/** Gere les boutons de la souris */
inline void tbMouseFunc( int button, int state, int x, int y )
{
    int modifiers = glutGetModifiers();

    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (modifiers & GLUT_ACTIVE_SHIFT) { // Shift pressed -> zoom
                if (state == GLUT_DOWN) {
                    tb_bougerZ = 1;
                    tb_ancienX = x;
                    tb_ancienY = y;
                } else if (state == GLUT_UP) {
                    tb_bougerZ = 0;
                }
            } else { // No shift -> rotate
                if (state == GLUT_DOWN) {
                    tb_tournerXY = 1;
                    tb_ancienX = x;
                    tb_ancienY = y;
                } else if (state == GLUT_UP) {
                    tb_tournerXY = 0;
                }
            }
            break;
        case GLUT_MIDDLE_BUTTON:
            if (state == GLUT_DOWN) {
                tb_bougerZ = 1;
                tb_ancienX = x;
                tb_ancienY = y;
            } else if (state == GLUT_UP) {
                tb_bougerZ = 0;
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN) {
                tb_translaterXY = 1;
                tb_ancienX = x;
                tb_ancienY = y;
            } else if (state == GLUT_UP) {
                tb_translaterXY = 0;
            }
            break;
    }
}


/** Traite le changement de position de la souris */
inline void tbMotionFunc( int x, int y )
{
    double dx,dy,nrm, tx,ty,tz;

    if( tb_tournerXY || tb_translaterXY || tb_bougerZ )
    {
        /* deplacement */
        dx = x - tb_ancienX;
        dy = tb_ancienY - y; /* axe vertical dans l'autre sens */

        if( tb_tournerXY )
        {
            tx = tb_matrix[12];
            tb_matrix[12]=0;
            ty = tb_matrix[13];
            tb_matrix[13]=0;
            tz = tb_matrix[14];
            tb_matrix[14]=0;

            nrm = sqrt( dx*dx+dy*dy+dx*dx+dy*dy )*speedfact;
            glLoadIdentity();
            glRotatef( nrm, -dy, dx, 0 );/*axe perpendiculaire au deplacement*/
            glMultMatrixd( tb_matrix );
            glGetDoublev( GL_MODELVIEW_MATRIX, tb_matrix );

            tb_matrix[12] = tx;
            tb_matrix[13] = ty;
            tb_matrix[14] = tz;
        }
        else if( tb_translaterXY )
        {
            tb_matrix[12] += dx/100.0*speedfact;
            tb_matrix[13] += dy/100.0*speedfact;
        }
        else if( fabs(dx)>fabs(dy) )
        { // rotation z
            tx = tb_matrix[12];
            tb_matrix[12]=0;
            ty = tb_matrix[13];
            tb_matrix[13]=0;
            tz = tb_matrix[14];
            tb_matrix[14]=0;

            glLoadIdentity();
            glRotatef( dx, 0,0,-1 );/*axe perpendiculaire a l'ecran*/
            glMultMatrixd( tb_matrix );
            glGetDoublev( GL_MODELVIEW_MATRIX, tb_matrix );

            tb_matrix[12] = tx;
            tb_matrix[13] = ty;
            tb_matrix[14] = tz;
        }
        else if( fabs(dy)>fabs(dx) )
        {
            tb_matrix[14] -= dy/100.0*speedfact;
        }
        tb_ancienX = x;
        tb_ancienY = y;
        inverse( tb_matrix, tb_inverse );
        glutPostRedisplay();
    }
}

/** Traite le changement de position de la souris */
inline void tbRotate( double angle, double x, double y, double z )
{
    double tx,ty,tz;

    tx = tb_matrix[12];
    tb_matrix[12]=0;
    ty = tb_matrix[13];
    tb_matrix[13]=0;
    tz = tb_matrix[14];
    tb_matrix[14]=0;

    glLoadIdentity();
    glRotatef( angle, x, y, z );
    glMultMatrixd( tb_matrix );
    glGetDoublev( GL_MODELVIEW_MATRIX, tb_matrix );

    tb_matrix[12] = tx;
    tb_matrix[13] = ty;
    tb_matrix[14] = tz;

    inverse( tb_matrix, tb_inverse );
    glutPostRedisplay();
}

/// Projection dans le repere du monde
inline void tbProject( const GLdouble *m, const GLdouble* p, GLdouble* q )
{
    double pp[4];
    //cout<<"tb, matrix: "; printMatrix(tb_matrix); cout<<endl;
    //cout<<"tb, inverse: "; printMatrix(tb_inverse); cout<<endl;
    project( m, p, pp );
    //cout<<"proj: "<<pp[0]<<", "<<pp[1]<<", "<<pp[2]<<", "<<pp[3]<<endl;
    project( tb_inverse, pp, q );
    //cout<<"projRep: "<<q[0]<<", "<<q[1]<<", "<<q[2]<<", "<<q[3]<<endl;
}

inline void tbProject( const GLdouble* p, GLdouble* q )
{
    //cout<<"proj: "<<pp[0]<<", "<<pp[1]<<", "<<pp[2]<<", "<<pp[3]<<endl;
    project( tb_inverse, p, q );
    //cout<<"projRep: "<<q[0]<<", "<<q[1]<<", "<<q[2]<<", "<<q[3]<<endl;
}
inline Vector3f getCameraPosition()
{
	const GLdouble p[]={0,0,0,1};
	GLdouble LightP[4];
	tbProject(p, LightP);
	Vector3f LightPos;
	LightPos[0]=LightP[0];
	LightPos[1]=LightP[1];
	LightPos[2]=LightP[2];
	return LightPos;
}

inline Vector3f getWorldPositionOfPixel(unsigned int px, unsigned int py)
{

	double mv[16];
	double pr[16];
	int vp[4];
	glGetDoublev(GL_MODELVIEW_MATRIX,mv);
	glGetDoublev(GL_PROJECTION_MATRIX,pr);
	glGetIntegerv(GL_VIEWPORT,vp);

	double x,y,z;
	gluUnProject(double(px),double(py),0,mv,pr,vp,&x,&y,&z);



	return Vector3f(x,y,z);
}
