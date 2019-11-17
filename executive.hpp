// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Header file for executive class.
    @file executive.hpp 

    This class handles the execution
    of the program. */

#ifndef EXECUTIVE_H
#define EXECUTIVE_H
// #include "av.hpp"
// #include "avNumeric.hpp"
// #include "avSymbolic.hpp"
#include "dataset.hpp"
// #include "concept.hpp"
#include <string>

class Executive {
public:
    Executive();
    ~Executive();

    /* Reads the file into a dataset structure.
       @returns True if file read is successful; false, otherwise. */
    bool parseInFile(std::string filename);

    /* Prints ruleset structure to a file.
       @returns True if file write is successful; false, otherwise. */
    bool generateOutFile(std::string filename);

    // /* Generate attribute-value blocks for dataset. 
    //    @post m_avBlocks is initialized and populated. */
    // void generateAVBlocks();

    // /* Generate concept blocks for dataset. */
    // std::vector<Concept *> generateConcepts();

    // /* Generate ruleset using MLEM2. */
    // std::vector<std::set<int>> induceRules(std::vector<AV *> AV, std::set<int> B);

    // /* Generates the set (Specificity, Strength, Size) for a rule).
    //    @returns Vector of classifications in order. */
    // std::vector<int> classifyRule(std::set<int> rule, Concept * concept);
    
    // /* Prints a single rule in (a, v) -> (d, v) format. */
    // std::string ruleString(std::set<int> rule, Concept * concept, std::vector<int> classSet);

private:
    Dataset * m_data;
    std::size_t m_numAttributes;
    // std::vector<AV *> m_avBlocks;

    /* parseFile HELPER: Parses format line. */
    void parseFormat(std::istream& file);

    /* parseFile HELPER: Parses header line. */
    void parseHeader(std::istream& file);

    /* parseFile HELPER: Removes comments. */
    std::string removeComments(std::istream & file);

    // /* induceRules HELPER: Finds the position of the best choice attribute-value pair. */
    // int getOptimalChoice(std::vector<AV *> AV, std::vector<std::set<int>> T_G);

    // /* induceRules HELPER: Remove a single condition from a rule.
    //    @post T param is not modified. */
    // std::vector<std::set<int>> removeCondition(std::vector<std::set<int>> & T, int index);
    
    // /* induceRules HELPER: Remove a single rule from a covering.
    //    @post T param is not modified. */
    // std::vector<std::vector<std::set<int>>> removeRule(std::vector<std::vector<std::set<int>>> & T, int index);
};
#endif