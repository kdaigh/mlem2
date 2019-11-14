// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Source file for executive class.
    @file executive.cpp 

    This class handles the execution
    of the program. */

#include "executive.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

using namespace std;

const size_t MAX_STREAM = numeric_limits<streamsize>::max();

Executive::Executive(){
    m_numAttributes = 0;
}

Executive::~Executive(){
    delete m_data;
}

bool Executive::parseInFile(string filename) {
    fstream file;
    file.open(filename);

    // Unable to open file; Signal with boolean
    if(!file) {
        return false;
    }

    parseFormat(file);
    m_data = new Dataset(m_numAttributes + 1);
    parseHeader(file);

    string word;
    int col = 0;
    while(file >> word){
        if(col == m_numCols){
            col = 0;
        }
        word = removeComments(word);
        if(!word.empty()){
            m_data->addValue(col, word);
        }
        col++;
    }

    // TEST: Check output of dataset
    m_data->print();

    file.close();
    return true;
}

void Executive::generateBlocks(){
    // LOOP: For each attribute (column)
    for(int col = 0; col < m_numAttributes; col++){
        string attr = m_data->getAttribute(col);
        
        // IF: Attribute values are numeric
        if(m_data->getValue(1, col)->isNumeric()){
            float min = 0, max = 0;
            list<float> cutpoints = m_data->discretize(col, min, max);

            // LOOP: For each cutpoint, create (empty) attribute-value blocks 
            for (float c : cutpoints){
                m_avBlocks.push_back(new AVNumeric(attr, col, min, c));
                m_avBlocks.push_back(new AVNumeric(attr, col, c, max));
            }
        }
        // IF: Attribute values are symbolic
        else {
            list<string> values = m_data->getPossibleValues(col);

            // LOOP: For each value, create an (empty) attribute-value block
            for (string v : values){
                m_avBlocks.push_back(new AVSymbolic(attr, col, v));
            }
        }
    }

    // LOOP: For each attribute-value block
    for(int i = 0; i < m_avBlocks.size(); i++){

        // LOOP: For each case (row), add matching values to the block
        for(int r = 1; r <= m_data->getNumCases(); r++){
            int c = m_avBlocks[i]->getAttrCol();
            
            m_avBlocks[i]->addOnMatch(m_data->getValue(r, c), r);    
        }

        // TEST: Check output of blocks
        m_avBlocks[i]->print();
    }
}

void Executive::runMLEM2(){
    // FUNC: Generate attribute-value blocks
    generateBlocks();

    // MLEM2 * mlem2 = new MLEM2(m_avBlocks);

    // // TODO: Generate concepts
    // // TEST: Test concept for yes
    // set<int> conceptYes;
    // conceptYes.emplace(1);
    // conceptYes.emplace(2);
    // conceptYes.emplace(3);

    // mlem2->induceRules(conceptYes);
}

// set<set<int>> MLEM2(vector<Block *> AV, set<int> B){
//     set<int> G = B;
//     set<set<int>> localCovering;
//     while(!G.empty()){
//         set<int> T;
//         set<int> T_G;
//     }
// }

bool Executive::generateOutFile(string filename){
    ofstream file;
    file.open(filename);

    // Unable to open file; Signal with boolean
    if(!file) {
        return false;
    }

    /*  TODO: Write to file
        EX: file << "Writing this to a file.\n"; */

    file.close();
    return true;
}

string Executive::removeComments(string str){
    size_t pos = str.find('!');
    if(pos != string::npos){
        str.erase(pos);
    }
    return str;
}

void Executive::parseFormat(istream& file){
    if(file.eof()){
        cerr << "No format found." << endl;
        return;
    }

    string str;
    getline(file, str);
    str = removeComments(str);
    size_t beg = str.find('<');
    size_t end = str.find('>');
    if(beg != string::npos && end != string::npos){
        str = str.substr(beg + 1, end);
    }
    for(char& c : str) {
        if(c == 'a'){
            m_numAttributes++;
        }
    }
    m_numCols = m_numAttributes + 1;
}

void Executive::parseHeader(istream& file){
    if(file.eof()){
        cerr << "No header found." << endl;
        return;
    }

    string str, word;
    file.ignore(MAX_STREAM, '[');
    file.ignore(MAX_STREAM, ' ');
    getline(file, str, ']');
    str = removeComments(str);

    istringstream buffer(str);
    for(int i = 0; i < m_numAttributes; i++){
        buffer >> word;
        m_data->addAttribute(word);      
    }
    buffer >> word;
    m_data->setDecision(word);
}

