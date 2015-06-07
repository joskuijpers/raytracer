#pragma once

#include "vector3.h"

class Vertex {
public:

#pragma mark - Constructors

    inline Vertex () {}
    inline Vertex (const vector3f &p) : p (p) {}
    inline Vertex (const vector3f &p, const vector3f &n) : p (p), n (n) {}
    inline Vertex (const Vertex &v) : p (v.p), n (v.n) {}

    inline virtual ~Vertex () {}

#pragma mark - Operators

    inline Vertex& operator= (const Vertex &v) {
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
