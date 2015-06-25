#pragma once

#include "platform.h"

#include <cmath>
#include <cassert>
#include <vector>

#include "color.h"

/**
 * Image class.
 *
 * Can write an image to file in PPM format.
 */
class Image
{
public:
    Image(int width, int height) : width(width), height(height) {
        image.resize(3 * width * height);
    }

    void setPixel(int i, int j, const Color3 &rgb) {
        image[3 * (width * j + i)] = rgb[0];
        image[3 * (width * j + i) + 1] = rgb[1];
        image[3 * (width * j + i) + 2] = rgb[2];
    }

    std::vector<float> image;
    int width;
    int height;

    bool write(const char *filename);
};
