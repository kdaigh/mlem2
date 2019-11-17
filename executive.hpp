// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Header file for executive class.
    @file executive.hpp 

    This class handles the execution
    of the program. */

#ifndef EXECUTIVE_H
#define EXECUTIVE_H
#include "dataset.hpp"
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

private:
    Dataset * m_data;
    std::size_t m_numAttributes;

    /* parseFile HELPER: Parses format line. */
    void parseFormat(std::istream& file);

    /* parseFile HELPER: Parses header line. */
    void parseHeader(std::istream& file);

    /* parseFile HELPER: Removes comments. */
    std::string removeComments(std::istream & file);
};
#endif