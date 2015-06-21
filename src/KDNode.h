#pragma once

#include "Triangle.h"
#include "aabb.h"
#include <vector>

#define MAX_DEPTH 25
#define MIN_TRIANGLES 50

class KDNode {
public:
#pragma mark - Methods
    static KDNode* buildTree(vector<WsTriangle> triangles, int depth);
    static AABoundingBox createBoundingBoxFromTriangle(WsTriangle t);
    static Vector3f calculateCenterOfTriangle(WsTriangle t);


#pragma mark - Properties
    AABoundingBox box;
    KDNode* left;
    KDNode* right;
    vector<WsTriangle> triangles;

};