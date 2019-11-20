// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Source file for the local 
    covering class.
    @file localCover.cpp */

#include "localCover.hpp"

using namespace std;

void LocalCover::addRule(Rule * rule){
    m_rules[rule->id] = rule;
}

void LocalCover::removeRule(Rule * rule){
    m_rules.erase(rule->id);
}

string LocalCover::classifyRule(vector<AV *> avBlocks, Rule * rule) const {
    stringstream result;
    
    // Intersect all attribute blocks
    set<int> matchLHS = rule->getBlock(avBlocks);

    // Intersect all attribute blocks with concept block
    set<int> matchCase = setIntersection(rule->getBlock(avBlocks), m_concept->getBlock());

    result << rule->size() << ", " << matchCase.size() << ", " << matchLHS.size() << endl;

    return result.str();
}

set<int> LocalCover::getCoveredConditions(vector<AV *> avBlocks) const {
    vector<set<int>> blocks;
    for( auto const & [id, rule] : m_rules){
        blocks.push_back(rule->getBlock(avBlocks));
    }
    return (setsUnion(blocks));
}

void LocalCover::dropRules(vector<AV *> avBlocks, set<int> B){
    // IF: Covering has one or less rule
    if(m_rules.size() <= 1){
        return;
    }
    // Else: Covering has at least two rules
    map<int, set<int>>::iterator iter;
    for(iter = m_rules.begin(); iter != m_rules.end();){
        LocalCover lc = *this;
        lc.removeRule(rule);
        if(lc.getCoveredConditions(avBlocks) == B){
            iter = m_rules.erase(iter);
        } else {
            iter++;
        }
    }
}

string LocalCover::toString(std::vector<AV *> avBlocks) const {
    stringstream result;
    // FOR: Each rule, print to file
    for(auto const & [id, rule] : m_rules){
        result << classifyRule(avBlocks, rule);
        result << rule->toString(avBlocks);
        result << " -> " << m_concept->labelString() << endl;
    }
    return result.str();
}