// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Source file for the rule class.
    @file rule.cpp */

#include "avNumeric.hpp"
#include "rule.hpp"
#include "utils.hpp"
#include <vector>
#define DEBUG false

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

void Rule::setConditions(set<int> conditions){
    m_conditions = conditions;
}

set<int> Rule::getConditions() const {
    return m_conditions;
}

set<int> Rule::getBlock(vector<AV *> avBlocks) const {
    vector<set<int>> blocks;
    for(int i : m_conditions){
        blocks.push_back(avBlocks[i]->getBlock());
    }
    return (setsIntersection(blocks));
}

set<int> Rule::getAttributeGroup(vector<AV *> avBlocks, int index) const {
    set<int> result;
    for( int i : m_conditions ){
        if( (i != index) && (avBlocks[index]->getAttr() == avBlocks[i]->getAttr()) ){
            result.insert(i);
        }
    }
    return result;
}

void Rule::mergeIntervals(vector<AV *> avBlocks) {
    set<int> mergedConditions;
    mergedConditions.insert(m_conditions.begin(), m_conditions.end());
    for(auto iter = m_conditions.begin(); iter != m_conditions.end();){
        bool increment = true;
        int mergedMin = avBlocks[(*iter)]->getMinValue();
        int mergedMax = avBlocks[(*iter)]->getMaxValue();
        set<int> attrGroup;
        set<int> mergedBlock;
        if(avBlocks[(*iter)]->isNumeric()){
            attrGroup = getAttributeGroup(avBlocks, (*iter));
            mergedBlock = avBlocks[(*iter)]->getBlock();
            for( int i : attrGroup ){
                mergedMin = max(mergedMin, avBlocks[i]->getMinValue());
                mergedMax = min(mergedMax, avBlocks[i]->getMaxValue());
                mergedBlock = setIntersection(mergedBlock, avBlocks[i]->getBlock());
            }
            attrGroup.insert((*iter));

            // If: Intervals overlap, merge intervals
            if(mergedMax > mergedMin){
                #if DEBUG==true
                    cout << "Merging " << mergedMin << ".." << mergedMax << endl;
                #endif
                int pos = -1;
                for( int j : attrGroup){
                    if(avBlocks[j]->getMinValue() == mergedMin && avBlocks[j]->getMaxValue() == mergedMax){
                        pos = j;
                        break;
                    }
                }
                
                if(pos == -1){
                    pos = avBlocks.size();
                    avBlocks.push_back(new AVNumeric(avBlocks[(*iter)]->getAttr(), -1, mergedMin, mergedMax));
                    avBlocks[pos]->setBlock(mergedBlock);            
                }

                for( int j : attrGroup ){
                    mergedConditions.erase(j);
                    iter++;
                }
                mergedConditions.insert(pos);
                increment = false;
            }
        }
        if(increment){
            iter++;
        }
    }
    printSet("mergedConditions = ", mergedConditions);
    this->setConditions(mergedConditions);
}

void Rule::dropConditions(vector<AV *> avBlocks, set<int> B){ 
    // IF: Rule has one or less condition
    if(m_conditions.size() <= 1){
        return;
    }
    // Else: Rule has at least two conditions
    set<int>::iterator iter;
    for(iter = m_conditions.begin(); iter != m_conditions.end();){
        Rule r = *this;
        r.removeCondition(*iter);
        if(subsetEq(r.getBlock(avBlocks), B)){
            #if DEBUG==true
                cout << "Dropping condition " << avBlocks[*iter]->labelString() << endl;
            #endif
            iter = m_conditions.erase(iter);
        } else {
            iter++;
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