// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Source file for the rule class.
    @file rule.cpp */

#include "avNumeric.hpp"
#include "rule.hpp"
#include "utils.hpp"
#include <vector>
#define DEBUG true

using namespace std;

int Rule::idCount = 0;

Rule::Rule() :
    id(idCount++){ }

void Rule::addCondition(int index){
    m_conditions.insert(index);
}
    
void Rule::removeCondition(int index){
    m_conditions.erase(index);
}

bool Rule::containsCondition(int index) const {
    if(m_conditions.find(index) == m_conditions.end()){
        return false;
    }
    return true;
}

set<int> Rule::getBlock(vector<AV *> avBlocks) const {
    vector<set<int>> blocks;
    for(int i : m_conditions){
        blocks.push_back(avBlocks[i]->getBlock());
    }
    return (setsIntersection(blocks));
}

void Rule::mergeIntervals(vector<AV *> avBlocks) {
    unsigned i = 0;

    // WHILE: There are two elements left to compare
    while( i < (m_conditions.size() - 1) ){
        bool increment = true;
        set<int>::iterator curIter = next(m_conditions.begin(), i);
        set<int>::iterator nextIter = next(curIter); 
        AV * curBlock = avBlocks[(*curIter)];
        AV * nextBlock = avBlocks[(*nextIter)];

        #if DEBUG == true
            cout << "index: " << i << endl;
            cout << "T = {";
            for(int c : m_conditions){
                cout << avBlocks[c]->labelString() << ", ";
            }
            cout << "}\n";
        #endif

        // IF: Current and next attribute-value blocks are numeric
        if(curBlock->isNumeric() && nextBlock->isNumeric()){
            // IF: Current and next blocks have same attribute
            if(curBlock->getAttr() == nextBlock->getAttr()){
                int mergedMin = max(curBlock->getMinValue(), nextBlock->getMinValue());
                int mergedMax = min(curBlock->getMaxValue(), nextBlock->getMaxValue());
                // If: Intervals overlap, merge intervals
                if(mergedMax > mergedMin){
                    #if DEBUG == true
                        cout << "Merging blocks " << curBlock->labelString() << " & " << nextBlock->labelString() << endl;
                    #endif

                    int pos = -1;
                    for(unsigned j = 0; j < avBlocks.size(); j++){
                        if(avBlocks[j]->getMinValue() == mergedMin && avBlocks[j]->getMaxValue() == mergedMax){
                            pos = j;

                            #if DEBUG==true
                                cout << "Found merged block @ " << j << endl;
                            #endif

                            break;
                        }
                    }

                    // IF: Block does not already exist, create one
                    if(pos == -1){
                        pos = avBlocks.size();
                        avBlocks.push_back(new AVNumeric(curBlock->getAttr(), -1, mergedMin, mergedMax));
                        avBlocks[pos]->setBlock(setIntersection(curBlock->getBlock(), nextBlock->getBlock()));
                        #if DEBUG==true
                            cout << "Created new block for " << avBlocks[pos]->labelString() << " @ " << pos << endl;
                        #endif
                    }
                    m_conditions.erase((*curIter));
                    m_conditions.erase((*nextIter));
                    m_conditions.insert(pos);
                    increment = false;
                } // END IF
            } // END IF
        } // END IF

        // IF: Current block is unchanged, consider next
        if(increment){ i++; }

    } // END WHILE
}

void Rule::dropConditions(vector<AV *> avBlocks, set<int> B){ 
    // IF: Rule has one or less condition
    if(m_conditions.size() <= 1){
        return;
    }
    // Else: Rule has at least two conditions
    for(int c : m_conditions){
        Rule r = *this;
        r.removeCondition(c);
        if(subsetEq(r.getBlock(avBlocks), B)){
            this->removeCondition(c);
        }
    }
}

string Rule::toString(vector<AV *> avBlocks) const {
    stringstream stream;

    unsigned index = 0;
    for(int c : m_conditions){
        stream << avBlocks[c]->labelString();
            if(index + 1 != m_conditions.size()){
            stream << " & ";
        }
        index++;
    }
    return stream.str();
}

size_t Rule::size() const {
    return m_conditions.size();
}

bool Rule::empty() const {
    return m_conditions.empty();
}