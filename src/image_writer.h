#pragma once

#include "platform.h"

#include <stdlib.h>
#include <math.h>
#include <assert.h>


class rgb_value
{
public:
    rgb_value(float r = 0.f, float g = 0.f, float b = 0.f, float a = 1.f) : r(r), g(g), b(b), a(a)
    {
        if (r > 1.f)
            r = 1.f;
        else if (r < 0.f)
            r = 0.f;

        if (g > 1.f)
            g = 1.f;
        else if (g < 0.f)
            g = 0.f;

        if (b > 1.f)
            b = 1.f;
        else if (b < 0.f)
            b = 0.f;

        if (a > 1.f)
            a = 1.f;
        else if (a < 0.f)
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

/**
 * Image class.
 *
 * Can write an image to file in PPM format.
 */
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
    if (!file) {
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
