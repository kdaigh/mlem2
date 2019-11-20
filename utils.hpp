// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Header file for utils.
    @file utils.hpp 
    This class provides project-wide
    utility functions for sets, etc. */

#include <set>
#include <algorithm>
#include <vector>

std::set<int> setDifference(std::set<int> setA, std::set<int> setB);

std::set<int> setIntersection(std::set<int> setA, std::set<int> setB);

std::set<int> setsIntersection(std::vector<std::set<int> > sets);

std::set<int> setUnion(std::set<int> setA, std::set<int> setB);

std::set<int> setsUnion(std::vector<std::set<int> > sets);

bool subsetEq(std::set<int> setA, std::set<int> setB);

bool commonElements(std::set<int> setA, std::set<int> setB);

void printSet(std::string label, std::set<int> set);

void printList(std::string label, std::vector<std::set<int> > list);