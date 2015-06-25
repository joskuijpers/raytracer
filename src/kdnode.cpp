#include "kdnode.h"

KDNode* KDNode::buildTree(vector<vertex>& vertices, vector<Triangle>& triangles, int depth) {
    KDNode* node = new KDNode();
    node->box = AABoundingBox();
    node->triangles = triangles;
    node->left = NULL;
    node->right = NULL;

    if(triangles.size() == 0)
        return node;
    node->box = KDNode::createBoundingBoxFromTriangle(vertices, triangles[0]);
    for(Triangle t : triangles)
        node->box.extend(KDNode::createBoundingBoxFromTriangle(vertices, t));
    if(triangles.size() < MIN_KD_TRIANGLES || depth >= MAX_KD_DEPTH) { // node is small or deep enough to stop dividing
        node->left = new KDNode();
        node->right = new KDNode();
        node->left->triangles = vector<Triangle>();
        node->right->triangles = vector<Triangle>();
        return node;
    }
    Vector3f center;

    for(Triangle t : triangles)
        center += (KDNode::calculateCenterOfTriangle(vertices, t) / triangles.size());

    vector<Triangle> left_triangles = vector<Triangle>();
    vector<Triangle> right_triangles = vector<Triangle>();
    int axis = node->box.findLongestAxis();
    for(Triangle t : triangles) {
        Vector3f centerOfTriangle = KDNode::calculateCenterOfTriangle(vertices, t);
        if(center[axis] >= centerOfTriangle[axis]) {
            right_triangles.push_back(t);
        } else {
            left_triangles.push_back(t);
        }
    }
    node->left = buildTree(vertices, left_triangles, depth + 1);
    node->right = buildTree(vertices, right_triangles, depth + 1);
    return node;
}

Vector3f KDNode::calculateCenterOfTriangle(vector<vertex> vertices, Triangle t) {
    return (vertices[t.v[0]].p + vertices[t.v[1]].p + vertices[t.v[2]].p) / 3.f;
}

AABoundingBox KDNode::createBoundingBoxFromTriangle(vector<vertex> vertices, Triangle t) {
    AABoundingBox box;
    box.extend(vertices[t.v[0]].p);
    box.extend(vertices[t.v[1]].p);
    box.extend(vertices[t.v[2]].p);

    box.color = Vector3f(.082, .996, .776);

    return box;
}

void KDNode::draw() {
    if (triangles.size() > 0) {
        box.draw();

        left->draw();
        right->draw();
    }
}