#include "Router.h"
#include "Setting.h"

using std::cout;
using std::endl;

#define COMPLETE_LINKAGE 0
#define SINGLE_LINKAGE 1
#define L1 1
#define L2 2
#define eps 1e-6
const string padding(30, '=');

void TreeTopology::init(int leafNum, int sz) {
    leafNumber = leafNum;
    size = sz;
    cout << "Initialize topo: " << leafNum << " leaves and " << sz << " nodes in total" << endl;
    constructTree(true);
}
void TreeTopology::constructTree(bool modifyCurrentTree) {
    vector<vector<int>> DAG(size);
    // cout << "rows " << HC_result.rows() << endl;
    int n_merges = HC_result.rows();
    int cur_internal_node_idx = leafNumber;
    for (int i = 0; i < n_merges; i++) {
        int p1 = HC_result[i][0];
        int p2 = HC_result[i][1];
        DAG[cur_internal_node_idx].push_back(p1);
        DAG[cur_internal_node_idx].push_back(p2);
        cur_internal_node_idx++;
        // cout << "Merge " << p1 << " and " << p2 << endl;
    }
    // Construct tree
    tmp_root = make_shared<TreeNode>(size - 1);  // last merge point as root
    std::function<void(shared_ptr<TreeNode>)> buildTree = [&](shared_ptr<TreeNode> curNode) {
        int curId = curNode->id;
        id_treeNode[curId] = curNode;
        if (DAG[curId].size() != 0) {
            auto lc = make_shared<TreeNode>(DAG[curId][0]);
            auto rc = make_shared<TreeNode>(DAG[curId][1]);
            curNode->set_lc(lc);
            curNode->set_rc(rc);
            lc->set_par(curNode);
            rc->set_par(curNode);
            buildTree(curNode->lc);
            buildTree(curNode->rc);
            return;
        }
    };
    buildTree(tmp_root);

    std::function<void(shared_ptr<TreeNode>)> postOrderTraversal = [&](shared_ptr<TreeNode> curNode) {
        int curId = curNode->id;
        if (curNode->lc != NULL && curNode->rc != NULL) {
            postOrderTraversal(curNode->lc);
            postOrderTraversal(curNode->rc);
            cout << "Vis: " << curId << endl;
            return;
        } else {
            cout << "Vis: " << curId << endl;
        }
    };
    // postOrderTraversal(tmp_root);

    if (modifyCurrentTree == true) {
        root = tmp_root;
    }
}

void Router::init() {
    // read input and build data structure
    readInput();
}
void Router::readInput() {
    ifstream fin(setting.input_file_name);
    if (fin.fail()) {
        cout << "Fail to open file:" << setting.input_file_name << endl;
        exit(1);
    } else {
        cout << padding << "Successfully open input:" << setting.input_file_name << padding << endl;
    }
    string line;

    while (getline(fin, line)) {
        istringstream iss(line);
        if (iss.str().find("MAX_RUNTIME") != std::string::npos) {
            MAX_RUNTIME = stoi(line.substr(line.find(" ") + 1));
            cout << "MAX_RUNTIME: " << MAX_RUNTIME << endl;
        }

        if (iss.str().find("END TAPS") != std::string::npos) {
            break;
        } else if (iss.str().find("TAPS") != std::string::npos) {
            NUM_TAPS = stoi(line.substr(line.find(" ") + 1));
            cout << "NUM_TAPS: " << NUM_TAPS << endl;
        } else if (iss.str().find("TAP") != std::string::npos) {
            char buf[10];
            int tap_id;
            double x, y;
            iss >> buf >> tap_id >> x >> y;
            taps.emplace_back(tap_id, x, y);
        }
    }
    for (auto& tap : taps) {
        cout << tap << endl;
    }
    cout << padding << "Finish Reading input" << padding << endl;
}

// Recursive Geometric Matching
void Router::RGM() {
    // TODO
}

// nearest neighbor topology
void Router::NS() {
    // TODO
}

// Agglomerative  Hierarchical Clustering
void Router::HC() {
    // 1. Create XY array
    // vector<vector<double>> points;
    string points_str;
    points_str += "[";
    for (int i = 0; i < taps.size(); i++) {
        points_str += taps[i].str_xy();
        if (i != taps.size() - 1) {
            points_str += ",";
        }
    }
    points_str += "]";
    // cout <<"Points: " << points_str << endl;

    // 2. Create clusterizer
    using namespace alglib;
    clusterizerstate s;
    ahcreport rep;
    real_2d_array xy(points_str.c_str());
    // int linkage_type = SINGLE_LINKAGE;
    int linkage_type = COMPLETE_LINKAGE;
    int metric = L1;

    clusterizercreate(s);
    clusterizersetpoints(s, xy, metric);  // manhattan dist
    clusterizersetahcalgo(s, linkage_type);
    clusterizerrunahc(s, rep);
    printf("%s\n", rep.z.tostring().c_str());

    // 3. Construct binary tree topology
    topo = make_shared<TreeTopology>(rep.z);
    topo->init(xy.rows(), 2 * xy.rows() - 1);

    cout << padding << "Finish Topology generation" << padding << endl;
}

