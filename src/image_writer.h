#pragma once

#include "platform.h"

#include <stdlib.h>
#include <math.h>
#include <assert.h>

//image class
//This class can be used to write your final result to an image.
//You can open the image using a PPM viewer.

//YOU CAN IGNORE THIS CODE!
class rgb_value
{
public:
    rgb_value(float r = 0, float g = 0, float b = 0, float a = 1) : r(r), g(g), b(b), a(a)
    {
        if (r > 1)
            r = 1.f;
        else if (r < 0)
            r = 0.f;

        if (g > 1)
            g = 1.f;
        else if (g < 0)
            g = 0.f;

        if (b > 1)
            b = 1.f;
        else if (b < 0)
            b = 0.f;

        if (a > 1)
            a = 1.f;
        else if (a < 0)
            a = 0.f;
    };

    float operator[](int i) const
    {
        switch(i)
        {
            case 0:
                return r;
            case 1:
                return g;
            case 2:
                return b;
            case 3:
                return a;
            default:
                return r;
        }
    }

    float& operator[](int i)
    {
        switch(i)
        {
            case 0:
                return r;
            case 1:
                return g;
            case 2:
                return b;
            case 3:
                return a;
            default:
                return r;
        }
    }

    float r, g, b, a;
};

class image
{
public:
    image(int width, int height) : _width(width), _height(height)
    {
        _image.resize(3 * _width * _height);
    }

    void setPixel(int i, int j, const rgb_value &rgb)
    {
        _image[3 * (_width * j + i)] = rgb[0];
        _image[3 * (_width * j + i) + 1] = rgb[1];
        _image[3 * (_width * j + i) + 2] = rgb[2];

    }

    std::vector<float> _image;
    int _width;
    int _height;

    bool writeImage(const char *filename);
};

bool image::writeImage(const char *filename)
{
    FILE *file;
    int t;

    file = fopen(filename, "wb");

    if (!file)
    {
        printf("Dump file problem... fopen\n");
        return false;
    }

    fprintf(file, "P6\n%i %i\n255\n", _width, _height);

    std::vector<unsigned char> imageC(_image.size());

    for (unsigned int i = 0; i < _image.size(); ++i) {
        imageC[i] = (unsigned char)(_image[i] * 255.f);
    }

    t = (int)fwrite(&(imageC[0]), _width * _height * 3, 1, file);

    if (t != 1)
    {
        printf("Dump file problem... fwrite\n");
        return false;
    }
    
    fclose(file);
    return true;
}
