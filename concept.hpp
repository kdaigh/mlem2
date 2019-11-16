// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Header file for the concept object.
    @file concept.hpp */

#ifndef CONCEPT_H
#define CONCEPT_H
#include <string>
#include <sstream>
#include <set>

using namespace std;

class Concept {
public:
    Concept(string d, string v) 
        : m_decision(d), m_value(v) {}
    string getDecision() const {
        return m_decision;
    }
    string getValue() const {
        return m_value;
    }
    set<int> getBlock() const {
        return m_block;
    }
    size_t getBlockSize() const {
        return m_block.size();
    }
    void addCase(int c){ 
        m_block.insert(c); 
    }
    string toString(){
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
private:
    string m_decision;
    string m_value;
    set<int> m_block;
};
#endif
