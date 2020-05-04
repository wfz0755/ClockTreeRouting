#pragma once
#include "global.h"
using namespace std;
class Router {
public:
    string input_file_name;
    string output_file_name;

    Router(string in_name, string out_name) : input_file_name(in_name), output_file_name(out_name) {}

    void init();
    void readInput();
    void RGM();  // Recursive Geometric Matching
    void DME();  // Deferred-Merge Embedding

    void route();

    void writeSolution();
};