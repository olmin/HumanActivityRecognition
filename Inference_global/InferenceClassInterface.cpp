/* 
 * File:   InferenceObjInterface.cpp
 * Author: olmin
 * 
 * Created on 31 de octubre de 2013, 14:11
 */

#include "InferenceClassInterface.h"
#include "../Clustering/Commons.h"
 
 //InferenceClassInterface *getInferenceObject(){
//    
//    InferenceClassInterface *inferenceObj;
//      if(InferenceClassUsed =="HMM"){
//            inferenceObj= new HMM;                
//      }        
//    return inferenceObj;
//}
double minDist2correct=DBL_MAX;

int compareModels(vector<InferenceClassInterface*> models, BehaviourSequence behSeq){
    int selAction=-1;
    double best=DBL_MIN;

    for (int i = 0; i < models.size(); i++) {
        if (models[i]->isSequenceValid(behSeq)) {
            double count = models[i]->getActualRating();            
            if (selAction == -1 || count > best) {          
                best = count;
                selAction = i;
                
            }
        }      
    }
      
//   if(behSeq.idAction!= selAction){       
////       if(models[behSeq.idAction]->getActualRating()>maxDist2correct){
////           maxDist2correct=models[behSeq.idAction]->getActualRating();
//           cout<<"Selected:"<<selAction<<", instead of: "<<behSeq.idAction<<",    with score: "<<best <<"   instead of  :"<<models[behSeq.idAction]->getActualRating() <<endl;
////           cout<<"              MAXDIST: "<<maxDist2correct<<endl;
////       }
//   }
    return selAction;
}

void saveSubactivityResult(int fold, int train_test_type, int  act_id_real, string subact_name_detected, 
        double distance_grade){//, int start_frame, int  end_frame){
     
    PreparedStatement *prep_stmt;    
    
    prep_stmt = con -> prepareStatement("INSERT INTO `SubactivityResuts`"
            "(`fold`, `train_test_type`, `subact_id_real`, `subact_name_detected`, `distance_grade`)"
            " VALUES (?,?,?,?,?)");
    
    prep_stmt -> setInt(1, fold);
    prep_stmt -> setInt(2, train_test_type);
    prep_stmt -> setInt(3, act_id_real);  
    prep_stmt -> setString(4, subact_name_detected);  
    prep_stmt -> setDouble(5, distance_grade);  
//    prep_stmt -> setInt(6, start_frame);
//    prep_stmt -> setInt(7, end_frame);
    
    prep_stmt->executeUpdate();
    
    delete prep_stmt;
    
}

int compareModels(vector<InferenceClassInterface*> models, BehaviourSequence behSeq, int fold, int trainOrTest){
    int selAction=-1;
    double best=DBL_MIN;
       
//    connectSQL();
    
    for (int i = 0; i < models.size(); i++) {
        if (models[i]->isSequenceValid(behSeq)) {
            double count = models[i]->getActualRating();            
            if (selAction == -1 || count > best) {          
                best = count;
                selAction = i;                
            }
//            if(count>0)
//                saveSubactivityResult(fold, trainOrTest, behSeq.idBehaviour,  listActions[i], count);
        }      
    }
//   if(behSeq.idAction== 9 ){       
////       if(models[behSeq.idAction]->getActualRating()<minDist2correct){
////           minDist2correct=models[behSeq.idAction]->getActualRating();
//           cout<<"Selected:"<<selAction<<", instead of: "<<behSeq.idAction<<",    with score: "<<best <<"   instead of  :"<<models[behSeq.idAction]->getActualRating() <<endl;
////           cout<<"              MINDIST: "<<minDist2correct<<endl;
////       }
//   }    
    
//    disconnectSQL();
    
    return selAction;
}
 
 
//int compareModels(vector<InferenceClassInterface*> models, vector<int> *behSeq){
//    int selAction=-1;
//    double best=-100;
//
//    for (int i = 0; i < models.size(); i++) {
//        if (models[i]->isSequenceValid(behSeq)) {
//            double count = models[i]->getActualRating();
//            if (selAction == -1 || count > best) {
//                best = count;
//                selAction = i;
//            }
//        }      
//    }
//
//    return selAction;
//}

void copyModels(vector<InferenceClassInterface*>& copy, vector<InferenceClassInterface*>& origin) {
    copy.resize(origin.size());

    for (int i = 0; i < origin.size(); i++) {
        InferenceClassInterface *obj = getInferenceModel();
        origin[i]->copyModel( obj);
       copy[i] = obj;
    }
}