#pragma once

#include "platform.h"

#include <cmath>
#include <cassert>
#include <vector>

class rgb_value
{
public:
    rgb_value(float r = 0.f, float g = 0.f, float b = 0.f) : r(r), g(g), b(b)
    {
        if (r > 1.f)
            this->r = 1.f;
        else if (r < 0.f)
            this->r = 0.f;

        if (g > 1.f)
            this->g = 1.f;
        else if (g < 0.f)
            this->g = 0.f;

        if (b > 1.f)
            this->b = 1.f;
        else if (b < 0.f)
            this->b = 0.f;
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
            default:
                return r;
        }
    }

    float r, g, b;
};

/**
 * Image class.
 *
 * Can write an image to file in PPM format.
 */
class image
{
public:
    image(int width, int height) : _width(width), _height(height) {
        _image.resize(3 * _width * _height);
    }

    void setPixel(int i, int j, const rgb_value &rgb) {
        _image[3 * (_width * j + i)] = rgb[0];
        _image[3 * (_width * j + i) + 1] = rgb[1];
        _image[3 * (_width * j + i) + 2] = rgb[2];
    }

    std::vector<float> _image;
    int _width;
    int _height;

    bool writeImage(const char *filename);
};
