// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Source file for the local 
    covering class.
    @file localCover.hpp */

#ifndef LOCAL_COVER_H
#define LOCAL_COVER_H
#include "av.hpp"
#include "concept.hpp"
#include "rule.hpp"
#include "utils.hpp"
#include <map>
#include <set>
#include <vector>

class LocalCover {
public:
    LocalCover(Concept * concept)
        : m_concept(concept) { }

    /* Adds a single rule to the covering. */  
    void addRule(Rule * rule);
    
    /* Removes a single rule from the covering. */
    void removeRule(Rule * rule);

    /* Gets the classification string for a rule: (Specificity, Strength, Size) */
    std::string classifyRule(std::vector<AV *> avBlocks, Rule * rule) const;

    /* Retrieves the union of all rule blocks. */
    set<int> getCoveredConditions(std::vector<AV *> avBlocks) const;

    /* Optimizes covering by removing unnecessary rules. */
    void dropRules(std::vector<AV *> avBlocks, std::set<int> B);

    /* Converts covering to string. */
    std::string toString(std::vector<AV *> avBlocks) const;
private:
    /* Set of indices of selected conditions for rule. */
    std::map<int, Rule *> m_rules;

    /* Concept for the local covering. */
    Concept * m_concept;
};
#endif
