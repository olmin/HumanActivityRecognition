/* 
 * File:   InferenceObjInterface.h
 * Author: olmin
 *
 * Created on 31 de octubre de 2013, 14:11
 */

#ifndef INFERENCECLASSINTERFACE_H
#define	INFERENCECLASSINTERFACE_H

#include "../Clustering/config.h"
#include "../Clustering/BehaviourSequence.h"

class InferenceClassInterface {
public:   
    
   virtual ~InferenceClassInterface(){}; 
      
   virtual void train(vector<SimpleSkeleton> clusters, list<BehaviourSequence>& dataTraining, int idAction)=0;
   
   virtual void startAutomata()=0;
    
   virtual bool isFinalState()=0;

//   virtual bool isSequenceValid(vector<int> *sequence)=0;
   
   virtual bool isSequenceValid(BehaviourSequence behSequence)=0;
   
   virtual void addNewState(SimpleSkeleton newState)=0;

   virtual double getActualRating()=0;       
   
   virtual void save2DB()=0; 
   virtual bool readDB(int idAction)=0; 
   
   virtual  void copyModel(InferenceClassInterface *copy)=0;   
  
};

InferenceClassInterface *getInferenceModel();

int compareModels(vector<InferenceClassInterface*> models, BehaviourSequence behSeq);

int compareModels(vector<InferenceClassInterface*> models, BehaviourSequence behSeq, int fold, int trainOrTest);

void copyModels(vector<InferenceClassInterface*>& copy, vector<InferenceClassInterface*>& origin) ;

#endif	/* INFERENCECLASSINTERFACE_H */

