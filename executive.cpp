// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Source file for executive class.
    @file executive.cpp 

    This class handles the execution
    of the program. */

#include "executive.hpp"
#include "setUtils.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <limits.h>
#include <algorithm>
#define DEBUG false

using namespace std;

const size_t MAX_STREAM = numeric_limits<streamsize>::max();

Executive::Executive(){
    m_numAttributes = 0;
}

Executive::~Executive(){
    delete m_data;
    for(int i = 0; i < m_avBlocks.size(); i++){
        delete m_avBlocks[i];
    }
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
        if(col == m_numAttributes + 1){
            col = 0;
        }
        word = removeComments(word);
        if(!word.empty()){
            m_data->addValue(col, word);
        }
        col++;
    }

    #if DEBUG == true
        m_data->print();
    #endif

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

    // Generate attribute-value blocks
    generateAVBlocks();
    
    // Generate concepts
    vector <Concept *> concepts = generateConcepts();

    // LOOP: For each concept
    for(Concept * c : concepts){
        vector<set<int>> ruleset = induceRules(m_avBlocks, c->block);
        // LOOP: For each rule
        for(set<int> r : ruleset){
            file << ruleString(r, c);
        }
        delete c;
    }

    file.close();
    return true;
}

void Executive::generateAVBlocks(){
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

    // Populate attribute-value blocks
    // LOOP: For each attribute-value block
    for(int i = 0; i < m_avBlocks.size(); i++){

        // LOOP: For each case (row), add matching values to the block
        for(int r = 1; r <= m_data->getNumCases(); r++){
            int c = m_avBlocks[i]->getAttrCol();
            
            m_avBlocks[i]->addOnMatch(m_data->getValue(r, c), r);    
        }

        #if DEBUG == true
           m_avBlocks[i]->print();
        #endif
    }
}

vector<Concept *> Executive::generateConcepts(){
    vector <Concept *> concepts;
    list<string> values = m_data->getPossibleValues(m_numAttributes);
    for(string v : values){
        concepts.push_back(new Concept(m_data->getDecision(), v));
    }

    // LOOP: For each concept
    for(int i = 0; i < concepts.size(); i++){

        // LOOP: For each case (row), add matching values to the block
        for(int r = 1; r <= m_data->getNumCases(); r++){
            int c = m_numAttributes;
            string cellValue = m_data->getValue(r, c)->getStrValue();
            if(concepts[i]->value == cellValue){
                concepts[i]->addCase(r);
            }
        }
    }
    return concepts;
}

vector<set<int>> Executive::induceRules(vector<AV *> AV, set<int> B){
    set<int> G = B;
    vector<vector<set<int>>> LC;
    vector<set<int>> LC_indices;

    // LOOP: While G is non-empty
    while(!G.empty()){
        vector<set<int>> T;
        set<int> T_indices;
        vector<set<int>> T_G;
        
        // LOOP: For all attribute-value blocks
        for(int i = 0; i < AV.size(); i++){
            T_G.push_back(setIntersection(AV[i]->getBlock(), G));
        }

        // LOOP: While T is non-empty or T is not subsetEq to B
        while(T.empty() || !(subsetEq(subsetIntersection(T), B))){
            // Find optimal choice; Add it to T
            int choicePos = getOptimalChoice(AV, T_G);
            T.push_back(AV[choicePos]->getBlock());
            T_indices.insert(choicePos);

            // Update goal set
            G = setIntersection(AV[choicePos]->getBlock(), G);
                
            // Update intersections
            for(int i = 0; i < AV.size(); i++){
                set<int> coveredCases = subsetIntersection(T);
                if(subsetEq(coveredCases, AV[i]->getBlock())){
                    T_G[i].clear();
                }
                else {
                    T_G[i] = setIntersection(AV[i]->getBlock(), G);
                }
            }
        }

        if(T.size() > 1){
            // Remove unnecessary conditions
            for(int i = 0; i < T.size(); i++){
                vector<set<int>> tMinus = removeCondition(T, i);
                if(subsetEq(subsetIntersection(tMinus), B)){
                    T = tMinus;
                    T_indices.erase(i);
                }
            }
        }

        // Add to local coverings
        LC.push_back(T);
        LC_indices.push_back(T_indices);
        
        // Update goal set
        vector<set<int>> tIntersects; 
        for(vector<set<int>> T : LC){
            tIntersects.push_back(subsetIntersection(T));
        }
        G = setDifference(B, subsetUnion(tIntersects));
    }

    // Remove unnecessary rules
    if(LC.size() > 1){
        for(int i = 0; i < LC.size(); i++){
            vector<vector<set<int>>> lcMinus = removeRule(LC, i);

            vector<set<int>> tIntersects; 
            for(vector<set<int>> T : lcMinus){
                tIntersects.push_back(subsetIntersection(T));
            }
            if(subsetUnion(tIntersects) == B){
                LC = lcMinus;
                LC_indices.erase(LC_indices.begin() + i);
            }
        }
    }
    return LC_indices;
}

std::string Executive::ruleString(set<int> attributes, Concept * concept){
    int index = 0;
    stringstream stream;
    for(int a : attributes){
        stream << m_avBlocks[a]->labelString();
        if(index + 1 != attributes.size()){
            stream << " & ";
        }
        index++;
    }
    stream << " -> (" << concept->decision << ", " << concept->value << ")" << endl;
    return stream.str();
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

string Executive::removeComments(string str){
    size_t pos = str.find('!');
    if(pos != string::npos){
        str.erase(pos);
    }
    return str;
}

int Executive::getOptimalChoice(vector<AV *> AV, vector<set<int>> T_G){
    std::list<int> maxSizePos;
    int maxSize = 0, minCard = INT_MAX, pos = 0;

    // LOOP: For each set intersection
    for(int i = 0; i < T_G.size(); i++){
        // IF: Set is non-empty
        if(!(T_G[i].empty())){
            // IF: Current size is larger than maxSize, clear list and add index
            if(T_G[i].size() > maxSize){
                maxSize = T_G[i].size();
                maxSizePos.clear();
                maxSizePos.push_back(i);
            // IF: Current size is equal to maxSize, add index for consideration
            } else if (T_G[i].size() == maxSize){
                maxSizePos.push_back(i);
            }
        }
    }

    // IF: Unique largest size is found, return position
    if(maxSizePos.size() == 1){
        return maxSizePos.front();
    }

    // LOOP: For each set intersection in maxSizePos
    for(int i : maxSizePos){
        // IF: Set is non-empty
        if(!(T_G[i].empty())){
            // IF: Current cardinality is smaller than minCard, update minCard
            if(AV[i]->size() < minCard){
                minCard = AV[i]->size();
                pos = i;
            // IF: Current size is equal to minCard, cardinality is not unique
            } else if (AV[i]->size() == minCard){
                break;
            }
        }
    }
    // RETURN: Position with smallest cardinality or first occuring if tie
    return pos;     
}

vector<set<int>> Executive::removeCondition(vector<set<int>> & T, int index){
    vector<set<int>> temp = T;
    temp.erase(temp.begin() + index);
    return temp;
}

vector<vector<set<int>>> Executive::removeRule(vector<vector<set<int>>> & T, int index){
    vector<vector<set<int>>> temp = T;
    temp.erase(temp.begin() + index);
    return temp;
}