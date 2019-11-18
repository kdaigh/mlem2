// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Header file for setUtils.
    @file setUtils.hpp 
    This class provides project-wide
    utility functions for sets, etc. */

#include <set>
#include <algorithm>
#include <vector>

std::set<int> setDifference(std::set<int> setA, std::set<int> setB){
    if(setA.empty() || setB.empty()){
        return setA;
    }
    std::set<int> diffSet;
    std::set_difference(setA.begin(), setA.end(),
                        setB.begin(), setB.end(),
                        std::inserter(diffSet, diffSet.begin()));
    return diffSet;
}

std::set<int> setIntersection(std::set<int> setA, std::set<int> setB){
    if(setB.empty()){
        return setB;
    } else if(setA.empty()){
        return setA;
    }
    std::set<int> intersectSet;
    std::set_intersection(setA.begin(), setA.end(),
                          setB.begin(), setB.end(),
                          std::inserter(intersectSet, intersectSet.begin()));
    return intersectSet;
}

std::set<int> setsIntersection(std::vector<std::set<int>> & sets){
    set<int> result;
    if(sets.empty()){
        return result;
    }
    if(sets.size() == 1){
        return sets[0];
    }

    result = sets[0];
    set<int> buffer;    
    for(unsigned i = 2; i < sets.size(); i++){
        buffer.clear();

        std::set_intersection(result.begin(), result.end(),
                              sets[i].begin(), sets[i].end(),
                              std::inserter(buffer, buffer.begin()));
        swap(result, buffer);
    }
    return result;
}

std::set<int> setUnion(std::set<int> setA, std::set<int> setB){
    if(setB.empty()){
        return setA;
    } else if(setA.empty()){
        return setB;
    }
    std::set<int> unionSet;
    std::set_union(setA.begin(), setA.end(),
                   setB.begin(), setB.end(),
                   std::inserter(unionSet, unionSet.begin()));
    return unionSet;
}

std::set<int> setsUnion(std::vector<std::set<int>> & sets){
    set<int> result;
    if(sets.empty()){
        return result;
    }
    if(sets.size() == 1){
        return sets[0];
    }

    result = sets[0];
    set<int> buffer;    
    for(unsigned i = 1; i < sets.size(); i++){
        buffer.clear();

        std::set_union(result.begin(), result.end(),
                       sets[i].begin(), sets[i].end(),
                       std::inserter(buffer, buffer.begin()));
        swap(result, buffer);
    }
    return result;
}

bool subsetEq(std::set<int> setA, std::set<int> setB){
    if(setA.empty()){
        return true;
    }
    return std::includes(setB.begin(), setB.end(),
                         setA.begin(), setA.end());
}

bool commonElements(std::set<int> setA, std::set<int> setB){
    return !(setIntersection(setA, setB).empty());
}

void printSet(std::string label, std::set<int> set){
    if(set.empty()){
        cout << label << "{}\n";
        return;
    }
    std::cout << label << "{";
    for(int i : set){
        std::cout << i << ", ";
    }
    std::cout << "}\n";
}

void printList(std::string label, std::vector<std::set<int>> list){
    if(list.empty()){
        cout << label << "{}\n";
        return;
    }
    std::cout << label << "{\t";
    for(set<int> set : list){
        std::cout << "{";
        for(int i : set){
            std::cout << i << ", ";
        }
        std::cout << "},\n\t";
    }
    std::cout << "}\n";
}
