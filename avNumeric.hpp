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

using namespace std;

class AVNumeric : public AV {
public:
    AVNumeric(string attr, int attrCol, float minValue, float maxValue) 
        : AV(attr, attrCol) {
        m_minValue = minValue;
        m_maxValue = maxValue;
    }
    bool addOnMatch(Value * value, int caseNo){
        if(value->getNumValue() >= m_minValue && value->getNumValue() <= m_maxValue){
            m_block.emplace(caseNo);
            return true;
        }
        return false;
    }
    void print() const {
        cout << "[(" << m_attr << ", " << m_minValue << ".." << m_maxValue << ")] = {";
        for(int caseNo : m_block){
            cout << caseNo << ", ";
        }
        cout << "}" << endl;
    }
private:
    float m_minValue;
    float m_maxValue;
};
#endif