// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

#include "executive.hpp"
#include <iostream>
#include <stdexcept>
#include <string>

/* Utility function */
string getFile(string type);

int main(int argc, char* argv[]){
    bool validFile = false;
    string inFile, outFile;
    Executive * exec = new Executive();

    // Request input file from user
    while(!validFile){
        inFile = getFile("input");
        validFile = exec->parseInFile(inFile);
    }

    // Run MLEM2 rule induction algorihtm on dataset
    exec->runMLEM2();

    // TODO: call exec->generateOutputFile(outputFilename)
    validFile = false;
    while(!validFile){
        inFile = getFile("output");
        cerr << "TODO: generate output file" << endl;
    }

    return 0;
}

string getFile(string type){
    cout << "Please provide an " << type << " file (i.e. test.txt):" << endl;
    string file;
    cin >> file;
    while(cin.fail()){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout << "\nPlease provide an " << type << " file (i.e. test.txt):" << endl;
        cin >> file;
    }
    return file;
}

