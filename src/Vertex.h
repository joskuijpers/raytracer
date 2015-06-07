#pragma once

#include "vector3.h"

/************************************************************
 * Vertex of the mesh (containing normal n and vertetx v)
 ************************************************************/
class Vertex {
public:
    inline Vertex () {}
    inline Vertex (const vector3f &p) : p (p) {}
    inline Vertex (const vector3f &p, const vector3f &n) : p (p), n (n) {}
    inline Vertex (const Vertex &v) : p (v.p), n (v.n) {}
    inline virtual ~Vertex () {}
    inline Vertex& operator= (const Vertex &v) {
        p = v.p;
        n = v.n;

        return (*this);
    }

    vector3f p;
    vector3f n;
};
