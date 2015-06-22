#pragma once

#include "Triangle.h"
#include "aabb.h"
#include <vector>

#define MAX_DEPTH 20
#define MIN_TRIANGLES 100

class KDNode {
public:
#pragma mark - Methods
    static KDNode* buildTree(vector<vertex>& vertices, vector<Triangle>& triangles, int depth);
    static AABoundingBox createBoundingBoxFromTriangle(vector<vertex> vertices, Triangle t);
    static Vector3f calculateCenterOfTriangle(vector<vertex> vertices, Triangle t);

#pragma mark - Properties
    AABoundingBox box;
    KDNode* left;
    KDNode* right;
    vector<Triangle> triangles;

};