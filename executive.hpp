// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Header file for executive class.
    @file executive.hpp 

    This class handles the execution
    of the program. */

#ifndef EXECUTIVE_H
#define EXECUTIVE_H
#include "av.hpp"
#include "avNumeric.hpp"
#include "avSymbolic.hpp"
#include "dataset.hpp"
#include <string>

class Concept {
public:
    Concept(string d, string v) : decision(d), value(v) {}

    void addCase(int c){ block.insert(c); }

    string decision;
    string value;
    set<int> block;
};

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

    /* Generate attribute-value blocks for dataset. 
       @post m_avBlocks is initialized and populated. */
    void generateAVBlocks();

    /* Generate concept blocks for dataset. */
    std::vector<Concept *> generateConcepts();

    /* Generate ruleset using MLEM2. */
    std::vector<std::set<int>> induceRules(std::vector<AV *> AV, std::set<int> B);
    
    /* Prints a single rule in (a, v) -> (d, v) format. */
    std::string ruleString(std::set<int> attributes, Concept * decision);

private:
    Dataset * m_data;
    std::size_t m_numAttributes;
    std::vector<AV *> m_avBlocks;

    /* parseFile HELPER: Parses format line. */
    void parseFormat(std::istream& file);

    /* parseFile HELPER: Parses header line. */
    void parseHeader(std::istream& file);

    /* parseFile HELPER: Removes comments. */
    std::string removeComments(std::string str);

    /* induceRules HELPER: Finds the position of the best choice attribute-value pair. */
    int getOptimalChoice(std::vector<AV *> AV, std::vector<std::set<int>> T_G);

    /* induceRules HELPER: Remove a single condition from a rule.
       @post T param is not modified. */
    std::vector<std::set<int>> removeCondition(std::vector<std::set<int>> & T, int index);
    
    /* induceRules HELPER: Remove a single rule from a covering.
       @post T param is not modified. */
    std::vector<std::vector<std::set<int>>> removeRule(std::vector<std::vector<std::set<int>>> & T, int index);
};
#endif