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

    // TODO: Generate concepts
    // TEST: Test concept for yes
    set<int> conceptYes;
    conceptYes.emplace(1);
    conceptYes.emplace(2);
    conceptYes.emplace(3);

    set<set<int>> localCovering = MLEM2(m_avBlocks, conceptYes);



    // mlem2->induceRules(conceptYes);
}

set<set<int>> Executive::MLEM2(vector<AV *> AV, set<int> B){
    set<int> G = B;
    set<set<int>> LC;
    // LOOP: While G is non-empty
    while(!G.empty()){
        printSet("B = ", B);
        printSet("G = ", G);
        set<int> T;
        vector<set<int>> T_G;
        //set<int> T_G;

        // LOOP: For all attribute-value blocks
        for(int i = 0; i < AV.size(); i++){
            // IF: Intersection of [(a,v)] and G is non-empty
            // if(commonElements(AV[i]->getBlock(), G)){
            //     T_G.insert(i);
            // }  
            T_G.push_back(AV[i]->getBlock);
        }

        for(set<int> t : T_G){
            printSet("t(G)", t);
        }
        //printSet("T(G) = ", T_G);

        vector<set<int>> test = getBlocks(AV, T);
        for(set<int> t : test){
            printSet("t = ", t);
        }
        
        set<int> test1 = subsetIntersection(getBlocks(AV, T));
        printSet("[T] = ", test1);

        // LOOP: While T is non-empty or T is not subsetEq to B
        while(T.empty() || !(subsetEq(subsetIntersection(getBlocks(AV, T)), B))){
            // Find optimal choice; Add it to [t]
            int choicePos = getOptimalChoice(AV, getBlocks(AV, T_G));
            T.insert(choicePos);

            // TEST
            //std::cout << "best choice @ " << choicePos << std::endl;
            //printSet("T = ", T);

            // Update goal set
            G = setIntersection(AV[choicePos]->getBlock(), G);

            // LOOP: For all attribute-value blocks
            for(int i = 0; i < AV.size(); i++){
                // IF: Intersection of [(a,v)] and G is non-empty
                if(commonElements(AV[i]->getBlock(), G)){
                    T_G.insert(i);
                }  
            }
            T_G = setDifference(T_G, T);
        }
        for(int t : T){
            set<int> TMinusT = reduceT(T, t);
            if(subsetEq(subsetIntersection(getBlocks(AV, TMinusT)), B)){
                T = TMinusT;
            }
        }
        LC.insert(T);
        G = setDifference(B, subsetUnion(getListBlocks(AV, LC)));

        for(set<int> T : LC){
            set<set<int>> temp = reduceLC(LC, T);
            if(subsetUnion(getListBlocks(AV, temp)) == B){
                LC = temp;
            }
        }
    }
    return LC;
}

vector<set<int>> Executive::getBlocks(vector<AV *> AV, set<int> & cases){
    vector<set<int>> retVec;
    for(int c : cases){
        retVec.push_back(AV[c]->getBlock());
    }
    return retVec;
}

vector<set<int>> Executive::getListBlocks(vector<AV *> AV, set<set<int>> & cases){
    vector<set<int>> retVec;
    for(set<int> c : cases){
        retVec.push_back(subsetIntersection(getBlocks(AV, c)));
    }
    return retVec;
}

set<int> Executive::reduceT(set<int> & T, int t){
    set<int> temp = T;
    temp.erase(t);
    return temp;
}

set<set<int>> Executive::reduceLC(set<set<int>> & LC, set<int> T){
    set<set<int>> temp = LC;
    temp.erase(T);
    return temp;
}

/* Uses criteria to select optimal choice.
   @returns Position of optimal choice. */
int Executive::getOptimalChoice(vector<AV *> AV, vector<set<int>> T_G){
    std::list<int> maxSizePos;
    int maxSize = 0, minCard = INT_MAX, pos = -1;
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