inline double L1Dist(GridPoint p1, GridPoint p2) { return abs(p1.x - p2.x) + abs(p1.y - p2.y); }

Segment TRRintersect(TRR& trr1, TRR& trr2) {
    // get four edges
    cout << "Merging: " << trr1 << " and " << trr2 << endl;
    vector<GridPoint> trr1_boundary_grid;
    vector<GridPoint> trr2_boundary_grid;
    vector<Segment> trr1_Sides;
    vector<Segment> trr2_Sides;
    if (trr1.core.slope() > 0) {
        trr1_boundary_grid.emplace_back(trr1.core.p1.x, trr1.core.p1.y - trr1.radius);
        trr1_boundary_grid.emplace_back(trr1.core.p2.x + trr1.radius, trr1.core.p2.y);
        trr1_boundary_grid.emplace_back(trr1.core.p2.x, trr1.core.p2.y + trr1.radius);
        trr1_boundary_grid.emplace_back(trr1.core.p1.x - trr1.radius, trr1.core.p1.y);  // clock-wise
    } else if (trr1.core.slope() < 0) {
        trr1_boundary_grid.emplace_back(trr1.core.p1.x + trr1.radius, trr1.core.p1.y);
        trr1_boundary_grid.emplace_back(trr1.core.p2.x, trr1.core.p2.y + trr1.radius);
        trr1_boundary_grid.emplace_back(trr1.core.p2.x - trr1.radius, trr1.core.p2.y);
        trr1_boundary_grid.emplace_back(trr1.core.p1.x, trr1.core.p1.y - trr1.radius);  // clock-wise
    } else {                                                                            // leaf node
        trr1_boundary_grid.emplace_back(trr1.core.p1.x, trr1.core.p1.y - trr1.radius);
        trr1_boundary_grid.emplace_back(trr1.core.p1.x + trr1.radius, trr1.core.p1.y);
        trr1_boundary_grid.emplace_back(trr1.core.p1.x, trr1.core.p1.y + trr1.radius);
        trr1_boundary_grid.emplace_back(trr1.core.p1.x - trr1.radius, trr1.core.p1.y);  // clock-wise
    }

    if (trr2.core.slope() > 0) {
        trr2_boundary_grid.emplace_back(trr2.core.p1.x, trr2.core.p1.y - trr2.radius);
        trr2_boundary_grid.emplace_back(trr2.core.p2.x + trr2.radius, trr2.core.p2.y);
        trr2_boundary_grid.emplace_back(trr2.core.p2.x, trr2.core.p2.y + trr2.radius);
        trr2_boundary_grid.emplace_back(trr2.core.p1.x - trr2.radius, trr2.core.p1.y);  // clock-wise
    } else if (trr2.core.slope() < 0) {
        trr2_boundary_grid.emplace_back(trr2.core.p1.x + trr2.radius, trr2.core.p1.y);
        trr2_boundary_grid.emplace_back(trr2.core.p2.x, trr2.core.p2.y + trr2.radius);
        trr2_boundary_grid.emplace_back(trr2.core.p2.x - trr2.radius, trr2.core.p2.y);
        trr2_boundary_grid.emplace_back(trr2.core.p1.x, trr2.core.p1.y - trr2.radius);  // clock-wise
    } else {                                                                            // leaf node
        trr2_boundary_grid.emplace_back(trr2.core.p1.x, trr2.core.p1.y - trr2.radius);
        trr2_boundary_grid.emplace_back(trr2.core.p1.x + trr2.radius, trr2.core.p1.y);
        trr2_boundary_grid.emplace_back(trr2.core.p1.x, trr2.core.p1.y + trr2.radius);
        trr2_boundary_grid.emplace_back(trr2.core.p1.x - trr2.radius, trr2.core.p1.y);  // clock-wise
    }

    for (int i = 0; i < 3; i++) {
        trr1_Sides.emplace_back(trr1_boundary_grid[i], trr1_boundary_grid[i + 1]);
        trr2_Sides.emplace_back(trr2_boundary_grid[i], trr2_boundary_grid[i + 1]);
    }
    trr1_Sides.emplace_back(trr1_boundary_grid[3], trr1_boundary_grid[0]);
    trr2_Sides.emplace_back(trr2_boundary_grid[3], trr2_boundary_grid[0]);

    cout << "Print trr1's sides" << endl;
    for (auto& seg1 : trr1_Sides) {
        cout << seg1 << endl;
    }

    cout << "Print trr2's sides" << endl;

    for (auto& seg2 : trr2_Sides) {
        cout << seg2 << endl;
    }

    // for 4*4 check intersect
    for (auto& seg1 : trr1_Sides) {
        for (auto& seg2 : trr2_Sides) {
            Segment seg = seg1.intersect(seg2);
            if (seg.id == 0) {
                return seg;
            }
        }
    }
    cout << "Cannot find intersection between two TRRs" << endl;
    Segment ret;
    ret.id = -1;
    return ret;
}
// Deferred-Merge Embedding
void Router::DME() {
    vertexMS.resize(topo->size);
    vertexTRR.resize(topo->size);
    vertexDistE.resize(topo->size);

    // 1. Build Tree of Segments (bottom up)
    std::function<void(shared_ptr<TreeNode>)> postOrderTraversal = [&](shared_ptr<TreeNode> curNode) {
        int curId = curNode->id;
        if (curNode->lc != NULL && curNode->rc != NULL) {
            postOrderTraversal(curNode->lc);
            postOrderTraversal(curNode->rc);

            // create merging segment for curNode
            auto& ms_a = vertexMS[curNode->lc->id];
            auto& ms_b = vertexMS[curNode->rc->id];
            // get |e_a|, |e_b|
            double d = min(L1Dist(ms_a.p1, ms_b.p1), L1Dist(ms_a.p1, ms_b.p2));
            d = min(d, L1Dist(ms_a.p2, ms_b.p1));
            d = min(d, L1Dist(ms_a.p2, ms_b.p2));
            double e_a_dist = (ms_b.delay - ms_a.delay + d) / 2;
            double e_b_dist = (ms_a.delay - ms_b.delay + d) / 2;
            if (e_a_dist < 0 || e_b_dist < 0) {
                cout << "Skew too large" << endl;
                exit(1);
            }
            vertexDistE[curNode->lc->id] = e_a_dist;
            vertexDistE[curNode->rc->id] = e_b_dist;

            // get trr_a, trr_b
            TRR trr_a(ms_a, e_a_dist);
            TRR trr_b(ms_b, e_b_dist);
            vertexTRR[curNode->lc->id] = trr_a;
            vertexTRR[curNode->rc->id] = trr_b;
            // intersect trr_a, trr_b to get ms_v
            Segment ms_v = TRRintersect(trr_a, trr_b);
            cout << "Merging result: " << ms_v << endl;
            if (ms_v.id == -1) {
                cout << "Merge failure" << endl;
                exit(1);
            }
            ms_v.delay = e_a_dist + ms_a.delay;
            vertexMS[curId] = ms_v;
            // cout << "Delay diff " << e_a_dist + ms_a.delay - (e_b_dist + ms_b.delay) << endl;
        } else {
            // Create ms for leaf node
            vertexMS[curId] = Segment(taps[curId], taps[curId]);
        }
    };
    postOrderTraversal(topo->root);
    cout << padding << "Finish bottom-up process" << padding << endl;

    // 2. Find Exact Placement(top down)

    pl.resize(topo->size);
    sol.resize(topo->leafNumber);
    auto& rootMS = vertexMS[topo->root->id];

    std::function<void(shared_ptr<TreeNode>)> preOrderTraversal = [&](shared_ptr<TreeNode> curNode) {
        int curId = curNode->id;

        if (curNode->lc != NULL && curNode->rc != NULL) {
            // handle curNode
            if (curNode == topo->root) {
                clockSource = rootMS.p1;
                pl[curId] = rootMS.p1;
            } else {
                auto& par = curNode->par;
                int parId = par->id;
                auto& trr_par = vertexTRR[parId];
                trr_par.core = Segment(pl[parId], pl[parId]);
                trr_par.radius = vertexDistE[curId];
                Segment merged = trr_par.intersect(vertexMS[curId]);
                if (merged.id == -1) {
                    cout << "TRR-MS merging failed" << endl;
                    exit(1);
                }
                pl[curId] = merged.p1;
            }

            cout << "Steiner Point " << curId << " located at " << pl[curId] << endl;
            preOrderTraversal(curNode->lc);
            preOrderTraversal(curNode->rc);
        } else {
            // sinks
            pl[curId] = vertexMS[curId].p1;
            return;
        }
    };
    preOrderTraversal(topo->root);
    cout << padding << "Finish top-down process" << padding << endl;

    cout << padding << "Finished DME" << padding << endl;
    cout << padding << "Total Wire Length: " << padding << endl;
}

void Router::route() {
    HC();  // try hierarchical clustering
    DME();
    cout << padding << "Finish Routing" << padding << endl;
}

void Router::writeSolution() { cout << padding << "Finish Write Result" << padding << endl; }
