// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Header file for the attribute-value
    symbolic block object. This is a subclass
    of the block type.
    @file avSymbolic.hpp */

#ifndef AV_SYMBOLIC_H
#define AV_SYMBOLIC_H
#include "av.hpp"
#include <list>
#include <iostream>
#include <string>
#include <sstream>

class AVSymbolic : public AV {
public:
    AVSymbolic(string attr, int attrCol, string value) 
        : AV(attr, attrCol) {
        m_value = value;
    }
    bool addOnMatch(Value * value, int x){
        if(m_value.compare(value->getStrValue()) == 0){
            m_block.insert(x);
            return true;
        }
        return false;
    }
    bool isNumeric() const {
        return false;
    }
    void print() const {
        cout << "[(" << m_attr << ", " << m_value << ")] = {";
        for(int caseNo : m_block){
            cout << caseNo << ", ";
        }
        cout << "}" << endl;
    }
    std::string labelString() const {
        stringstream stream;
        stream << "(" << m_attr << ", " << m_value << ")";
        return stream.str();
    }
private:
    string m_value;
};
#endif
