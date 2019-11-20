// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Source file for algorithm class.
    @file algorithm.cpp 

    This class handles the execution
    of the MLEM2 algorithm. */


#include "algorithm.hpp"
#include <algorithm>
#include <limits.h>

using namespace std;

Algorithm::Algorithm(std::size_t numAttributes)
    : m_numAttributes(numAttributes){}

void Algorithm::generateAVBlocks(Dataset * data){
    // FOR: Each attribute (column)
    for(unsigned col = 0; col < m_numAttributes; col++){
        string attr = data->getAttribute(col);

        // IF: Attribute values are numeric
        if(data->getValue(1, col)->isNumeric()){
            float min = 0, max = 0;
            list<float> cutpoints = data->discretize(col, min, max);
            #if DEBUG==true
                cout << "NUMERIC: Cutpoints for " << attr << ": ";
                for(float c : cutpoints){
                    cout << c << ", ";
                }
                cout << endl;
            #endif

            // FOR: Each cutpoint, create (empty) attribute-value blocks 
            for (float c : cutpoints){
                m_avBlocks.push_back(new AVNumeric(attr, col, min, c));
                m_avBlocks.push_back(new AVNumeric(attr, col, c, max));
            }
        }
        // ELSE: Attribute values are symbolic
        else {
            list<string> values = data->getPossibleValues(col);
            #if DEBUG==true
                cout << "SYMBOLIC: Possible values for " << attr << ": ";
                for(string str : values){
                    cout << str << ", ";
                }
                cout << endl;
            #endif

            // FOR: Each value, create an (empty) attribute-value block
            for (string v : values){
                m_avBlocks.push_back(new AVSymbolic(attr, col, v));
            }
        }
    } // END FOR

    // Populate attribute-value blocks
    // LOOP: For each attribute-value block
    for(unsigned i = 0; i < m_avBlocks.size(); i++){

        // LOOP: For each case (row), add matching values to the block
        for(unsigned r = 1; r <= data->getNumCases(); r++){
            int c = m_avBlocks[i]->getAttrCol();
            
            m_avBlocks[i]->addOnMatch(data->getValue(r, c), r);    
        }

        #if DEBUG == true
           m_avBlocks[i]->print();
        #endif
    }
}

vector<Concept *> Algorithm::generateConcepts(Dataset * data){
    vector <Concept *> concepts;
    list<string> values = data->getPossibleValues(m_numAttributes);
    for(string v : values){
        concepts.push_back(new Concept(data->getDecision(), v));
    }

    // FOR: Each concept
    for(unsigned i = 0; i < concepts.size(); i++){
        // FOR: Each case (row), add matching values to the block
        for(unsigned r = 1; r <= data->getNumCases(); r++){
            int c = m_numAttributes;
            string cellValue = data->getValue(r, c)->getStrValue();
            if(concepts[i]->getValue() == cellValue){
                concepts[i]->addCase(r);
            }
        }
    }
    return concepts;
}

