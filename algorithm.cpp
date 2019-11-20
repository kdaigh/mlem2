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


LocalCover Algorithm::induceRules(Concept * concept){
    set<int> B = concept->getBlock();
    set<int> G = B;
    LocalCover lc(concept);
    size_t numOrigBlocks = m_avBlocks.size();

    // WHILE: G is non-empty
    while(!G.empty()){
        Rule rule;
        map<int, set<int>> T_G;

        #if DEBUG == true
            printSet("G = ", G);
        #endif

        // FOR: All attribute-value blocks
        for(unsigned i = 0; i < numOrigBlocks; i++){
            set<int> intersectSet = setIntersection(m_avBlocks[i]->getBlock(), G);
            // IF: Intersection is non-empty
            if(!(intersectSet.empty())){
                T_G[i] = intersectSet;
            }        
        }
        
        // Select conditions for rule
        // WHILE: T is non-empty or T is not subsetEq to B
        while( (rule.empty()) || !(subsetEq(rule.getBlock(m_avBlocks), B)) ){
            int choicePos = getOptimalCondition(T_G);
            rule.addCondition(choicePos);
            T_G.erase(choicePos);
                        
            // Update goal set
            G = setIntersection(m_avBlocks[choicePos]->getBlock(), G);

            // FOR: Relevant attribute-value block intersections
            for(auto const & [i, intersect] : T_G){
                T_G[i] = setIntersection(m_avBlocks[i]->getBlock(), G);
            }
        } // END WHILE (INNER LOOP)

        // Remove unnecessary conditions
        // rule.mergeIntervals(m_avBlocks);
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

int Algorithm::getOptimalCondition(map<int, set<int>> T_G){
    std::list<int> maxSizePos, minCardPos;
    size_t maxSize = 0, minCard = INT_MAX;

    for(auto const & [i, intersect] : T_G){
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

    // IF: Unique largest size is found, return position
    if(maxSizePos.size() == 1){
        return maxSizePos.front();
    }

    // LOOP: For each set intersection in maxSizePos
    for(int i : maxSizePos){
        // IF: Set is non-empty
        if(!(T_G[i].empty())){
            // IF: Current cardinality is smaller than minCard, update minCard
            if(m_avBlocks[i]->size() < minCard){
                minCard = m_avBlocks[i]->size();
                minCardPos.clear();
                minCardPos.push_back(i);
            // IF: Current size is equal to minCard, cardinality is not unique
            } else if (m_avBlocks[i]->size() == minCard){
                minCardPos.push_back(i);
            }
        }
    }
    // RETURN: Position with smallest cardinality or first occuring if tie
    return minCardPos.front();    
}