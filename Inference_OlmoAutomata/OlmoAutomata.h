/* 
 * 
 * 
 * File:   OlmoAutomata.h
 * Author: olmin
 *
 * Created on 22 de noviembre de 2012, 12:18
 */

#ifndef OLMOAUTOMATA_H
#define	OLMOAUTOMATA_H

#include "../Clustering/config.h"
#include "../Clustering/Commons.h"
#include "../Inference_global/InferenceClassInterface.h"

class OlmoAutomata: public InferenceClassInterface {
public:
    
    OlmoAutomata(){
    };
    
    void train(vector <vector <int> > data,int idAction);
    
//    OlmoAutomata(vector <vector <int> > data);
    
    void train(vector<SimpleSkeleton> clusters,list<BehaviourSequence> dataTraining,int idAction);
     
    void startAutomata();
    
    bool isSequenceValid(vector<int> *sequence);  
    
    bool isSequenceValid(BehaviourSequence behSequence);
   
    bool isFinalState();

    void addNewState(int newState);
    void addNewState(SimpleSkeleton newState);

    double getActualRating();
    
    void save2DB(); 
    bool readDB(int idAction); 
private:
    int idAction;
    vector<vector<int> >  transitionMatrix;
    vector< vector<int> > transMatrCopy;//after training will hold the validation of new sequences
    vector< vector<int> > stateMatrix;
//    vector<int> initStates;
//    vector<int> finalStates;
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

  void saveOlmoTrainingData(Connection *con, string idsTraining, 
         int bestErrorTrain, int bestErrorTest,
         int worstErrorTrain, int worstErrorTest,
         double meanErrorTrain, double meanErrorTest,
         vector<int> *errorsTrainByAction, vector<vector<int> >*errorsTrainByActionSel,
         vector<int> *errorsTestByAction, vector<vector<int> > *errorsTestByActionSel);

#endif	/* OLMOAUTOMATA_H */

