// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Header file for executive class.
    @file executive.hpp 

    This class handles the execution
    of the program. */

#ifndef EXECUTIVE_H
#define EXECUTIVE_H
#include "avNumeric.hpp"
#include "avSymbolic.hpp"
#include "av.hpp"
#include "dataset.hpp"
#include <string>

class Executive {
public:
    /* Initializes member data structures and values. */
    Executive();

    /* Destroys member data structures. */
    ~Executive();

    /* Reads the file into a dataset structure.
       @returns True if file read is successful; false, otherwise. */
    bool parseInFile(std::string filename);

    /* Generate attribute-value blocks for dataset. */
    void generateBlocks();

    /* Runs MLEM2 on the dataset. */
    void runMLEM2();

    std::vector<std::vector<std::set<int>>> MLEM2(std::vector<AV *> AV, std::set<int> B);

    int getOptimalChoice(std::vector<AV *> AV, std::vector<std::set<int>> T_G);

    std::vector<std::set<int>> removeCondition(std::vector<std::set<int>> & T, int index);
    std::vector<std::vector<std::set<int>>> removeRule(std::vector<std::vector<std::set<int>>> & T, int index);

    /* Prints ruleset structure to a file.
       @returns True if file write is successful; false, otherwise. */
    bool generateOutFile(std::string filename);

private:
    Dataset * m_data;
    std::size_t m_numAttributes;
    std::size_t m_numCols;
    std::vector<AV *> m_avBlocks;

    /* parseFile HELPER: Removes comments. */
    std::string removeComments(std::string str);

    /* parseFile HELPER: Parses format line. */
    void parseFormat(std::istream& file);

    /* parseFile HELPER: Parses header line. */
    void parseHeader(std::istream& file);
};
#endif