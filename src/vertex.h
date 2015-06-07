#pragma once

#include "vector3.h"

class vertex {
public:

#pragma mark - Constructors

    inline vertex() {}
    inline vertex(const vector3f &p) : p (p) {}
    inline vertex(const vector3f &p, const vector3f &n) : p (p), n (n) {}
    inline vertex(const vertex &v) : p (v.p), n (v.n) {}

    inline virtual ~vertex () {}

#pragma mark - Operators

    inline vertex& operator= (const vertex &v) {
        p = v.p;
        n = v.n;

        return (*this);
    }

#pragma mark - Instance variables

    /// The vertex
    vector3f p;

    /// The normal of the vertex
    vector3f n;
};
