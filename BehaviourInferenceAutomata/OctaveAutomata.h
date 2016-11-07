/* 
 * File:   OctaveAutomata.h
 * Author: olmin
 *
 * Created on 15 de noviembre de 2012, 12:44
 */

#ifndef OCTAVEAUTOMATA_H
#define	OCTAVEAUTOMATA_H

#include "../Clustering/config.h"

#include <octave/oct.h>
#include <octave/octave.h>
#include <octave/parse.h>
#include <octave/toplev.h> /* do_octave_atexit */
#include <octave/ov-struct.h>
#include <octave/oct-map.h>

using namespace std;
using namespace __gnu_cxx;

class OctaveAutomata {
public:    
    OctaveAutomata(vector<vector<int> > inData, int k);
    OctaveAutomata(string inData, int k);
    hash_map <int, int> getAlphabet();
    int getNumStates();
    vector<vector<int> >  getTransitionMatrix();
    vector<int> getInitStates();
    vector<int> getFinalStates();
    
private:
    NDArray finiteStates;     
    octave_value_list alphabets;
    Matrix transitionMatrix;
    NDArray initState ;    
    NDArray acceptedStates;
    
    void initAutomata(string inData,int k);
};

#endif	/* OCTAVEAUTOMATA_H */

