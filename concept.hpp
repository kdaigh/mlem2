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

class Concept {
public:
    Concept(std::string d, std::string v);
    std::string getDecision() const;
    std::string getValue() const;
    std::set<int> getBlock() const;
    std::size_t getBlockSize() const;
    void addCase(int c);
    std::string toString();
    std::string labelString();
private:
    std::string m_decision;
    std::string m_value;
    std::set<int> m_block;
};
#endif
