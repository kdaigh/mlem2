// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Source file for executive class.
    @file executive.cpp 

    This class handles the execution
    of the program. */

#include "algorithm.hpp"
#include "executive.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

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
    unsigned col = 0;
    while(file >> word){
        // IF: col is at last column, reset col
        if(col == m_numAttributes + 1){
            col = 0;
        }
        size_t pos = word.find('!');
        if(pos != string::npos){
            if(pos != 0){
                word = word.erase(pos);
            } else {
                word = "";
            }         
            string comment;
            getline(file, comment, '\n');
        }
        if(!word.empty()){
            m_data->addValue(col, word);
            col++;
        }  
    }

    file.close();
    return true;
}

bool Executive::generateOutFile(string filename){
    ofstream file;
    file.open(filename);

    // Unable to open file; Signal with boolean
    if(!file) {
        return false;
    }

    Algorithm mlem2(m_numAttributes);
    file << mlem2.generateRuleset(m_data);

    file.close();
    return true;
}

void Executive::parseFormat(istream& file){
    if(file.eof()){
        cerr << "No format found." << endl;
        return;
    }

    string line = removeComments(file);
    size_t beg = line.find('<');
    size_t end = line.find('>');
    if(beg != string::npos && end != string::npos){
        line = line.substr(beg + 1, end);
    }
    for(char& c : line) {
        if(c == 'a'){
            m_numAttributes++;
        }
    }
}

void Executive::parseHeader(istream& file){
    if(file.eof()){
        cerr << "No header found." << endl;
        return;
    }

    string line = removeComments(file);
    size_t beg = line.find('[');
    size_t end = line.find(']');
    if(beg != string::npos && end != string::npos){
        line = line.substr(beg + 1, end);
    }

    string word;
    istringstream buffer(line);
    for(unsigned i = 0; i < m_numAttributes; i++){
        buffer >> word;
        m_data->addAttribute(word);      
    }
    buffer >> word;
    m_data->setDecision(word);
}

string Executive::removeComments(istream & file){
    string line;
    getline(file, line);
    size_t pos = line.find('!');
    // WHILE: Line contains comment
    while(pos != string::npos){
        // IF: Entire line is comment
        if(line.find('!') == 0){
            getline(file, line);
            pos = line.find('!');
        }
        // ELSE: Comment is at the end of line
        else {
            line.erase('!');
            pos = string::npos;
        }
    }
    return line;
}