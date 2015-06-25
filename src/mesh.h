#pragma once

#include <vector>
#include <map>
#include <string>

#include "vertex.h"
#include "material.h"
#include "scene_node.h"
#include "triangle.h"
#include "kdnode.h"

using namespace std;


/**
 * Mesh class.
 *
 * All objects that consist of triangles. Loaded from file.
 *
 * @warning Can't be used on stack!
 */
class Mesh : public SceneNode
{
public:
    Mesh(const char *name) : SceneNode(name) {}

    inline Mesh(const std::vector<vertex> &v, const std::vector<Triangle> &t)
    : SceneNode("mesh"), vertices (v), triangles (t) {}

#pragma mark - Loading

    bool loadMesh(const char *filename, bool randomizeTriangulation);
    bool loadMaterial(const char *filename, std::map<std::string, unsigned int> &materialIndex);
    void computeVertexNormals();

#pragma mark - Drawing

    void draw(void);
    void drawNormals(void);
    void drawStructure(void);

#pragma mark - Raytracing

    void createBoundingBox();
    hit_result hit(Ray ray, shared_ptr<SceneNode> skip = nullptr);

private:
    int rayTriangleIntersect(Ray ray, Triangle triangle, Vector3f &point, float &hitDistance, float &s, float &t);
    Vector3f normalOfFace(Triangle triangle, float s, float t);
    void findTriangles(Ray ray, KDNode* node, vector<Triangle>& triangles);
public:

#pragma mark - Properties

    // Vertices are the vertex positions, and normals of the mesh.
    std::vector<vertex> vertices;

    // texCoords are the texture coordinates, these are DIFFERENT indices in triangles.
    // in the current version, if you use textures, then you have to use texture coords everywhere
    // for convenience, Vector3f is used, although only 2D tex coordinates are stored (x,y entry of the Vector3f).
    std::vector<Vector3f> texcoords;

    // triangles are the indices of the vertices involved in a triangle.
    // A triangle, thus, contains a triplet of values corresponding to the 3 vertices of a triangle.
    std::vector<Triangle> triangles;

    std::vector<Vector3f> normals;

    // These are the material properties
    // each triangle (!), NOT (!) each vertex, has a material.
    // Use the triangle index to receive a material INDEX
    std::vector<unsigned int> triangleMaterials;

    // using the material index, you can then recover the material from this vector
    // the class material is defined just above
    std::vector<Material> materials;

    KDNode* treeRoot;
};
