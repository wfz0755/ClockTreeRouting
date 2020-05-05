#pragma once
#include "global.h"
using namespace std;

class gridPoint {
public:
    int x, y;

    gridPoint() {}
    gridPoint(int _x, int _y) : x(_x), y(_y) {}

    bool operator==(const gridPoint& rhs) const { return (x == rhs.x && y == rhs.y); }
};
class segment {
public:
    gridPoint p1, p2;
    gridPoint center;
    shared_ptr<segment> ch[2];
    shared_ptr<segment> par;
    int id;

    segment(gridPoint u, gridPoint v) : p1(u), p2(v) {}
    bool isLeaf() { return p1 == p2; }
    bool operator==(const segment& rhs) const { return (p1 == rhs.p1 && p2 == rhs.y && vid == rhs.vid); }
};

class Router {
public:
    unordered_map<int,segment> segmentsMap; // set of segments

    // some structure to store the final routing result


    void init();
    void readInput();
    // To generate topology(try L1 based metric and L2 based ones)
    void NS();    // nearest neighbor topology
    void CL();    // clustering based algorithm
    void Refinement();  // clustering with refinement 4<= m <=6 in the original paper
    void RGM();   // Recursive Geometric Matching

    // Generate embedding
    void DME();  // Deferred-Merge Embedding

    void route();

    void writeSolution();
};