// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Header file for the attribute-value
    block object. This will be sub-classed
    with symbolic and numeric blocks.
    @file av.hpp */

#ifndef AV_H
#define AV_H
#include "value.hpp"
#include <string>
#include <set>

using namespace std;

class AV {
public:
    AV(string attr, int attrCol) 
        : m_attr(attr), m_attrCol(attrCol) { }

    virtual set<int> getBlock() const {
        return m_block;
    }

    /* Returns the column number of the attribute */
    virtual int getAttrCol() const {
        return m_attrCol;
    }
    
    /* TEMPLATE: Adds if value of block and specified case match.
       @returns True if add is successful; False, otherwise. */
    virtual bool addOnMatch(Value * value, int x) = 0;

    /* Gets the size of the block set. */
    virtual size_t size() const {
        return m_block.size();
    }

    /* TEMPLATE: Outputs the attribute-value block. Used for debugging. */
    virtual void print() const = 0;

    /* TEMPLATE: Prints (Attribute, value) in the given format. */
    virtual void printLabel() const = 0;

protected:
    string m_attr;
    int m_attrCol;
    string m_value;
    set<int> m_block;
};
#endif
