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
#include "dataset.hpp"
#include "localCover.hpp"
#include "setUtils.hpp"
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
    LocalCover induceRules(Concept * concept);

    /* Finds the position of the best attribute-value pair. */
    int getOptimalCondition(std::vector<AV *> AV, std::vector<std::set<int>> T_G, std::set<int> T_G_indices);

    // /* Merges intervals for overlapping numeric attribute-value blocks. */
    // void mergeIntervals(std::vector<std::set<int>> & T, std::set<int> & T_indices);

    // /* Drops conditions that are unnecessary for the rule. */
    // void dropConditions(std::vector<std::set<int>> & T, std::set<int> & T_indices, std::set<int> B);

    // /* Drops rules that are unnecessary for the ruleset. */
    // void dropRules(std::vector<std::vector<std::set<int>>> & LC, std::vector<std::set<int>> & LC_indices, std::set<int> B);

    // /* induceRules HELPER: Remove a single condition from a rule.
    //    @post T param is not modified. */
    // std::vector<std::set<int>> removeCondition(const std::vector<std::set<int>> & T, int index);
    
    // /* induceRules HELPER: Remove a single rule from a covering.
    //    @post T param is not modified. */
    // std::vector<std::vector<std::set<int>>> removeRule(const std::vector<std::vector<std::set<int>>> & T, int index);

    // /* Generates the set (Specificity, Strength, Size) for a rule).
    //    @returns Vector of classifications in order. */
    // std::vector<int> classifyRule(Rule * rule, Concept * concept);
    
    /* Creates string with all rules in (a, v) -> (d, v) format. */
    void generateRuleset(ostream & file, Dataset * data);

private:
    std::size_t m_numAttributes;
    std::vector<AV *> m_avBlocks;
};
#endif