LocalCover Algorithm::induceRules(Concept * concept){
    set<int> B = concept->getBlock();
    set<int> G = B;
    LocalCover lc(concept);
    size_t numOrigBlocks = m_avBlocks.size();

    // WHILE: G is non-empty
    while(!G.empty()){
        Rule rule;
        vector<set<int>> T_G;
        set<int> T_G_indices;

        #if DEBUG == true
            printSet("G = ", G);
        #endif
        
        // FOR: All attribute-value blocks
        for(unsigned i = 0; i < numOrigBlocks; i++){
            set<int> intersectSet = setIntersection(m_avBlocks[i]->getBlock(), G);
            // IF: Intersection is non-empty
            if(!(intersectSet.empty())){
                T_G_indices.insert(i);
                T_G.push_back(setIntersection(m_avBlocks[i]->getBlock(), G));
            }        
        }
        
        // Select conditions for rule
        // WHILE: T is non-empty or T is not subsetEq to B
        while( (rule.empty()) || !(subsetEq(rule.getBlock(m_avBlocks), B)) ){
            // Find optimal choice; Add it to T
            int choicePos = getOptimalCondition(m_avBlocks, T_G, T_G_indices);
            rule.addCondition(choicePos);
                
            // Update goal set
            G = setIntersection(m_avBlocks[choicePos]->getBlock(), G);
        
            // Update candidates
            T_G_indices = setDifference(T_G_indices, rule.m_conditions);

            // FOR: Relevant attribute-value blocks
            int setIndex = 0;
            for(int i : T_G_indices){
                set<int> intersectSet = setIntersection(m_avBlocks[i]->getBlock(), G);
                // IF: Pair is not in T
                if(rule.containsCondition(i)){
                    T_G[setIndex] = setIntersection(m_avBlocks[i]->getBlock(), G);
                } 
                // ELSE: Pair is in T
                else {
                    T_G.erase(T_G.begin(), T_G.begin() + setIndex);
                }
                setIndex++;
            }
        } // END WHILE (INNER LOOP)

        // Remove unnecessary conditions
        rule.mergeIntervals(m_avBlocks);
        rule.dropConditions(m_avBlocks, B);

        // Add to local covering
        lc.addRule(new Rule(rule));
        
        // Update goal set
        G = setDifference(B, lc.getCoveredConditions(m_avBlocks));
    } // END WHILE (OUTER LOOP)

    // Remove unnecessary rules
    lc.dropRules(m_avBlocks, B);

    return lc;
}

// vector<int> Algorithm::classifyRule(Rule * rule, Concept * concept){
//     vector<int> ret;
//     vector<set<int>> cases;
    
//     // Intersect all attribute blocks
//     set<int> matchLHS = rule->getBlock(m_avBlocks);

//     // Intersect all attribute blocks with concept block
//     set<int> matchCase = setIntersection(rule->getBlock(m_avBlocks), concept->getBlock());

//     ret.push_back(rule->size());         // push specificity
//     ret.push_back(matchCase.size());    // push strength
//     ret.push_back(matchLHS.size());     // push size

//     return ret;
// }

void Algorithm::generateRuleset(ostream & file, Dataset * data){
    // Generate program components
    generateAVBlocks(data);
    vector <Concept *> concepts = generateConcepts(data);

    // FOR: Each concept, generate rules and print to stream
    for(Concept * concept : concepts){
        #if DEBUG == true
            cout << concept->toString() << endl;
        #endif
        
        LocalCover rules = induceRules(concept);
        file << rules.toString(m_avBlocks);

        delete concept;
    }
}

int Algorithm::getOptimalCondition(vector<AV *> av, vector<set<int>> T_G, set<int> T_G_indices){
    std::list<int> maxSizePos, minCardPos;
    size_t maxSize = 0, minCard = INT_MAX;

    int setIndex = 0;
    // LOOP: For each AV-block index
    for(int i : T_G_indices){
        // IF: Set is non-empty
        if(!(T_G[setIndex].empty())){
            // IF: Current size is larger than maxSize, clear list and add index
            if(T_G[setIndex].size() > maxSize){
                maxSize = T_G[setIndex].size();
                maxSizePos.clear();
                maxSizePos.push_back(i);
            // IF: Current size is equal to maxSize, add index for consideration
            } else if (T_G[setIndex].size() == maxSize){
                maxSizePos.push_back(i);
            }
        }
        setIndex++;
    }

    // IF: Unique largest size is found, return position
    if(maxSizePos.size() == 1){
        return maxSizePos.front();
    }

    // LOOP: For each set intersection in maxSizePos
    for(int i : maxSizePos){
        // IF: Set is non-empty
        if(!(T_G[i].empty())){
            // IF: Current cardinality is smaller than minCard, update minCard
            if(av[i]->size() < minCard){
                minCard = av[i]->size();
                minCardPos.clear();
                minCardPos.push_back(i);
            // IF: Current size is equal to minCard, cardinality is not unique
            } else if (av[i]->size() == minCard){
                minCardPos.push_back(i);
            }
        }
    }
    // RETURN: Position with smallest cardinality or first occuring if tie
    return minCardPos.front();    
}

