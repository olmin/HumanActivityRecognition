/* 
 * File:   HMM.h
 * Author: olmin
 *
 * Created on 4 de noviembre de 2011, 18:15
 */

#ifndef HMM_H
#define	HMM_H

#include "../Clustering/config.h"
#include "../Inference_global/ClusterDistribution.h"
#include "../Clustering/SimpleSkeleton.h"
#include "../Clustering/BehaviourSequence.h"
#include "../Clustering/Commons.h"
#include "../Inference_global/InferenceClassInterface.h"

using namespace std;
using namespace sql;
    
class HMM :public InferenceClassInterface{
public:
    HMM(){        
    };
    /*!Must be initialize with the number of states, the Clusters vector, and the training set of data*/
    HMM(int numStates, vector<SimpleSkeleton> clusters,list<BehaviourSequence> *dataSeq,int id); 
      
    void train(vector<SimpleSkeleton> clusters,list<BehaviourSequence> dataTraining, int id);
    
    /*!Iterates the HMM after the observations*/
//    void setObservations(list<SimpleSkeleton> *observation);

    /*!Sequence of discrete observations*/
    void setObservations(list<SimpleSkeleton> *observation);
    
    bool isSequenceValid(vector<int> *behSeq);
    
    bool isSequenceValid(BehaviourSequence behSequence);
    
      /*!Sequence of discrete observations*/
    void setObservationsAlpha(vector<int> *observation);
            
    /*!Returns the probability of the actual execution of the HMM*/
    double getActualProbability() {
        return actualProb;
    }
    
    void startAutomata();
    
    bool isFinalState();

    void addNewState(int newState);
    void addNewState(SimpleSkeleton newState);

    double getActualRating(){
        return actualProb;
    }  
    void save2DB(); 
    bool readDB(int idAction); 

private:
    int idAction;
    //int numStates;
    /*!actual probability of the execution of the HMM*/
    double actualProb;
    double thresholdBelong;
    
    vector<int> sequenceWindow; 
    int actualSequenceLength=0;
    int sequenceLength;
    
    vector<vector<double> > stateTrans;
    vector<double> initialStateDist;
    vector<vector<double> > obsProbDist;
    vector<ClusterDistribution> clustDist;
    vector<double>  fwdVars;   
    vector<vector<double> > alpha;
    vector<vector<double> > betha;
    vector<double>  scale;    

     /*! Initializes de HMM: the initialStateDistribution, the stateTransition matrix and the observation probabilistic distribution */
    void initialize(list<BehaviourSequence> *dataSeq);
    
    void setStateTransition(list<BehaviourSequence> *dataSeq);

    void setInitialStateDist(list<BehaviourSequence> *dataSeq);

    void setObsProbDist(list<BehaviourSequence> *dataSeq);
   
    void  setInitialForwdVar(SimpleSkeleton dataObs);
    
    double iterateForwdVar(SimpleSkeleton dataObs,int actualState);
    
//    void  setInitialBackwdVar();
//    
//    double iterateBackwdVar(SimpleSkeleton nextObs, int actualState);

    bool execBaumWelch(vector<int> *observation);   
     
    void setFwdVars(vector<int> *observations);

    void setBackwVars(vector<int> *observations);
    
    //double getGamma(int time, int actualState, list<SimpleSkeleton> *observations);
    vector<vector<double> > getGamma( vector<int> *observations);
    
    //double getEpsilon(int time, int actualState,int nextState, list<SimpleSkeleton> *observations);
    vector<vector<vector<double> > >getEpsilon(vector<int> *observations);
       
};

int compareHMM(vector<HMM> models,list<SimpleSkeleton> *behSeq);

int compareHMM(vector<HMM> models,vector<int> behSeq);

// void saveTrainingData(Connection *con, string idsTraining, 
//         int bestErrorTrain, int bestErrorTest,
//         int worstErrorTrain, int worstErrorTest,
//         double meanErrorTrain, double meanErrorTest,
//         vector<int> *errorsTrainByAction, vector<vector<int> >*errorsTrainByActionSel,
//         vector<int> *errorsTestByAction, vector<vector<int> > *errorsTestByActionSel);
// 
  void saveTrainingDataMultiUser(Connection *con, string idsTraining, 
         int bestErrorTrain, int bestErrorTest,
         int worstErrorTrain, int worstErrorTest,
         double meanErrorTrain, double meanErrorTest,
         vector<int> *errorsTrainByAction, vector<vector<int> >*errorsTrainByActionSel,
         vector<int> *errorsTestByAction, vector<vector<int> > *errorsTestByActionSel);

//string getIdsTraining(list<BehaviourSequence> *dataTraining);

//list<BehaviourSequence> getTrainedData(Connection *con,int idAction);

//void saveValidationData(Connection *con, int errors,          
//                 vector<int> * errorsByAction, vector<int> *errorsByActionSel);

  
//InferenceClassInterface *getInferenceObjHMM();
  
#endif	/* HMM_H */

