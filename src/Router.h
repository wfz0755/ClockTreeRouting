#pragma once
#include "global.h"
using namespace std;

const double eps = 1e-4;

class GridPoint {
public:
    double x, y;

    GridPoint() {}
    GridPoint(double _x, double _y) : x(_x), y(_y) {}

    bool operator==(const GridPoint& rhs) const { return (x == rhs.x && y == rhs.y); }
    friend inline std::ostream& operator<<(std::ostream& os, const GridPoint& gp) {
        os << "(" << gp.x << "," << gp.y << ")";
        return os;
    }
};

class TAP : public GridPoint {
public:
    int id;
    TAP() {}
    TAP(int _id, double _x, double _y) : id(_id) {
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
class Segment {
public:
    GridPoint p1, p2;  // p1 has lower y
    // GridPoint center;
    shared_ptr<Segment> ch[2];
    shared_ptr<Segment> par;
    double delay;
    int id = 0;  // unique id for each segment
    Segment() {}
    Segment(GridPoint u, GridPoint v) : p1(u), p2(v) {
        delay = 0;
        if (p1.y > p2.y) {
            swap(p1, p2);
        }
    }
    bool isLeaf() { return p1 == p2; }
    bool operator==(const Segment& rhs) const { return (p1 == rhs.p1 && p2 == rhs.p2); }

    friend inline std::ostream& operator<<(std::ostream& os, const Segment& seg) {
        os << "Seg: (" << seg.p1 << "," << seg.p2 << ")";
        return os;
    }

    double slope() {
        if (isLeaf()) {
            return 0;
        }
        if (p2.x == p1.x) {
            return 0;
        }
        return 1.0 * (p1.y - p2.y) / (p1.x - p2.x);
    }

    Segment intersect(Segment& rhs) {
        double cur_slope = slope();
        double rhs_slope = rhs.slope();

        // check if 4 points same line
        // if (abs(cur_slope - rhs_slope) < eps) {
        //     if (abs((rhs.p1.y - p1.y) * (p2.x - p1.x) - (p2.y - p1.y) * (rhs.p1.x - p1.x)) < eps) {
        if (rhs.isLeaf()) {  // if current segment is intersecting a single grid point
            Segment ret = rhs;
            if(abs((rhs.p1.y - p1.y) * (p2.x - p1.x) - (p2.y - p1.y) * (rhs.p1.x - p1.x)) < eps){
            // if ((rhs.p1.y - p1.y) * (p2.x - p1.x) == (p2.y - p1.y) * (rhs.p1.x - p1.x)) {
                if (p1.y-eps <= rhs.p1.y && rhs.p1.y <= p2.y+eps) {  // valid intersection
                    Segment ret = rhs;
                    ret.id = -2;  // return single point intersection
                    return ret;
                }
            }
            ret.id = -1;
            return ret;
        }
        if (abs(cur_slope - rhs_slope) < eps) {
            if (abs((rhs.p1.y - p1.y) * (p2.x - p1.x) - (p2.y - p1.y) * (rhs.p1.x - p1.x)) < eps) {
                assert(rhs.p1.y <= rhs.p2.y && p1.y <= p2.y);
                GridPoint upper, lower;
                if (rhs.p2.y < p2.y) {
                    upper = rhs.p2;
                } else {
                    upper = p2;
                }
                if (rhs.p1.y > p1.y) {
                    lower = rhs.p1;
                } else {
                    lower = p1;
                }
                if (upper.y < lower.y) {
                    cout << "No overlap between two segs on the line" << endl;
                    exit(1);
                }
                return Segment(lower, upper);
            } else {
                Segment ret;
                ret.id = -1;
                return ret;
            }
        } else {
            // might be 1 point or 0
            double A1 = p2.y - p1.y;
            // double B1 = p2.x - p1.x;
            double B1 = p1.x - p2.x;
            double C1 = A1 * p1.x + B1 * p1.y;
            double A2 = rhs.p2.y - rhs.p1.y;
            // double B2 = rhs.p2.x - rhs.p1.x;
            double B2 = rhs.p1.x - rhs.p2.x;
            double C2 = A2 * rhs.p1.x + B2 * rhs.p1.y;
            double det = A1 * B2 - A2 * B1;
            double x = (B2 * C1 - B1 * C2) / det;
            double y = (A1 * C2 - A2 * C1) / det;

            Segment ret;
            // if (p1.y-eps <= y && y <= p2.y +eps) {  // valid intersection
            // if(abs(x-p1.x) <= 0.5 && abs(y-p1.y) <= 0.5){
            //     ret.p1 = p1;
            //     ret.p2 = p1;
            //     ret.id = -3;
            // }else if(abs(x-p2.x) <= 0.5 && abs(y-p2.y) <= 0.5){

            // }
            // else if(abs(x-rhs.p1.x) <= 0.5 && abs(y-rhs.p1.y) <= 0.5){

            // }else if(abs(x-rhs.p2.x) <= 0.5 && abs(y-rhs.p2.y) <= 0.5){

            // }
            if (p1.y-eps <= y && y <= p2.y+eps && rhs.p1.y-eps <= y && y <= rhs.p2.y+eps) {  // valid intersection

                ret.p1 = GridPoint(x, y);
                ret.p2 = GridPoint(x, y);
                ret.id = -2;  // return single point intersection
            } else {
                ret.id = -1;
            }
            return ret;
        }
        // if return with id=-1, means no intersection
    }
};
class TRR {
public:
    Segment core;
    double radius;
    TRR() {}
    TRR(Segment seg, double radi) : core(seg), radius(radi) {}
    friend inline std::ostream& operator<<(std::ostream& os, const TRR& trr) {
        os << trr.core << "; radius:" << trr.radius;
        return os;
    }
    Segment intersect(Segment& seg) {
        vector<GridPoint> trr_boundary_grid;
        vector<Segment> trr_Sides;
        trr_boundary_grid.emplace_back(core.p1.x, core.p1.y - radius);
        trr_boundary_grid.emplace_back(core.p1.x + radius, core.p1.y);
        trr_boundary_grid.emplace_back(core.p1.x, core.p1.y + radius);
        trr_boundary_grid.emplace_back(core.p1.x - radius, core.p1.y);  // clock-wise
        for (int i = 0; i < 3; i++) {
            trr_Sides.emplace_back(trr_boundary_grid[i], trr_boundary_grid[i + 1]);
        }
        trr_Sides.emplace_back(trr_boundary_grid[3], trr_boundary_grid[0]);

        cout << "Core is " << core << endl;
        cout << "Print trr's sides" << endl;
        for (auto& seg1 : trr_Sides) {
            cout << seg1 << endl;
        }
        cout << "MS:" << seg << endl;

        for (auto& side : trr_Sides) {
            Segment intersection = side.intersect(seg);
            if (intersection.id != -1) {
                return intersection;
            }
        }
        Segment ret;
        ret.id = -1;
        return ret;
    }
};

class TreeNode {
public:
    int id;
    shared_ptr<TreeNode> lc;
    shared_ptr<TreeNode> rc;
    shared_ptr<TreeNode> par;
    TreeNode(int _id) {
        id = _id;
        lc = NULL;
        rc = NULL;
        par = NULL;
    }
    void set_lc(shared_ptr<TreeNode> child) { lc = child; }
    void set_rc(shared_ptr<TreeNode> child) { rc = child; }
    void set_par(shared_ptr<TreeNode> p) { par = p; }
};
class TreeTopology {
public:
    shared_ptr<TreeNode> root;
    shared_ptr<TreeNode> tmp_root;  // used for refinement evaluation
    int leafNumber;
    int size;
    alglib::integer_2d_array& HC_result;
    unordered_map<int, shared_ptr<TreeNode>> id_treeNode;

    TreeTopology(alglib::integer_2d_array& HC_res) : HC_result(HC_res) {}
    void init(int leafNum, int sz);
    // construct binary tree structure based on current Hierarchical clustering result
    void constructTree(bool modifyCurrentTree = false);
    int getSumOfDiameter();
    // randomly switch leaf nodes to reduce sum of diameter
    void refineStructure(int iter = 10000);
};

class Router {
public:
    int MAX_RUNTIME = 3600;  // test 1
    int NUM_TAPS = 4;        // test 1
    vector<TAP> taps;
    vector<Segment> vertexMS;  // set of segments
    vector<TRR> vertexTRR;
    vector<double> vertexDistE;
    shared_ptr<TreeTopology> topo;

    // Structures to store the final routing result
    GridPoint clockSource;
    vector<GridPoint> pl;
    vector<vector<GridPoint>> sol;

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