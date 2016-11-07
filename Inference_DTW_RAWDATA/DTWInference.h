/* 
 * File:   DTWInference.h
 * Author: olmin
 *
 * Created on 4 de noviembre de 2011, 18:15
 */

#ifndef DTWInference_H
#define	DTWInference_H

#include "../Clustering/config.h"

#include "../Clustering/Commons.h"
//#include "../Inference_global/InferenceClassInterface.h"
//#include "../Clustering/vec3.h"
#include "Skeleton.h"

using namespace std;
using namespace sql;
    
class DTWInference {
public:
    DTWInference(){
    };
   
//    ~DTWInference(){        
//        for (int i = 0; i < sizeof(dataTrain); i++){
//            for (int j = 0; j < seqLength; j++){
//                    for (int k = 0; k < 15; k++){
//                            delete[] dataTrain[i][j][k];
//                    }
//                    delete[] dataTrain[i][j];
//            }
//            delete[] dataTrain[i];
//        }
//           delete[] dataTrain;
//    };
//    
    void train(list<SkeletonSequence> dataTraining);   
    
    bool isSequenceValid(SkeletonSequence behSeq);

    void startAutomata();
    
    bool isFinalState();

    void addNewState(Skeleton newState);

    double getActualRating(){
        return actualProb;
    };
    
    void calcActualSequenceRating();
    
    void copyModel(DTWInference *copy);
    
private:
    double actualProb;
    int seqLength;
    int actualSequenceLength;
//    vector< vector<vector<double> > > actualSequence;
    double**** dataTrain;
    int trainLength;
};


  int compareModels(vector<DTWInference> models, SkeletonSequence behSeq);
  
  void copyModels(vector<DTWInference> *copy, vector<DTWInference> *origin);
  
#endif	/* DTWInference_H */

