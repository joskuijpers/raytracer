#pragma once

#include "vertex.h"

/**
 * Triangle class.
 *
 * A triangle contains 3 indices to refer to vertex positions
 * and 3 indices to refer to texture coordinates (optional)
 */
class Triangle {
public:

#pragma mark - Constructors

    inline Triangle() {
        index = 0;
        v[0] = v[1] = v[2] = 0;
        n[0] = n[1] = n[2] = 0;
    }

    inline Triangle(const Triangle& t2) {
        index = t2.index;
        v[0] = t2.v[0];
        v[1] = t2.v[1];
        v[2] = t2.v[2];

        n[0] = t2.n[0];
        n[1] = t2.n[1];
        n[2] = t2.n[2];

        t[0] = t2.t[0];
        t[1] = t2.t[1];
        t[2] = t2.t[2];
    }

    inline Triangle(size_t i, unsigned int v0, unsigned int t0, unsigned int v1, unsigned int t1, unsigned int v2, unsigned int t2) {
        index = i;
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;

        n[0] = n[1] = n[2] = 0;

        t[0] = t0;
        t[1] = t1;
        t[2] = t2;
    }

    inline virtual ~Triangle () {}

#pragma mark - Operators

    inline Triangle& operator= (const Triangle& other) {
        v[0] = other.v[0];
        v[1] = other.v[1];
        v[2] = other.v[2];

        n[0] = other.n[0];
        n[1] = other.n[1];
        n[2] = other.n[2];

        t[0] = other.t[0];
        t[1] = other.t[1];
        t[2] = other.t[2];

        index = other.index;
        return (*this);
    }

    inline bool has_normal() {
        return  n[0] != 0 &&
                n[1] != 0 &&
                n[2] != 0;
    }

#pragma mark - Instance variables

    // vertex position
    unsigned int v[3];

    // vertex normals
    unsigned int n[3];

    // texture coordinate
    unsigned int t[3];

    size_t index;
};