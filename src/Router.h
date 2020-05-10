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

class TAP : public gridPoint {
public:
    int id;
    TAP() {}
    TAP(int _id, int _x, int _y) : id(_id) {
        x = _x;
        y = _y;
    }

    string str_xy() {
        stringstream s;
        s << "[" << x << "," << y << "]";
        return s.str();
    }
    friend inline std::ostream& operator<<(std::ostream& os, const TAP& tap) {
        os << "TAP " << tap.id << ", x: " << tap.x << ", y: " << tap.y;
        return os;
    }
};
class segment {
public:
    gridPoint p1, p2;
    gridPoint center;
    shared_ptr<segment> ch[2];
    shared_ptr<segment> par;
    int id;  // unique id for each segment

    segment(gridPoint u, gridPoint v) : p1(u), p2(v) {}
    bool isLeaf() { return p1 == p2; }
    bool operator==(const segment& rhs) const { return (p1 == rhs.p1 && p2 == rhs.p2 && id == rhs.id); }
};

class TreeNode {
public:
    int id;
    shared_ptr<TreeNode> lc;
    shared_ptr<TreeNode> rc;
    TreeNode(int _id){
        id = _id;
        lc=NULL;
        rc=NULL;
    }
    void set_lc(shared_ptr<TreeNode> child){lc = child;}
    void set_rc(shared_ptr<TreeNode> child){rc = child;}

};
class TreeTopology {
public:
    shared_ptr<TreeNode> root;
    shared_ptr<TreeNode> tmp_root; // used for refinement evaluation
    int leafNumber;
    int size;
    alglib::integer_2d_array& HC_result;

    TreeTopology(alglib::integer_2d_array& HC_res):HC_result(HC_res){}
    void init(int leafNum,int sz);
    // construct binary tree structure based on current Hierarchical clustering result
    void constructTree(bool modifyCurrentTree=false);
    int getSumOfDiameter();
    // randomly switch leaf nodes to reduce sum of diameter
    void refineStructure(int iter = 10000); 
};

class Router {
public:
    int MAX_RUNTIME = 3600;  // test 1
    int NUM_TAPS = 4;        // test 1
    vector<TAP> taps;
    unordered_map<int, segment> segmentsMap;  // set of segments
    shared_ptr<TreeTopology> topo;
    // some structure to store the final routing result

    void init();
    void readInput();
    // To generate topology(try L1 based metric and L2 based ones)
    void NS();          // nearest neighbor topology
    void HC();          // Agglomerative  Hierarchical Clustering
    void CL();          // clustering based algorithm
    void Refinement();  // clustering with refinement 4<= m <=6 in the original paper
    void RGM();         // Recursive Geometric Matching

    // Generate embedding
    void DME();  // Deferred-Merge Embedding

    void route();

    void writeSolution();
};