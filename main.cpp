#include <iostream>
#include "Router.h"
using namespace std;


int main(int argc, char *argv[]){
    cout << "Handling Task One: CLOCK SRC-TAPS ROUTING " << endl;
    cout << "Input file name" << argv[1] << endl;
    cout << "Output file name" << argv[2] << endl;
    
    string input_file_name(argv[1]);
    string output_file_name(argv[2]);
    
    Router router(input_file_name,output_file_name);
    router.init();
    router.route();
    router.writeSolution();
    
    cout << "End of Process" << endl;
    return 0;
}