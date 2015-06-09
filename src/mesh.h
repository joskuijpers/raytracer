#pragma once

#include <vector>
#include <map>
#include <string>

#include "vertex.h"
#include "material.h"
#include "scene_node.h"

using namespace std;

/**
 * Triangle class.
 *
 * A triangle contains 3 indices to refer to vertex positions
 * and 3 indices to refer to texture coordinates (optional)
 */
class triangle {
public:

#pragma mark - Constructors

    inline triangle() {
        v[0] = v[1] = v[2] = 0;
    }

    inline triangle (const triangle & t2) {
        v[0] = t2.v[0];
        v[1] = t2.v[1];
        v[2] = t2.v[2];

        t[0] = t2.t[0];
        t[1] = t2.t[1];
        t[2] = t2.t[2];

    }

    inline triangle (unsigned int v0, unsigned int t0, unsigned int v1, unsigned int t1, unsigned int v2, unsigned int t2) {
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;

        t[0] = t0;
        t[1] = t1;
        t[2] = t2;
    }

    inline virtual ~triangle () {}

#pragma mark - Operators

    inline triangle& operator= (const triangle &other) {
        v[0] = other.v[0];
        v[1] = other.v[1];
        v[2] = other.v[2];

        t[0] = other.t[0];
        t[1] = other.t[1];
        t[2] = other.t[2];

        return (*this);
    }

#pragma mark - Instance variables

    // vertex position
    unsigned int v[3];

    // texture coordinate
    unsigned int t[3];
};

/**
 * Mesh class.
 *
 * All objects that consist of triangles. Loaded from file.
 *
 * @warning Can't be used on stack!
 */
class mesh : public scene_node
{
public:
    mesh(const char *name) : scene_node(name) {}

    inline mesh(const std::vector<vertex> &v, const std::vector<triangle> &t)
    : scene_node("mesh"), vertices (v), triangles (t) {}

#pragma mark - Loading

    bool loadMesh(const char *filename, bool randomizeTriangulation);
    bool loadMaterial(const char *filename, std::map<std::string, unsigned int> &materialIndex);
    void computeVertexNormals();

#pragma mark - Drawing

    void draw();
    void drawSmooth();

#pragma mark - Raytracing

    hit_result hit(Ray ray);
    color3 apply(unsigned int level, hit_result hit_info);

private:
    int rayTriangleIntersect(Ray ray, triangle triangle, vector3f &point, float &hitDistance);

public:

#pragma mark - Properties

    // Vertices are the vertex positions, and normals of the mesh.
    std::vector<vertex> vertices;

    // texCoords are the texture coordinates, these are DIFFERENT indices in triangles.
    // in the current version, if you use textures, then you have to use texture coords everywhere
    // for convenience, vector3f is used, although only 2D tex coordinates are stored (x,y entry of the vector3f).
    std::vector<vector3f> texcoords;

    // triangles are the indices of the vertices involved in a triangle.
    // A triangle, thus, contains a triplet of values corresponding to the 3 vertices of a triangle.
    std::vector<triangle> triangles;

    // These are the material properties
    // each triangle (!), NOT (!) each vertex, has a material.
    // Use the triangle index to receive a material INDEX
    std::vector<unsigned int> triangleMaterials;

    // using the material index, you can then recover the material from this vector
    // the class material is defined just above
    std::vector<Material> materials;
};
