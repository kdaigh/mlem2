// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Header file for dataset object.
    @file dataset.hpp 

    This holds the dataset information
    and makes access and manipulation easier. */

#ifndef DATASET_H
#define DATASET_H
#include "array2D.hpp"
#include "value.hpp"
#include <list>
#include <vector>
#include <string>

class Dataset {
public:
    Dataset(const int cols);
    ~Dataset();

    void setDecision(std::string decision);
    std::string getDecision() const;
    void addAttribute(std::string attr);
    std::string getAttribute(size_t pos);
    std::string * getAttributes() const;
    void addValue(int col, std::string strValue);
    Value * getValue(int row, int col) const;
    std::size_t getNumCases() const;

    /* Performs discretization on numeric column.
       @post Min and Max are updated.
       @returns List of cutpoints. */
    std::list<float> discretize(int col, float & min, float & max);

    /* Gets the possible values for a symbolic column. */
    std::list<std::string> getPossibleValues(int col);

    void print() const;

private:
    const int m_cols;
    std::string m_decision;
    std::string * m_attributes;
    int m_numAttributes;
    Array2D<Value *> * m_values;
};
#endif