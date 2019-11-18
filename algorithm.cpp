// Author: Kristi Daigh
// Project: MLEM2 Rule Induction
// Date: 11/20/2019

/** Source file for algorithm class.
    @file algorithm.cpp 

    This class handles the execution
    of the MLEM2 algorithm. */


#include "algorithm.hpp"
#include "setUtils.hpp"
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

            // FOR: Each cutpoint, create (empty) attribute-value blocks 
            for (float c : cutpoints){
                m_avBlocks.push_back(new AVNumeric(attr, col, min, c));
                m_avBlocks.push_back(new AVNumeric(attr, col, c, max));
            }
        }
        // ELSE: Attribute values are symbolic
        else {
            list<string> values = data->getPossibleValues(col);

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

vector<set<int>> Algorithm::induceRules(vector<AV *> av, set<int> B){
    set<int> G = B;
    vector<vector<set<int>>> LC;
    vector<set<int>> LC_indices;
    size_t numOrigBlocks = av.size();

    // WHILE: G is non-empty
    while(!G.empty()){
        vector<set<int>> T;
        set<int> T_indices;
        vector<set<int>> T_G;

        #if DEBUG == true
            printSet("G = ", G);
        #endif
        
        // FOR: All attribute-value blocks
        for(unsigned i = 0; i < numOrigBlocks; i++){
            T_G.push_back(setIntersection(av[i]->getBlock(), G));
        }
        
        // Select conditions for rule
        // WHILE: T is non-empty or T is not subsetEq to B
        while( (T.empty()) || !(subsetEq(setsIntersection(T), B)) ){
            // Find optimal choice; Add it to T
            int choicePos = getOptimalCondition(av, T_G);
            T.push_back(av[choicePos]->getBlock());
            T_indices.insert(choicePos);
                
            // Update goal set
            G = setIntersection(av[choicePos]->getBlock(), G);
                
            // Update intersections
            // FOR: Each attribute-value block
            for(unsigned i = 0; i < numOrigBlocks; i++){
                set<int> coveredCases = setsIntersection(T);
                if(subsetEq(coveredCases, av[i]->getBlock())){
                    T_G[i].clear();
                }
                else {
                    T_G[i] = setIntersection(av[i]->getBlock(), G);
                }
            }
        } // END WHILE (INNER LOOP)

        #if DEBUG == true
            printSet("T_indices = ", T_indices);
            printList("T = ", T);
        #endif

        // Remove unnecessary conditions
        if(T.size() > 1){
            mergeIntervals(T, T_indices);
        }
        if(T.size() > 1){
            dropConditions(T, T_indices, B);
        }

        // Add to local coverings
        if(T.size() > 0){
            LC.push_back(T);
            LC_indices.push_back(T_indices);
        }
        
        // Update goal set
        vector<set<int>> tIntersects; 
        for(vector<set<int>> T : LC){
            tIntersects.push_back(setsIntersection(T));
        }
        #if DEBUG == true
            printSet("G = B - ", setsUnion(tIntersects));
        #endif
        G = setDifference(B, setsUnion(tIntersects));
    } // END WHILE (OUTER LOOP)

    // Remove unnecessary rules
    if(LC.size() > 1){
        dropRules(LC, LC_indices, B);
    }
    return LC_indices;
}

vector<int> Algorithm::classifyRule(set<int> rule, Concept * concept){
    vector<int> ret;
    vector<set<int>> cases;
    
    // FOR: Each attribute in the rule, add the block
    for(int attribute : rule){
        cases.push_back(m_avBlocks[attribute]->getBlock());
    }

    // Intersect all attribute blocks
    set<int> matchLHS = setsIntersection(cases);

    // Intersect all attribute blocks with concept block
    cases.push_back(concept->getBlock());
    set<int> matchCase = setsIntersection(cases);

    ret.push_back(rule.size());         // push specificity
    ret.push_back(matchCase.size());    // push strength
    ret.push_back(matchLHS.size());     // push size

    return ret;
}

std::string Algorithm::generateRuleset(Dataset * data){
    stringstream stream;

    // Generate program components
    generateAVBlocks(data);
    vector <Concept *> concepts = generateConcepts(data);

    // FOR: Each concept, generate rules and print to stream
    for(Concept * concept : concepts){
        #if DEBUG == true
            cout << concept->toString() << endl;
        #endif
        
        vector<set<int>> rules = induceRules(m_avBlocks, concept->getBlock());

        // FOR: Each rule, print to file
        for(set<int> rule : rules){
            vector<int> classification = classifyRule(rule, concept);
            // Print classification to string
            stream << classification.at(0) << ", " << classification.at(1) << ", " << classification.at(2) << endl;

            // Print conditions to string
            unsigned index = 0;
            for(int condition : rule){
                stream << m_avBlocks[condition]->labelString();
                if(index + 1 != rule.size()){
                    stream << " & ";
                }
                index++;
            }

            // Print decision to string
            stream << " -> (" << concept->getDecision() << ", " << concept->getValue() << ")" << endl;
        }
        delete concept;
    }
    return stream.str();
}

int Algorithm::getOptimalCondition(vector<AV *> av, vector<set<int>> T_G){
    std::list<int> maxSizePos, minCardPos;
    size_t maxSize = 0, minCard = INT_MAX;

    // LOOP: For each set intersection
    for(unsigned i = 0; i < T_G.size(); i++){
        // IF: Set is non-empty
        if(!(T_G[i].empty())){
            // IF: Current size is larger than maxSize, clear list and add index
            if(T_G[i].size() > maxSize){
                maxSize = T_G[i].size();
                maxSizePos.clear();
                maxSizePos.push_back(i);
            // IF: Current size is equal to maxSize, add index for consideration
            } else if (T_G[i].size() == maxSize){
                maxSizePos.push_back(i);
            }
        }
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

void Algorithm::mergeIntervals(vector<set<int>> & T, set<int> & T_indices){
    unsigned i = 0;
    // WHILE: There are at least two elements to compare
    while(i < (T.size() - 1)){
        bool increment = true;
        set<int>::iterator curIter = next(T_indices.begin(), i);
        set<int>::iterator nextIter = next(curIter);     
        AV * curBlock = m_avBlocks[(*curIter)];
        AV * nextBlock = m_avBlocks[(*nextIter)];
        // IF: Current and next attribute-value blocks are numeric
        if(curBlock->isNumeric() && nextBlock->isNumeric()){
            // IF: Current and next blocks have same attribute
            if(curBlock->getAttr() == nextBlock->getAttr()){
                int mergedMin = max(curBlock->getMinValue(), nextBlock->getMinValue());
                int mergedMax = min(curBlock->getMaxValue(), nextBlock->getMaxValue());
                // If: Intervals overlap, merge intervals
                if(mergedMax > mergedMin){
                    #if DEBUG == true
                        cout << "Merging intervals to " << mergedMin << ".." << mergedMax << endl;
                    #endif

                    int pos = -1;
                    for(unsigned j = 0; j < m_avBlocks.size(); j++){
                        if(m_avBlocks[j]->getMinValue() == mergedMin && m_avBlocks[j]->getMaxValue() == mergedMax){
                            pos = j;
                            break;
                        }
                    }

                    // IF: Block does not already exist, create one
                    if(pos == -1){
                        pos = m_avBlocks.size();
                        m_avBlocks.push_back(new AVNumeric(curBlock->getAttr(), -1, mergedMin, mergedMax));
                        m_avBlocks[pos]->setBlock(setIntersection(curBlock->getBlock(), nextBlock->getBlock()));
                    }
                    T[i] = m_avBlocks[pos]->getBlock();
                    T = removeCondition(T, i + 1);
                    T_indices.erase((*curIter));
                    T_indices.erase((*nextIter));
                    T_indices.insert(pos);
                    increment = false;
                }
            }
        }
        // IF: Current block is unchanged, consider next
        if(increment){
            i++;
        }
    }
    #if DEBUG == true
        printList("Merge T = ", T);
    #endif
}

void Algorithm::dropConditions(vector<set<int>> & T, set<int> & T_indices, set<int> B){
    for(unsigned i = 0; i < T.size(); i++){
        vector<set<int>> tMinus = removeCondition(T, i);
        if(subsetEq(setsIntersection(tMinus), B)){
            T = tMinus;
            T_indices.erase(i);
        }
    }
}

void Algorithm::dropRules(vector<vector<set<int>>> & LC, vector<set<int>> & LC_indices, set<int> B){
    for(unsigned i = 0; i < LC.size(); i++){
        vector<vector<set<int>>> lcMinus = removeRule(LC, i);

        vector<set<int>> tIntersects; 
        for(vector<set<int>> T : lcMinus){
            tIntersects.push_back(setsIntersection(T));
        }
        if(setsUnion(tIntersects) == B){
            LC = lcMinus;
            LC_indices.erase(LC_indices.begin() + i);
        }
    }
}

vector<set<int>> Algorithm::removeCondition(const vector<set<int>> & T, int index){
    vector<set<int>> temp = T;
    temp.erase(temp.begin() + index);
    return temp;
}

vector<vector<set<int>>> Algorithm::removeRule(const vector<vector<set<int>>> & T, int index){
    vector<vector<set<int>>> temp = T;
    temp.erase(temp.begin() + index);
    return temp;
}