// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Header file for algorithm class.
    @file algorithm.hpp 

    This class handles the execution
    of the MLEM2 algorithm. */

#ifndef ALGORITHM_H
#define ALGORITHM_H
#include "avNumeric.hpp"
#include "avSymbolic.hpp"
#include "concept.hpp"
#include "dataset.hpp"
#include <set>
#include <vector>
#define DEBUG true

class Algorithm {
public:
    Algorithm(std::size_t numAttributes);

    /* Generate attribute-value blocks for dataset. 
       @post m_avBlocks is initialized and populated. */
    void generateAVBlocks(Dataset * data);

    /* Generate concept blocks for dataset. */
    std::vector<Concept *> generateConcepts(Dataset * data);

    /* Generate ruleset using MLEM2. */
    std::vector<std::set<int>> induceRules(std::vector<AV *> AV, std::set<int> B);

    /* induceRules HELPER: Finds the position of the best choice attribute-value pair. */
    int getOptimalCondition(std::vector<AV *> AV, std::vector<std::set<int>> T_G);

    // simplifyInterval(vector<set<int>> & T, set<int> & T_indices);

    /* induceRules HELPER: Remove a single condition from a rule.
       @post T param is not modified. */
    std::vector<std::set<int>> removeCondition(std::vector<std::set<int>> & T, int index);
    
    /* induceRules HELPER: Remove a single rule from a covering.
       @post T param is not modified. */
    std::vector<std::vector<std::set<int>>> removeRule(std::vector<std::vector<std::set<int>>> & T, int index);

    /* Generates the set (Specificity, Strength, Size) for a rule).
       @returns Vector of classifications in order. */
    std::vector<int> classifyRule(std::set<int> rule, Concept * concept);
    
    /* Creates string with all rules in (a, v) -> (d, v) format. */
    std::string generateRuleset(Dataset * data);

private:
    std::size_t m_numAttributes;
    std::vector<AV *> m_avBlocks;
};
#endif