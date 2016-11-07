/* 
 * File:   DTWInference.cpp
 * Author: olmin
 * 
 * Created on 4 de noviembre de 2011, 18:15
 */

#include "DTWInference.h"
#include "DTW.h"

 void DTWInference::save2DB(){
     
 } 
    
bool DTWInference::readDB(int idAction){
    return false;
}
    
void DTWInference::train(vector<SimpleSkeleton> clusters, list<BehaviourSequence> dataTraining, int id) {
    idAction=id;
    //dataTrain.clear();
    seqLength=dataTraining.back().sequence.size();    
    trainLength=dataTraining.size();
    
    //dataTrain.resize(dataTraining.size(), vector< vector< double> >(seqLength, vector<double>(Num_Angles+1, 0)));
      dataTrain = new double**[dataTraining.size()];
    for (int i = 0; i < trainLength; i++) {
        dataTrain[i] = new double*[seqLength];
        for (int j = 0; j < seqLength; j++) {
            dataTrain[i][j] = new double[Num_Angles+1];
        }            
    }
    
    int count=0;
    for (list<BehaviourSequence>::iterator it = dataTraining.begin(); it != dataTraining.end(); it++) {
        int frame=0;
        for(list<SimpleSkeleton>::iterator itSkel = it->skeletons.begin(); itSkel != it->skeletons.end(); itSkel++){            
//           double* dataSkel= new double[Num_Angles+1];
            if(frame==45)
                cout<<"whatsap?"<<endl;
           dataTrain[count][frame][0]=(itSkel->height);
          for(int i=0;i<Num_Angles;i++){
               dataTrain[count][frame][i+1]=(itSkel->angles[i]);
          }
//          memcpy(dataTrain[count][frame],dataSkel, Num_Angles+1);
//          std::copy ( dataSkel, dataSkel+Num_Angles, dataTrain[count][frame]);
          frame++;
        }
        count++;
    }
}

bool DTWInference::isSequenceValid(BehaviourSequence behSeq){    
    
    double min= DBL_MAX;
    //vector< vector <double > > sequence;
     double** sequence= new double*[ behSeq.skeletons.size()]; 
     int thisSeqLength=behSeq.skeletons.size();
     int count=0;
    for(list<SimpleSkeleton>::iterator itSkel = behSeq.skeletons.begin(); itSkel !=behSeq.skeletons.end(); itSkel++){            
//          double* dataSkel= new double[Num_Angles+1];
         sequence[count]=new double[Num_Angles+1];
         sequence[count][0]=(itSkel->height);
          for(int i=0;i<Num_Angles;i++){
              sequence[count][i+1]=(itSkel->angles[i]);
          }
//          memcpy(sequence[count],dataSkel, (Num_Angles+1)*sizeof(double));
//          std::copy ( dataSkel, dataSkel+Num_Angles, sequence[count]);
          count++;
    }
             
    for(int i=0;i<trainLength;i++){                  
        DTWMatching matching= DTWClassic(dataTrain[i], 0, seqLength-1, sequence, 0, thisSeqLength-1);                        
        //DTWMatching matching= DTW.DTWSubsequence(dataTrain[i], 0, seqLength, *behSeq, 0, behSeq->size());           
//        MinimizeMatching(&matching,dataTrain[i],sequence);
        
        if(matching.value<min)
            min=matching.value; 
        
        if(min==0)
            break;
    }  
//    cout<<"min:"<<min<<endl;
    
    if(DistanceMeasureUsed=="Max")
        actualProb=(35.0-min)/35.0;
    if(DistanceMeasureUsed=="Euclidean")
        actualProb=(100.0-min)/100.0;
    if(DistanceMeasureUsed=="CityBlock")
        actualProb=(75.0-min)/75.0;
//    cout<<"Puntuacion:"<<actualProb<<endl;
    return isFinalState(); 
}

void DTWInference::calcActualSequenceRating(){
    double min= DBL_MAX;
    
//    for(int i=0;i<dataTrain.size();i++){                  
//        DTWMatching matching= DTWClassic(dataTrain[i], 0, dataTrain[i].size()-1, actualSequence, 0, actualSequence.size()-1);                        
//        //DTWMatching matching= DTW.DTWSubsequence(dataTrain[i], 0, seqLength, *behSeq, 0, behSeq->size());           
//        
//        if(matching.value<min)
//            min=matching.value;              
//    }  
    if(DistanceMeasureUsed=="Max")
        actualProb=(35.0-min)/35.0;
    if(DistanceMeasureUsed=="Euclidean")
        actualProb=(100.0-min)/100.0;
    if(DistanceMeasureUsed=="CityBlock")
        actualProb=(75.0-min)/75.0;
}

void DTWInference::addNewState(SimpleSkeleton newState) {
    
     vector<double> dataSkel;
          dataSkel.push_back(newState.height);
          for(int i=0;i<Num_Angles;i++){
              dataSkel.push_back(newState.angles[i]);
          }
          
     if(actualSequenceLength<seqLength){        
        actualSequence[actualSequenceLength++]=dataSkel;        
        if(actualSequenceLength==seqLength){          
            //we must start the algorithm           
            calcActualSequenceRating();                                  
        }
    }else{         
        actualSequence.erase(actualSequence.begin());
        actualSequence.push_back(dataSkel);        
        calcActualSequenceRating(); 
    }     

}

void DTWInference::startAutomata() {
    actualProb=0;
    actualSequenceLength=0;
    actualSequence.clear();
    actualSequence.resize(seqLength, vector<double>(Num_Angles+1,-1));  
}

bool DTWInference::isFinalState() {
//        cout<<"Final?:"<<actualProb<<endl;
    return actualProb >= ThresholdBelong;
}