// void Algorithm::mergeIntervals(vector<set<int>> & T, set<int> & T_indices){
//     unsigned i = 0;
//     // WHILE: There are at least two elements to compare
//     while(i < (T.size() - 1)){
//         cout << "index: " << i << endl;
//         cout << "T = {";
//         for(int k : T_indices){
//             cout << m_avBlocks[k]->labelString() << ", ";
//         }
//         cout << "}\n";
//         bool increment = true;
//         set<int>::iterator curIter = next(T_indices.begin(), i);
//         set<int>::iterator nextIter = next(curIter);     
//         AV * curBlock = m_avBlocks[(*curIter)];
//         AV * nextBlock = m_avBlocks[(*nextIter)];
//         // IF: Current and next attribute-value blocks are numeric
//         if(curBlock->isNumeric() && nextBlock->isNumeric()){
//             // IF: Current and next blocks have same attribute
//             cout << curBlock->getAttr() << " == " << nextBlock->getAttr() << "?\n";
//             if(curBlock->getAttr() == nextBlock->getAttr()){
//                 int mergedMin = max(curBlock->getMinValue(), nextBlock->getMinValue());
//                 int mergedMax = min(curBlock->getMaxValue(), nextBlock->getMaxValue());
//                 // If: Intervals overlap, merge intervals
//                 if(mergedMax > mergedMin){
//                     #if DEBUG == true
//                         cout << "Merging blocks " << curBlock->labelString() << " & " << nextBlock->labelString() << endl;
//                     #endif

//                     int pos = -1;
//                     for(unsigned j = 0; j < m_avBlocks.size(); j++){
//                         if(m_avBlocks[j]->getMinValue() == mergedMin && m_avBlocks[j]->getMaxValue() == mergedMax){
//                             pos = j;

//                             #if DEBUG==true
//                                 cout << "Found merged block @ " << j << endl;
//                             #endif

//                             break;
//                         }
//                     }

//                     // IF: Block does not already exist, create one
//                     if(pos == -1){
//                         pos = m_avBlocks.size();
//                         m_avBlocks.push_back(new AVNumeric(curBlock->getAttr(), -1, mergedMin, mergedMax));
//                         m_avBlocks[pos]->setBlock(setIntersection(curBlock->getBlock(), nextBlock->getBlock()));
//                         #if DEBUG==true
//                             cout << "Created new block for " << m_avBlocks[pos]->labelString() << " @ " << pos << endl;
//                         #endif
//                     }
//                     T = removeCondition(T, i + 1);
//                     T[i] = m_avBlocks[pos]->getBlock();    
//                     T_indices.erase((*curIter));
//                     T_indices.erase((*nextIter));
//                     T_indices.insert(pos);
//                     increment = false;
//                 }
//             }
//         }
//         // IF: Current block is unchanged, consider next
//         if(increment){
//             i++;
//         }
//     }
// }

// void Algorithm::dropConditions(vector<set<int>> & T, set<int> & T_indices, set<int> B){
//     for(unsigned i = 0; i < T.size(); i++){
//         vector<set<int>> tMinus = removeCondition(T, i);
//         if(subsetEq(setsIntersection(tMinus), B)){
//             T = tMinus;
//             T_indices.erase(i);
//         }
//     }
// }

// void Algorithm::dropRules(vector<vector<set<int>>> & LC, vector<set<int>> & LC_indices, set<int> B){
//     for(unsigned i = 0; i < LC.size(); i++){
//         vector<vector<set<int>>> lcMinus = removeRule(LC, i);

//         vector<set<int>> tIntersects; 
//         for(vector<set<int>> T : lcMinus){
//             tIntersects.push_back(setsIntersection(T));
//         }
//         if(setsUnion(tIntersects) == B){
//             LC = lcMinus;
//             LC_indices.erase(LC_indices.begin() + i);
//         }
//     }
// }

// vector<set<int>> Algorithm::removeCondition(const vector<set<int>> & T, int index){
//     vector<set<int>> temp = T;
//     temp.erase(temp.begin() + index);
//     return temp;
// }

// vector<vector<set<int>>> Algorithm::removeRule(const vector<vector<set<int>>> & T, int index){
//     vector<vector<set<int>>> temp = T;
//     temp.erase(temp.begin() + index);
//     return temp;
// }