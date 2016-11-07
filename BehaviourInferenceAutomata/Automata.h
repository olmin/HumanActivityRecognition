/* 
 * File:   Automata.h
 * Author: olmin
 *
 * Created on 15 de noviembre de 2012, 11:52
 */

#ifndef AUTOMATA_H
#define	AUTOMATA_H

#include "../Clustering/config.h"

#include <hash_map>

using namespace std;
using namespace __gnu_cxx;

class Automata {
public:
    Automata(hash_map <int, int> alph, int nStates,  vector<vector<int> >  transMat, vector<int> initSts, vector<int> finSts) {
        alphabet=alph;
        numStates=nStates;
        transitionMatrix= transMat;
        initStates= initSts;
        finalStates= finSts;
    }

    void createDotFile(string fileName);
    
    void startAutomata();
    
    bool isSequenceValid(vector<int> sequence);
    
    bool isFinalState();
    
private:
    hash_map <int, int> alphabet;
    int numStates;
    vector<vector<int> >  transitionMatrix;
    vector<int> initStates;
    vector<int> finalStates;
    int actualState;

    int getIndexFromAlphabet(int column);
};

int compareAutomatas(vector<Automata> automs, vector<int> sequence);
        

#endif	/* AUTOMATA_H */

