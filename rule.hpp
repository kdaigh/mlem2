// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Header file for the rule class.
    @file rule.hpp */

#ifndef RULE_H
#define RULE_H
#include "av.hpp"
#include <set>
#include <string>

class Rule {
    /* Global to track current ID. */
    static int idCount;

public:
    /* Constructs new rule with unique ID. */
    Rule();

    /* Adds (the index of) a single condition from the rule. */  
    void addCondition(int index);
    
    /* Removes (the index of) a single condition from the rule. */
    void removeCondition(int index);

    /* Checks for the occurence of (the index of) a condition in the rule. 
       @returns True if the condition is found; False, otherwise. */
    bool containsCondition(int index) const;

    /* Retrieves the intersection of all attribute-value pairs in the rule.
       @param avBlocks, array of attribute-value blocks. */
    std::set<int> getBlock(std::vector<AV *> avBlocks) const;

    /* Optimize rule by merging intervals. 
       @param avBlocks, array of attribute-value blocks.*/
    void mergeIntervals(std::vector<AV *> avBlocks);

    /* Optimizes rule by linearly dropping unnecessary conditions.
       @param B, set for the concept. */
    void dropConditions(std::vector<AV *> avBlocks, std::set<int> B);

    /* Converts rule to string, with format (A1, V1) & ... & (AN, VN). */
    std::string toString(std::vector<AV *> avBlocks) const;

    /* Gets number of conditions. */
    std::size_t size() const;

    /* Checks if rule is empty (i.e. has no conditions). */
    bool empty() const;

    /* Unique identifier for the rule. */
    const int id;
    
    /* Set of indices of selected conditions for rule. */
    std::set<int> m_conditions;
private:
    // TODO: move m_conditions here
};
#endif
