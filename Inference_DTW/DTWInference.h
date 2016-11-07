/* 
 * File:   DTWInference.h
 * Author: olmin
 *
 * Created on 4 de noviembre de 2011, 18:15
 */

#ifndef DTWInference_H
#define	DTWInference_H

#include "../Clustering/config.h"
#include "../Inference_global/ClusterDistribution.h"
#include "../Clustering/SimpleSkeleton.h"
#include "../Clustering/BehaviourSequence.h"
#include "../Clustering/Commons.h"
#include "../Inference_global/InferenceClassInterface.h"

using namespace std;
using namespace sql;
    
class DTWInference :public InferenceClassInterface{
public:
    
    DTWInference(){
    };
   
    void train(vector<SimpleSkeleton> clusters,list<BehaviourSequence> dataTraining, int idAction);   
    
    bool isSequenceValid(BehaviourSequence behSeq);

    void startAutomata();
    
    bool isFinalState();

    void addNewState(SimpleSkeleton newState);

    double getActualRating(){
        return actualProb;
    };
    
    void calcActualSequenceRating();
    
    void save2DB(); 
    
    bool readDB(int idAction); 
    
private:
    int idAction;
    double actualProb;
    int seqLength;
    int actualSequenceLength;
    vector< vector<double> > actualSequence;
    //vector< vector< vector<double> > > dataTrain;
    int trainLength;
     double*** dataTrain;
};


  
#endif	/* DTWInference_H */

