#include "Router.h"
#include "Setting.h"

using namespace std;

void Router::init() {
    // read input and build data structure
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
        if (iss.str().find("TAPS") != std::string::npos) {
            NUM_TAPS = stoi(line.substr(line.find(" ") + 1));
            cout << "NUM_TAPS: " << NUM_TAPS << endl;
        }
        else if(iss.str().find("TAP") != std::string::npos){
            int tap_id = stoi(line.substr(line.find(" ") + 1));
            int x = stoi(line.substr(line.find(" ") + 2));
            int y = stoi(line.substr(line.find(" ") + 3));
            taps.emplace_back(tap_id,x,y);
        }
        if (iss.str().find("END TAPS") != std::string::npos) break;
    }
    for(auto & tap: taps){
        cout << tap << endl;
    }
    cout << "Finish Reading input" << endl;
}
void Router::readInput() {}

// Recursive Geometric Matching
void Router::RGM() {
    // TODO
}

// nearest neighbor topology
void Router::NS() {}

// Deferred-Merge Embedding
void Router::DME() {}

void Router::route() {}

void Router::writeSolution() {}