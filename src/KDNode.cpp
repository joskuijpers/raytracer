//
// Created by jens on 21-6-15.
//

#include "KDNode.h"

KDNode* KDNode::buildTree(vector<WsTriangle> triangles, int depth) {
    KDNode* node = new KDNode();
    node->triangles = triangles;

    if(triangles.size() == 0)
        return node;
    node->box = KDNode::createBoundingBoxFromTriangle(triangles[0]);
    if(triangles.size() < MIN_TRIANGLES || depth > MAX_DEPTH) {
        node->left->triangles = vector<WsTriangle>();
        node->right->triangles = vector<WsTriangle>();
        return node;
    }
    Vector3f center;

    for(WsTriangle t : triangles) {
        node->box.extend(KDNode::createBoundingBoxFromTriangle(t));
        center += KDNode::calculateCenterOfTriangle(t);
    }

    vector<WsTriangle> left_triangles;
    vector<WsTriangle> right_triangles;
    int axis = node->box.findLongestAxis();
    for(WsTriangle t : triangles) {
        center[axis] >= KDNode::calculateCenterOfTriangle(t)[axis] ? right_triangles.push_back(t) : left_triangles.push_back(t);
    }
    node->left = buildTree(left_triangles, depth + 1);
    node->right = buildTree(right_triangles, depth + 1);

    return node;
}

Vector3f KDNode::calculateCenterOfTriangle(WsTriangle t) {
    return Vector3<float>(nullptr);
}

AABoundingBox KDNode::createBoundingBoxFromTriangle(WsTriangle t) {
    AABoundingBox box;
    box.extend(t.v[0].p);
    box.extend(t.v[1].p);
    box.extend(t.v[2].p);
    return AABoundingBox();
}
