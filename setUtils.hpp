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
    std::set<int> diffSet;
    std::set_difference(setA.begin(), setA.end(),
                        setB.begin(), setB.end(),
                        std::inserter(diffSet, diffSet.begin()));
    return diffSet;
}

std::vector<std::set<int>> listDifference(std::vector<std::set<int>> listA, std::vector<std::set<int>> listB){
    std::vector<std::set<int>> diffList;
    std::set_difference(listA.begin(), listA.end(),
                        listB.begin(), listB.end(),
                        std::back_inserter(diffList));
    return diffList;
}

std::set<int> setIntersection(std::set<int> setA, std::set<int> setB){
    std::set<int> intersectSet;
    std::set_intersection(setA.begin(), setA.end(),
                        setB.begin(), setB.end(),
                        std::inserter(intersectSet, intersectSet.begin()));
    return intersectSet;
}

std::set<int> subsetIntersection(std::vector<std::set<int>> subsets){
    std::set<int> intersectSet = subsets[0];
    for(unsigned i = 1; i < subsets.size(); i++){
        std::set<int> & result = intersectSet;
        result = setIntersection(result, subsets[i]);
    }
    return intersectSet;
}

std::set<int> setUnion(std::set<int> setA, std::set<int> setB){
    std::set<int> unionSet;
    std::set_union(setA.begin(), setA.end(),
                        setB.begin(), setB.end(),
                        std::inserter(unionSet, unionSet.begin()));
    return unionSet;
}

std::set<int> subsetUnion(std::vector<std::set<int>> subsets){
    std::set<int> unionSet = subsets[0];
    for(unsigned i = 1; i < subsets.size(); i++){
        std::set<int> & result = unionSet;
        result = setUnion(result, subsets[i]);
    }
    return unionSet;
}

bool subsetEq(std::set<int> setA, std::set<int> setB){
    return setDifference(setA, setB).empty();
}

bool commonElements(std::set<int> setA, std::set<int> setB){
    return !(setIntersection(setA, setB).empty());
}

void printSet(std::string label, std::set<int> set){
    if(set.empty()){
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
        return;
    }
    std::cout << label << "{";
    for(set<int> set : list){
        std::cout << "{";
        for(int i : set){
            std::cout << i << ", ";
        }
        std::cout << "}, ";
    }
    std::cout << "}\n";
}
