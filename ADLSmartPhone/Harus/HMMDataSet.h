/* 
 * File:   HMMDataSet.h
 * Author: olmin
 *
 * Created on 29 de enero de 2013, 11:51
 */

#ifndef HMMDATASET_H
#define	HMMDATASET_H

#include "SimpleDataSet.h"
#include "config.h"
#include "InferenceClassInterface.h"
#include "ClusterData.h"
#include "ClusterDistribution.h"

class HMMDataSet: public InferenceClassInterface  {
public:
//    HMMDataSet():InferenceClassInterface(){}   
    
    void startInference(ClusterData clusters, HarusDataSet trainDataSet);
    void save(); 
    void read(int idAction);   
            
    void checkNewSequence(vector<int> sequence);
    /*!Returns the probability of the actual execution of the HMM*/
    double getActualProbability() {
        return actualProb;
    }    
    bool isFinalState();

    void addNewState(int newState);  
    
    int getIdAction(){
        return idAction;
    }  
    
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

     /*!Sequence of discrete observations*/
    void setObservations(HarusData *observation);
    
      /*!Sequence of discrete observations*/
    void setObservationsAlpha(vector<int> *observation);
    
     /*! Initializes de HMM: the initialStateDistribution, the stateTransition matrix and the observation probabilistic distribution */
    void initialize(HarusDataSet *dataSeq);
    
    void setStateTransition(HarusDataSet *dataSeq);

    void setInitialStateDist(HarusDataSet *dataSeq);

    void setObsProbDist(HarusDataSet *dataSeq);
   
    void  setInitialForwdVar(vector<float> dataObs);
    
    double iterateForwdVar(vector<float> dataObs,int actualState);
    
//    void  setInitialBackwdVar();
//    
//    double iterateBackwdVar(SimpleSkeleton nextObs, int actualState);

    bool execBaumWelch(vector<int> *observation);   
     
    void setFwdVars(vector<int> *observations);

    void setBackwVars(vector<int> *observations);
    
    //double getGamma(int time, int actualState, list<SimpleSkeleton> *observations);
    vector<vector<double> > getGamma( vector<int> *observations);
    
    //double getEpsilon(int time, int actualState,int nextState, list<SimpleSkeleton> *observations);
    double***getEpsilon(vector<int> *observations);

};

int compareHMM(vector<HMMDataSet> models, vector<int> behSeq) ;

vector<vector<int> > classify(vector<HMMDataSet> models, SimpleDataSet data);

#endif	/* HMMDATASET_H */

