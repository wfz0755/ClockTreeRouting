#include "Router.h"
#include "Setting.h"

using std::cout;
using std::endl;
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
        }
        else if (iss.str().find("TAPS") != std::string::npos) {
            NUM_TAPS = stoi(line.substr(line.find(" ") + 1));
            cout << "NUM_TAPS: " << NUM_TAPS << endl;
        }
        else if(iss.str().find("TAP") != std::string::npos){
            char buf[10];
            int tap_id,x,y;
            iss >> buf >> tap_id >> x >> y;
            taps.emplace_back(tap_id,x,y);
        }
    }
    for(auto & tap: taps){
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
void Router::HC(){



}

// Deferred-Merge Embedding
void Router::DME() {}

void Router::route() {
    HC(); // try hierarchical clustering
    DME();
    cout << "Finish Routing" << endl;
}

void Router::writeSolution() {


    cout << "Finish Write Result" << endl;
}