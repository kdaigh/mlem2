// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Header file for the attribute-value
    numeric block object. This is a subclass
    of the block type.
    @file avNumeric.hpp */

#ifndef AV_NUMERIC_H
#define AV_NUMERIC_H
#include "av.hpp"
#include <list>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

class AVNumeric : public AV {
public:
    AVNumeric(string attr, int attrCol, float minValue, float maxValue) 
        : AV(attr, attrCol) {
        m_minValue = minValue;
        m_maxValue = maxValue;
    }
    int getMinValue() const {
        return m_minValue;
    }
    int getMaxValue() const {
        return m_maxValue;
    }
    bool addOnMatch(Value * value, int x){
        if(value->getNumValue() >= m_minValue && value->getNumValue() <= m_maxValue){
            m_block.insert(x);
            return true;
        }
        return false;
    }
    bool isNumeric() const {
        return true;
    }
    // bool overlap(AV * block){
    //     // IF: Overlap
    //     if(m_maxValue > block->getMinValue() && block->getMaxValue() > m_minValue){
    //         return true;
    //     }
    //     // IF: Overlap
    //     else if(block->getMaxValue() > m_minValue && m_maxValue > block->getMinValue()){
    //         return true;
    //     }
    //     // ELSE: No overlap
    //     return false;
    // }
    void print() const {
        cout << "[(" << m_attr << ", " << m_minValue << ".." << m_maxValue << ")] = {";
        for(int caseNo : m_block){
            cout << caseNo << ", ";
        }
        cout << "}" << endl;
    }
    std::string labelString() const {
        stringstream stream;
        stream << "(" << m_attr << ", ";
        stream << setprecision(8) << m_minValue << "..";
        stream << setprecision(8) << m_maxValue << ")";
        return stream.str();
    }
private:
    float m_minValue;
    float m_maxValue;
};
#endif
