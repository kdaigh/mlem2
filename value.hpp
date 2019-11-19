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
        if(isNumber(strValue)){
            stream >> m_numValue;
            m_type = NUMERIC;
        } else {
            m_type = SYMBOLIC;
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
    bool isNumber(std::string str) const {
        // Check for symbolic ranges (i.e. "40-49")  
        size_t pos = str.find("-");
        if(pos != string::npos){
            // Dash is not the first character (it's a symbolic range)
            if(pos != 0){
                return false;
            }
        }

        // Check for symbolic ranges (i.e. "40..49")       
        if(str.find("..") != string::npos){
            return false;
        }

        // Check for non-numeric characters
        string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ[]{};:,/?=~|";
        for(char c : chars){
            if(str.find(c) != string::npos){
                return false;
            }
        }

        return true;
    }
private:
    string m_strValue;
    float m_numValue;
    Type m_type;
};
#endif
