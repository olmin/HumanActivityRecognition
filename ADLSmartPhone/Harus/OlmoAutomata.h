/* 
 * File:   OlmoAutomata.h
 * Author: olmin
 *
 * Created on 22 de noviembre de 2012, 12:18
 */

#ifndef OLMOAUTOMATA_H
#define	OLMOAUTOMATA_H

#include "config.h"
#include "Common.h"

class OlmoAutomata: public InferenceClassInterface {
public:
    OlmoAutomata(){}
    OlmoAutomata(vector <vector <int> > data);
    
    void startInference(ClusterData clusters, HarusDataSet trainDataSet);
    
    void save();
    
    void read(int idAction);
    
    void checkNewSequence(vector<int> sequence);
    
    double getActualProbability();        
    
    int getIdAction(){
        return idAction;
    };    
    void setIdAction(int id){
        idAction=id;
    }
    void startAutomata();
    
    bool isSequenceValid(vector<int> sequence);  
   
    bool isFinalState();

    void addNewState(int newState);

    
private:
    int idAction;
    
    vector<vector<int> >  transitionMatrix;
    vector< vector<int> > transMatrCopy;//after training will hold the validation of new sequences
    vector<int> initStates;
    vector<int> finalStates;
    vector<int> sequenceWindow;    
    int actualState;
    int actualHits;
    int actualErrors;
    int actualSequenceLength;
    int sequenceLength;
    double minPuntBelong;   
    double maxPuntBelong;  
    double actualBelongRate;
    
    void calcSequenceRating(vector<int> sequence);
};

int compareAutomatas(vector<OlmoAutomata> automs, vector<int> sequence);

#endif	/* OLMOAUTOMATA_H */

