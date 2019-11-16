// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

#include "executive.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include <limits>

using namespace std;

/* Utility function */
string getFile(string type);

int main(int argc, char* argv[]){
    bool validFile = false;
    string inFile, outFile;
    Executive exec;

    // Request input file from user
    while(!validFile){
        inFile = getFile("input");
        validFile = exec.parseInFile(inFile);
    }

    // Run rule induction algorithm on dataset and print to file
    validFile = false;
    while(!validFile){
        outFile = getFile("output");
        validFile = exec.generateOutFile(outFile);
    }
    return 0;
}

string getFile(string type){
    cout << "Please provide an " << type << " file (i.e. " << type << "_test.txt):" << endl;
    string file;
    cin >> file;
    while(cin.fail()){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout << "\nPlease provide an " << type << " file (i.e. " << type << "_test.txt):" << endl;
        cin >> file;
    }
    return file;
}

