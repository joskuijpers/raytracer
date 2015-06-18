#pragma once

#include "vector3.h"

class vertex {
public:

#pragma mark - Constructors

    inline vertex() {}
    inline vertex(const Vector3f &p) : p (p) {}
    inline vertex(const Vector3f &p, const Vector3f &n) : p (p), n (n) {}
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
    Vector3f p;

    /// The normal of the vertex
    Vector3f n;
};
