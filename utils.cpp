// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Source file for utils.
    @file utils.cpp 
    This class provides project-wide
    utility functions for sets, etc. */

#include "utils.hpp"
#include <iostream>

using namespace std;

set<int> setDifference(set<int> setA, set<int> setB){
    if(setA.empty() || setB.empty()){
        return setA;
    }
    set<int> result;
    set_difference(setA.begin(), setA.end(),
                        setB.begin(), setB.end(),
                        inserter(result, result.begin()));
    return result;
}

set<int> setIntersection(set<int> setA, set<int> setB){
    if(setB.empty()){
        return setB;
    } else if(setA.empty()){
        return setA;
    }
    set<int> result;
    set_intersection(setA.begin(), setA.end(),
                          setB.begin(), setB.end(),
                          inserter(result, result.begin()));
    return result;
}

set<int> setsIntersection(vector<set<int> > sets){
    set<int> result;
    if(sets.empty()){
        return result;
    }
    if(sets.size() == 1){
        return sets[0];
    }

    set_intersection(sets[0].begin(), sets[0].end(),
                     sets[1].begin(), sets[1].end(),
                     inserter(result, result.begin()));
    
    if(sets.size() == 2){
        return result;
    }

    set<int> buffer;    
    for(unsigned i = 2; i < sets.size(); i++){
        buffer.clear();

        set_intersection(result.begin(), result.end(),
                              sets[i].begin(), sets[i].end(),
                              inserter(buffer, buffer.begin()));
        swap(result, buffer);
    }
    return result;
}

set<int> setUnion(set<int> setA, set<int> setB){
    if(setB.empty()){
        return setA;
    } else if(setA.empty()){
        return setB;
    }
    set<int> result;
    set_union(setA.begin(), setA.end(),
                   setB.begin(), setB.end(),
                   inserter(result, result.begin()));
    return result;
}

set<int> setsUnion(vector<set<int> > sets){
    set<int> result;
    if(sets.empty()){
        return result;
    }
    if(sets.size() == 1){
        return sets[0];
    }

    set_union(sets[0].begin(), sets[0].end(),
              sets[1].begin(), sets[1].end(),
              inserter(result, result.begin()));
    
    if(sets.size() == 2){
        return result;
    }

    set<int> buffer;    
    for(unsigned i = 2; i < sets.size(); i++){
        buffer.clear();

        set_union(result.begin(), result.end(),
                  sets[i].begin(), sets[i].end(),
                  inserter(buffer, buffer.begin()));
        swap(result, buffer);
    }
    return result;
}

bool subsetEq(set<int> setA, set<int> setB){
    if(setA.empty()){
        return true;
    }
    return includes(setB.begin(), setB.end(),
                         setA.begin(), setA.end());
}

bool commonElements(set<int> setA, set<int> setB){
    return !(setIntersection(setA, setB).empty());
}

void printSet(string label, set<int> set){
    if(set.empty()){
        cout << label << "{}\n";
        return;
    }
    cout << label << "{";
    for(int i : set){
        cout << i << ", ";
    }
    cout << "}\n";
}

void printList(string label, vector<set<int> > list){
    if(list.empty()){
        cout << label << "{}\n";
        return;
    }
    cout << label << "{\t";
    for(set<int> set : list){
        cout << "{";
        for(int i : set){
            cout << i << ", ";
        }
        cout << "},\n\t";
    }
    cout << "}\n";
}

void printMap( map<int, set<int>> map ) {
    // FOR: Each rule, print to file
    for( auto const & [x, y] : map ){
        cout << "T[" << x << "] = ";
        printSet("", y);
    }
}