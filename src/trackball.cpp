#include "trackball.h"

/** \file traqueboule.h
Utilitaires pour appliquer une traqueboule a une fenetre OpenGL.
*/

#include "platform.h"

/** Placement de la scene par rapport a la camera */
GLdouble tb_matrix[16] =   { 1,0,0,0,
                             0,1,0,0,
                             0,0,1,0,
                             0,0,0,1  };
GLdouble tb_inverse[16] =  { 1,0,0,0,
                             0,1,0,0,
                             0,0,1,0,
                             0,0,0,1  };

double tb_angle, tb_x, tb_y, tb_z;

/** Gestion de la souris */
int tb_ancienX, tb_ancienY, tb_tournerXY=0, tb_translaterXY=0, tb_bougerZ=0;

