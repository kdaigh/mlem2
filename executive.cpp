// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Source file for executive class.
    @file executive.cpp 

    This class handles the execution
    of the program. */

#include "algorithm.hpp"
#include "executive.hpp"
// #include "setUtils.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
// #include <stdexcept>
// #include <algorithm>
#define DEBUG true

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

    Algorithm mlem2(m_numAttributes);
    file << mlem2.generateRuleset(m_data);

    // // Generate program components
    // generateAVBlocks();
    // vector <Concept *> concepts = generateConcepts();

    // // FOR: Each concept, induce rules
    // for(Concept * c : concepts){
    //     #if DEBUG == true
    //         cout << c->toString() << endl;
    //     #endif
        
    //     vector<set<int>> ruleset = induceRules(m_avBlocks, c->getBlock());
    //     printList("Ruleset = ", ruleset);
    //     // FOR: Each rule, print to file
    //     for(set<int> r : ruleset){
    //         vector<int> classification = classifyRule(r, c);
    //         file << ruleString(r, c, classification);
    //     }
    //     delete c;
    // }

    file.close();
    return true;
}

// void Executive::generateAVBlocks(){
//     // FOR: Each attribute (column)
//     for(unsigned col = 0; col < m_numAttributes; col++){
//         string attr = m_data->getAttribute(col);

//         // IF: Attribute values are numeric
//         if(m_data->getValue(1, col)->isNumeric()){
//             float min = 0, max = 0;
//             list<float> cutpoints = m_data->discretize(col, min, max);

//             // FOR: Each cutpoint, create (empty) attribute-value blocks 
//             for (float c : cutpoints){
//                 m_avBlocks.push_back(new AVNumeric(attr, col, min, c));
//                 m_avBlocks.push_back(new AVNumeric(attr, col, c, max));
//             }
//         }
//         // ELSE: Attribute values are symbolic
//         else {
//             list<string> values = m_data->getPossibleValues(col);

//             // FOR: Each value, create an (empty) attribute-value block
//             for (string v : values){
//                 m_avBlocks.push_back(new AVSymbolic(attr, col, v));
//             }
//         }
//     } // END FOR

//     // Populate attribute-value blocks
//     // LOOP: For each attribute-value block
//     for(unsigned i = 0; i < m_avBlocks.size(); i++){

//         // LOOP: For each case (row), add matching values to the block
//         for(unsigned r = 1; r <= m_data->getNumCases(); r++){
//             int c = m_avBlocks[i]->getAttrCol();
            
//             m_avBlocks[i]->addOnMatch(m_data->getValue(r, c), r);    
//         }

//         #if DEBUG == true
//            m_avBlocks[i]->print();
//         #endif
//     }
// }

// vector<Concept *> Executive::generateConcepts(){
//     vector <Concept *> concepts;
//     list<string> values = m_data->getPossibleValues(m_numAttributes);
//     for(string v : values){
//         concepts.push_back(new Concept(m_data->getDecision(), v));
//     }

//     // FOR: Each concept
//     for(unsigned i = 0; i < concepts.size(); i++){
//         // FOR: Each case (row), add matching values to the block
//         for(unsigned r = 1; r <= m_data->getNumCases(); r++){
//             int c = m_numAttributes;
//             string cellValue = m_data->getValue(r, c)->getStrValue();
//             if(concepts[i]->getValue() == cellValue){
//                 concepts[i]->addCase(r);
//             }
//         }
//     }
//     return concepts;
// }

// vector<set<int>> Executive::induceRules(vector<AV *> AV, set<int> B){
//     set<int> G = B;
//     vector<vector<set<int>>> LC;
//     vector<set<int>> LC_indices;

//     // WHILE: G is non-empty
//     while(!G.empty()){
//         vector<set<int>> T;
//         set<int> T_indices;
//         vector<set<int>> T_G;

//         #if DEBUG == true
//             printSet("G = ", G);
//         #endif
        
//         // FOR: All attribute-value blocks
//         for(unsigned i = 0; i < AV.size(); i++){
//             T_G.push_back(setIntersection(AV[i]->getBlock(), G));
//         }
        
//         // Select conditions for rule
//         // WHILE: T is non-empty or T is not subsetEq to B
//         while( (T.empty()) || !(subsetEq(subsetIntersection(T), B)) ){
//             // Find optimal choice; Add it to T
//             int choicePos = getOptimalChoice(AV, T_G);
//             T.push_back(AV[choicePos]->getBlock());
//             T_indices.insert(choicePos);

//             #if DEBUG == true
//                 printSet("T_indices = ", T_indices);
//             #endif
                
//             // Update goal set
//             G = setIntersection(AV[choicePos]->getBlock(), G);
                
//             // Update intersections
//             for(unsigned i = 0; i < AV.size(); i++){
//                 set<int> coveredCases = subsetIntersection(T);
//                 if(subsetEq(coveredCases, AV[i]->getBlock())){
//                     T_G[i].clear();
//                 }
//                 else {
//                     T_G[i] = setIntersection(AV[i]->getBlock(), G);
//                 }
//             }
//         } // END WHILE (INNER LOOP)

