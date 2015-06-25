#pragma once

#include "vector3.h"

class Color3
{
public:
    Color3(float r = 0.f, float g = 0.f, float b = 0.f) : r(r), g(g), b(b)
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

    /// Convert from vector to rgb
    Color3(Vector3f vector) : Color3(vector[0], vector[1], vector[2]) {}

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