#include "Router.h"
#include "Setting.h"

using std::cout;
using std::endl;

#define COMPLETE_LINKAGE 0
#define SINGLE_LINKAGE 1
#define L1 1
#define L2 2

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
        if (DAG[curId].size() != 0) {
            auto lc = make_shared<TreeNode>(DAG[curId][0]);
            auto rc = make_shared<TreeNode>(DAG[curId][1]);
            curNode->set_lc(lc);
            curNode->set_rc(rc);
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
        }else{
            cout << "Vis: " << curId << endl;
        }
    };
    // postOrderTraversal(tmp_root);

    if(modifyCurrentTree==true){
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
        cout << "Successfully open input:" << setting.input_file_name << endl;
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
            int tap_id, x, y;
            iss >> buf >> tap_id >> x >> y;
            taps.emplace_back(tap_id, x, y);
        }
    }
    for (auto& tap : taps) {
        cout << tap << endl;
    }
    cout << "Finish Reading input" << endl;
}

// Recursive Geometric Matching
void Router::RGM() {
    // TODO
}

// nearest neighbor topology
void Router::NS() {}

// Agglomerative  Hierarchical Clustering
void Router::HC() {
    // 1. Create XY array
    vector<vector<int>> points;
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
    int linkage_type = SINGLE_LINKAGE;
    int metric = L1;

    clusterizercreate(s);
    clusterizersetpoints(s, xy, metric);  // manhattan dist
    clusterizersetahcalgo(s, linkage_type);
    clusterizerrunahc(s, rep);
    printf("%s\n", rep.z.tostring().c_str());

    // 3. Construct binary tree topology
    topo = make_shared<TreeTopology>(rep.z);
    topo->init(xy.rows(), 2 * xy.rows() - 1);
}

// Deferred-Merge Embedding
void Router::DME() {}

void Router::route() {
    HC();  // try hierarchical clustering
    DME();
    cout << "Finish Routing" << endl;
}

void Router::writeSolution() { cout << "Finish Write Result" << endl; }
