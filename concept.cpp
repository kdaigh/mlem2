// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Source file for the concept object.
    @file concept.cpp */

#include "concept.hpp"

using namespace std;

Concept::Concept(string d, string v) 
    : m_decision(d), m_value(v) {}
    
string Concept::getDecision() const {
    return m_decision;
}

string Concept::getValue() const {
    return m_value;
}

set<int> Concept::getBlock() const {
    return m_block;
}

size_t Concept::getBlockSize() const {
    return m_block.size();
}

void Concept::addCase(int c){ 
    m_block.insert(c); 
}

string Concept::toString(){
    stringstream stream;
    stream << "[(" << m_decision << ", " << m_value << ")] = {";
    unsigned index = 0;
    for(int x : m_block){
        stream << x;
        if(index + 1 != m_block.size()){
            stream << ", ";
        }
    }
    stream << "}";
    return stream.str();
}

string Concept::labelString(){
    stringstream stream;
    stream << "(" << m_decision << ", " << m_value << ")";
    return stream.str();
}