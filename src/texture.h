#pragma once


#include <string>
#include <vector>
#include <iostream>
#include "lodepng.h"
#include "vector3.h"

class Texture {
unsigned width, height;
std::vector<unsigned char> image;

public:

    enum Face{
        FACE_TOP,
        FACE_FRONT,
        FACE_LEFT,
        FACE_RIGHT,
        FACE_BACK,
        FACE_BOTTOM
    };

    int frontx = 1200;
    int fronty = 800;
    int size = 500;

    Texture(std::string filename, int frontx, int fronty, int size)
    :frontx(frontx), fronty(fronty), size(size)
    {
        if(unsigned error = lodepng::decode(image, width, height, filename, LodePNGColorType::LCT_RGB)){
            std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
        }
        std::cout << "loaded texture with width: " << width << " Height: " << height << std::endl;
    }

    Vector3f getFromFace(Face face, float x, float y);

    std::vector<unsigned char> getImageData(){ return image; }
};