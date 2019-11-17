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
    virtual ~AV(){}

    virtual int getMinValue() const { return -1; }
    virtual int getMaxValue() const { return -1; }

    /* Gets the attribute string. */
    virtual string getAttr() const {
        return m_attr;
    }

    /* Gets the block set. */
    virtual set<int> getBlock() const {
        return m_block;
    }
    
    /* Gets the size of the block set. */
    virtual size_t size() const {
        return m_block.size();
    }

    /* Returns the column number of the attribute */
    virtual int getAttrCol() const {
        return m_attrCol;
    }

    virtual void setBlock(set<int> block) {
        m_block = block;
    }
    
    /* TEMPLATE: Adds if value of block and specified case match.
       @returns True if add is successful; False, otherwise. */
    virtual bool addOnMatch(Value * value, int x) = 0;

    /* TEMPLATE: Checks if block is numeric or symbolic. */
    virtual bool isNumeric() const = 0;

    /* TEMPLATE: Outputs the attribute-value block. Used for debugging. */
    virtual void print() const = 0;

    /* TEMPLATE: Prints (Attribute, value) in the given format. */
    virtual std::string labelString() const = 0;

protected:
    string m_attr;
    int m_attrCol;
    string m_value;
    set<int> m_block;
};
#endif
