

#include "texture.h"

Vector3f Texture::getFromFace(Texture::Face face, float xf, float yf) {
    int x, y;
    if(face == FACE_FRONT){
        x = (((xf *size) + size) /2) + 2*size;
        y = (((yf *size) + size) /2) + size;
        //printf( "x: %d, y: %d", (int)x, (int)y);
    }else if (face == FACE_BACK){

        x = (((xf *size) + size)) /2;
        y = (((yf *size) + size) /2) + size;
    }else if (face == FACE_LEFT){

        x = (((xf *size) + size) /2) + 1*size;
        y = (((yf *size) + size) /2) + size;
    }else if (face == FACE_RIGHT){

        x = (((xf *size) + size) /2) + 3*size;
        y = (((yf *size) + size) /2) + size;
    }else if (face == FACE_TOP){

        x = (((xf *size) + size) /2) + 2*size;
        y = (((yf *size) + size) /2) ;
    }else if (face == FACE_BOTTOM){

        x = (((xf *size) + size) /2) + 2*size;
        y = (((yf *size) + size) /2) + size *2 ;

    }else {

        return Vector3<float>(0,0,0);
    }


    unsigned char r = image[y * 3 * width + x * 3];
    unsigned char g = image[y * 3 * width + x * 3 +1];
    unsigned char b = image[y * 3 * width + x * 3 +2];
    //printf("R: %d, G: %d, B: %d", r, g, b);
    return Vector3f(((float)r)/255.0f, ((float)g)/255.0f , ((float)b)/255.0f);

}