//         // Remove unnecessary conditions (interval simplification)
//         if(T.size() > 1){
//             for(unsigned i = 0; i < (T.size() - 1); i++){
//                 // IF: Current and next attribute-value blocks are numeric
//                 if(m_avBlocks[i]->isNumeric() && m_avBlocks[i + 1]->isNumeric()){
//                     // IF: Current and next blocks have same attribute
//                     if(m_avBlocks[i]->getAttr() == m_avBlocks[i + 1]->getAttr()){
//                         // IF: Intervals overlap
//                         if(m_avBlocks[i]->overlap(m_avBlocks[i + 1])){
//                             cout << "overlap" << endl;
//                         }   
//                     }
//                 }
//             }
//         }

//         // Remove unnecessary conditions (linear dropping)
//         if(T.size() > 1){
//             for(unsigned i = 0; i < T.size(); i++){
//                 vector<set<int>> tMinus = removeCondition(T, i);
//                 if(subsetEq(subsetIntersection(tMinus), B)){
//                     T = tMinus;
//                     T_indices.erase(i);
//                 }
//             }
//         }

//         // Add to local coverings
//         LC.push_back(T);
//         LC_indices.push_back(T_indices);
        
//         // Update goal set
//         vector<set<int>> tIntersects; 
//         for(vector<set<int>> T : LC){
//             tIntersects.push_back(subsetIntersection(T));
//         }
//         G = setDifference(B, subsetUnion(tIntersects));
//     } // END WHILE (OUTER LOOP)

//     // Remove unnecessary rules
//     if(LC.size() > 1){
//         for(unsigned i = 0; i < LC.size(); i++){
//             vector<vector<set<int>>> lcMinus = removeRule(LC, i);

//             vector<set<int>> tIntersects; 
//             for(vector<set<int>> T : lcMinus){
//                 tIntersects.push_back(subsetIntersection(T));
//             }
//             if(subsetUnion(tIntersects) == B){
//                 LC = lcMinus;
//                 LC_indices.erase(LC_indices.begin() + i);
//             }
//         }
//     }
//     return LC_indices;
// }

// vector<int> Executive::classifyRule(set<int> rule, Concept * concept){
//     vector<int> ret;
//     vector<set<int>> cases;
    
//     // FOR: Each attribute in the rule, add the block
//     for(int attribute : rule){
//         cases.push_back(m_avBlocks[attribute]->getBlock());
//     }

//     // Intersect all attribute blocks
//     set<int> matchLHS = subsetIntersection(cases);

//     // Intersect all attribute blocks with concept block
//     cases.push_back(concept->getBlock());
//     set<int> matchCase = subsetIntersection(cases);

//     ret.push_back(rule.size());         // push specificity
//     ret.push_back(matchCase.size());    // push strength
//     ret.push_back(matchLHS.size());     // push size

//     return ret;
// }

// std::string Executive::ruleString(set<int> rule, Concept * concept, vector<int> classSet){
//     stringstream stream;

//     // Print classification to string
//     stream << classSet.at(0) << ", " << classSet.at(1) << ", " << classSet.at(2) << endl;

//     // Print attributes to string
//     unsigned index = 0;
//     for(int attribute : rule){
//         stream << m_avBlocks[attribute]->labelString();
//         if(index + 1 != rule.size()){
//             stream << " & ";
//         }
//         index++;
//     }

//     // Print decision to string
//     stream << " -> (" << concept->getDecision() << ", " << concept->getValue() << ")" << endl;
//     return stream.str();
// }

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

// int Executive::getOptimalChoice(vector<AV *> AV, vector<set<int>> T_G){
//     std::list<int> maxSizePos, minCardPos;
//     size_t maxSize = 0, minCard = INT_MAX;

//     // LOOP: For each set intersection
//     for(unsigned i = 0; i < T_G.size(); i++){
//         // IF: Set is non-empty
//         if(!(T_G[i].empty())){
//             // IF: Current size is larger than maxSize, clear list and add index
//             if(T_G[i].size() > maxSize){
//                 maxSize = T_G[i].size();
//                 maxSizePos.clear();
//                 maxSizePos.push_back(i);
//             // IF: Current size is equal to maxSize, add index for consideration
//             } else if (T_G[i].size() == maxSize){
//                 maxSizePos.push_back(i);
//             }
//         }
//     }

//     // IF: Unique largest size is found, return position
//     if(maxSizePos.size() == 1){
//         return maxSizePos.front();
//     }

//     // LOOP: For each set intersection in maxSizePos
//     for(int i : maxSizePos){
//         // IF: Set is non-empty
//         if(!(T_G[i].empty())){
//             // IF: Current cardinality is smaller than minCard, update minCard
//             if(AV[i]->size() < minCard){
//                 minCard = AV[i]->size();
//                 minCardPos.clear();
//                 minCardPos.push_back(i);
//             // IF: Current size is equal to minCard, cardinality is not unique
//             } else if (AV[i]->size() == minCard){
//                 minCardPos.push_back(i);
//             }
//         }
//     }
//     // RETURN: Position with smallest cardinality or first occuring if tie
//     return minCardPos.front();    
// }

// vector<set<int>> Executive::removeCondition(vector<set<int>> & T, int index){
//     vector<set<int>> temp = T;
//     temp.erase(temp.begin() + index);
//     return temp;
// }

// vector<vector<set<int>>> Executive::removeRule(vector<vector<set<int>>> & T, int index){
//     vector<vector<set<int>>> temp = T;
//     temp.erase(temp.begin() + index);
//     return temp;
// }