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
#include "utils.hpp"
#include <set>
#include <vector>
#define DEBUG false

class Algorithm {
public:
    Algorithm(std::size_t numAttributes);

    /* Generate attribute-value blocks for dataset. 
       @post m_avBlocks is initialized and populated. */
    void generateAVBlocks(Dataset * data);

    /* Generate concept blocks for dataset. */
    std::vector<Concept *> generateConcepts(Dataset * data);
    
    /* Creates string with all rules in (a, v) -> (d, v) format. */
    void generateRuleset(ostream & file, Dataset * data);

    /* Generate ruleset using MLEM2. */
    LocalCover induceRules(Concept * concept);

    /* Finds the position of the best attribute-value pair. */
    int getOptimalCondition(map<int, set<int>> T_G);

private:
    std::size_t m_numAttributes;
    std::vector<AV *> m_avBlocks;
};
#endif