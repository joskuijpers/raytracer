#pragma once

#include "triangle.h"
#include "aabb.h"
#include <vector>
#include "config.h"

class KDNode {
public:
#pragma mark - Methods
    static KDNode* buildTree(vector<vertex>& vertices, vector<Triangle>& triangles, int depth);
    static AABoundingBox createBoundingBoxFromTriangle(vector<vertex> vertices, Triangle t);
    static Vector3f calculateCenterOfTriangle(vector<vertex> vertices, Triangle t);
    void draw(void);

#pragma mark - Properties
    AABoundingBox box;
    KDNode* left;
    KDNode* right;
    vector<Triangle> triangles;

};