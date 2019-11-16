// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Header file for the attribute-value class.
    @file value.hpp */

#ifndef VALUE_H
#define VALUE_H
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

enum Type {
    SYMBOLIC, NUMERIC
};

class Value {
public:
    Value(string strValue) : m_strValue(strValue) {
        stringstream stream (strValue);
        stream >> m_numValue;
        if (stream.fail()) {
            m_type = SYMBOLIC;
        } else {
            if(isRange(strValue)){
                m_type = SYMBOLIC;
            } else {
                m_type = NUMERIC;
            }
        }
    }
    string getStrValue() const{
        return m_strValue;
    }
    float getNumValue() const{
        return m_numValue;
    }
    bool isNumeric() const{
        if(m_type == NUMERIC){
            return true;
        }
        return false;
    }
    bool isRange(std::string str) const {
        size_t pos = str.find("-");
        
        // Dash is found in string
        if(pos != string::npos){
            // Dash is not the first character (it's a symbolic range)
            if(pos != 0){
                return true;
            }
        }
        return false;
    }
private:
    string m_strValue;
    float m_numValue;
    Type m_type;
};
#endif
