// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Source file for dataset object.
    @file dataset.cpp 

    This holds the dataset information
    and makes access and manipulation easier. */

#include "dataset.hpp"
#include <vector>
#include <iostream>

using namespace std;

Dataset::Dataset(const int cols) : m_cols(cols) {
    m_values = new Array2D<Value *>(m_cols, 1);
    m_attributes = new string[m_cols];
    m_numAttributes = 0;
}

Dataset::~Dataset() {
    delete[] m_attributes;
    delete m_values;
}

void Dataset::setDecision(string decision){
    m_decision = decision;
}

string Dataset::getDecision() const{
    return m_decision;
}

void Dataset::addAttribute(string attr){
    if(m_numAttributes < m_cols){
        m_attributes[m_numAttributes] = attr;
        m_numAttributes++;
        return;
    }
    cerr << "Error adding attribute" << endl;
}

string Dataset::getAttribute(size_t pos){
    if(m_numAttributes < m_cols){
        return m_attributes[pos];
    }
    cerr << "Invalid position in vector." << endl;
    return "";
}

string * Dataset::getAttributes() const{
    return m_attributes;
}

void Dataset::addValue(int col, string strValue){
    Value * value = new Value(strValue);
    m_values->add(col, value);
}

Value * Dataset::getValue(int row, int col) const {
    try {
        return m_values->at(row, col);
    }
    catch(out_of_range &o) {
        cerr << "Issue getting case.\n";
    }
    return nullptr;
}

size_t Dataset::getNumCases() const {
    return m_values->getNumRows();
}

list<float> Dataset::discretize(int col, float & min, float & max){
    // Convert numeric column to sorted list with no duplicates
    list<float> nums;
    vector<Value *> casesCol = m_values->getCol(col);
    for(int i = 0; i < casesCol.size(); i++){
        nums.push_back(casesCol[i]->getNumValue());
    }
    nums.sort();
    nums.unique();

    // Set min and max for column
    min = nums.front();
    max = nums.back();

    // Generate cutpoints by taking averages of consecutive values
    list<float> cutpoints;
    list<float>::iterator cur, next;
    for(cur = nums.begin(); cur != nums.end(); cur++){
        next = cur;
        next++;
        if(next == nums.end()){
            break;
        } else {
            float c = ((*cur) + (*next)) / 2;
            cutpoints.push_back(c);
        }
    }
    return cutpoints;
}

list<string> Dataset::getPossibleValues(int col){
    list<string> vals;
    vector<Value *> casesCol = m_values->getCol(col);
    for(int i = 0; i < casesCol.size(); i++){
        vals.push_back(casesCol[i]->getStrValue());
    }
    vals.sort();
    vals.unique();
    return vals;
}

void Dataset::print() const {
    for(int i = 0; i < m_numAttributes; i++){
        cout << m_attributes[i] << "\t";
    }
    cout << m_decision << endl;
    for(int r = 1; r <= m_values->getNumRows(); r++){
        for(int c = 0; c < m_cols; c++){
            cout << m_values->at(r, c)->getStrValue() << "\t";
        }
        cout << endl;
    }
}