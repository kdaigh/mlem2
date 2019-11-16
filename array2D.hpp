// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Header file for Array2D template.
    @file array2D.hpp 

    This 2D array handles only arrays where 
    the number of columns is known in advance. */

#include <stdexcept>
#include <iostream>
#include <vector>

using namespace std;

template<class T>
class Array2D {
public:
    Array2D(const int cols, size_t offset = 0) 
    : m_cols(cols), m_offset(offset) {
        m_array = new vector<T>[m_cols];
    }
    ~Array2D(){
        delete[] m_array;
    }
    void add(int col, T& item){
        if(col < m_cols){
            m_array[col].push_back(item);
        }
    }
    T at(int row, int col){
        try {
            return m_array[col].at(row - m_offset);
        } catch (out_of_range){
            throw out_of_range("Invalid array coordinates.");
        } 
    }
    size_t getNumCols() const {
        return m_cols;
    }
    size_t getNumRows() const {
        return m_array[m_cols - 1].size();
    }
    vector<T> getCol(int col) const{
        return m_array[col];
    }
    vector<T> * m_array;
private:
    const int m_cols;
    size_t m_offset;
};