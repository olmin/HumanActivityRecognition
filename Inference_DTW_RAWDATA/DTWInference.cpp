/* 
 * File:   DTWInference.cpp
 * Author: olmin
 * 
 * Created on 4 de noviembre de 2011, 18:15
 */

#include "DTWInference.h"
#include "DTW.h"
#include "Skeleton.h"

void DTWInference::train( list<SkeletonSequence> dataTraining) {
    
  
    seqLength=dataTraining.front().sequence.size();    
    
//    dataTrain= new double[dataTraining.size()][seqLength][15][3];   
    trainLength=dataTraining.size();

    dataTrain = new double***[dataTraining.size()];
    for (int i = 0; i < dataTraining.size(); i++) {
        dataTrain[i] = new double**[seqLength];
        for (int j = 0; j < seqLength; j++) {
            dataTrain[i][j] = new double*[15];
            for (int k = 0; k < 15; k++)
                dataTrain[i][j][k] = new double[3];
        }
    }
    
    int count=0;
    for (list<SkeletonSequence>::iterator it = dataTraining.begin(); it != dataTraining.end(); it++) {
        int frame=0;
        for(vector<Skeleton>::iterator itSkel = it->sequence.begin(); itSkel != it->sequence.end(); itSkel++){               
          for(int i=0;i<15;i++)
            dataTrain[count][frame][i]=itSkel->joints[i];
//          if(count==5 and frame==1)
//                cout<< dataTrain[5][1][0]<< endl;
          frame++;
        }
        count++;
    }
//      cout<< "what? "<< dataTrain[5][1][0] <<endl;
}

bool DTWInference::isSequenceValid(SkeletonSequence behSeq){    
    
    double min= DBL_MAX;
    double*** sequence= new double**[behSeq.sequence.size()];   
    int count=0;
    for(vector<Skeleton>::iterator itSkel = behSeq.sequence.begin(); itSkel !=behSeq.sequence.end(); itSkel++){ 
        sequence[count]= new double*[15];
        for(int i=0;i<15;i++){
             sequence[count][i]= new double[3];
            for(int j=0;j<3;j++)
                  sequence[count][i][j]=itSkel->joints[i][j];
        }
        count++;
    }
             
//    cout<< "Data train:"<<sizeof(dataTrain)<<endl;
    
    for(int i=0;i<trainLength;i++){   
        //DTWMatching matching= DTWClassic(dataTrain[i], 0, seqLength-1, sequence, 0, count-1);                        
//        DTWMatching matching= DTWSubsequence(dataTrain[i], 0, seqLength, sequence, 0, sequence.size());           
//        MinimizeMatching(&matching,dataTrain[i],sequence);
        
        DTWMatching matching;
        matching.DTWClassic(dataTrain[i], 0, seqLength-1, sequence, 0, count-1);
            if(matching.value<min)
                min=matching.value; 
       
        if(min==0)
            break;
    }  
    delete [] sequence;
    
//    cout<<"min:"<<min<<endl;
    
//    if(DistanceMeasureUsed=="Max")
//        actualProb=(250000.0-min)/250000.0;
//    if(DistanceMeasureUsed=="Euclidean")
//         actualProb=(350000.0-min)/350000.0;
////        actualProb=(450000.0-min)/450000.0;
//    if(DistanceMeasureUsed=="CityBlock")
//        actualProb=(1.5e+06  -min)/1.5e+06;
    actualProb=min;
    
//    cout<<"Puntuacion:"<<actualProb<<endl;
    return true;//isFinalState(); 
}

void DTWInference::calcActualSequenceRating(){
    double min= DBL_MAX;
//    
//    for(int i=0;i<dataTrain.size();i++){                  
//        DTWMatching matching= DTWClassic(&dataTrain[i], 0, dataTrain[i].size()-1, &actualSequence, 0, actualSequence.size()-1);                        
//        //DTWMatching matching= DTW.DTWSubsequence(dataTrain[i], 0, seqLength, *behSeq, 0, behSeq->size());           
//        
//        if(matching.value<min)
//            min=matching.value;              
//    }  
//    if(DistanceMeasureUsed=="Max")
//        actualProb=(35.0-min)/35.0;
//    if(DistanceMeasureUsed=="Euclidean")
//        actualProb=(100.0-min)/100.0;
//    if(DistanceMeasureUsed=="CityBlock")
//        actualProb=(75.0-min)/75.0;
}

void DTWInference::addNewState(Skeleton newState) {
    
//     vector<vector<double> > dataSkel=newState.joints;         
          
//     if(actualSequenceLength<seqLength){        
//        actualSequence[actualSequenceLength++]=dataSkel;        
//        if(actualSequenceLength==seqLength){          
//            //we must start the algorithm           
//            calcActualSequenceRating();                                  
//        }
//    }else{         
//        actualSequence.erase(actualSequence.begin());
//        actualSequence.push_back(dataSkel);        
//        calcActualSequenceRating(); 
//    }     

}

void DTWInference::startAutomata() {
    actualProb=0;
    actualSequenceLength=0;
//    actualSequence.clear();
//    actualSequence.resize(seqLength);//, vector<vector<double> >(14+1,vec3(0,0,0)));  
}

bool DTWInference::isFinalState() {
//        cout<<"Final?:"<<actualProb<<endl;
    return actualProb >= ThresholdBelong;
}



int compareModels(vector<DTWInference> models, SkeletonSequence behSeq){
    int selAction=-1;
    double best=DBL_MAX;

    for (int i = 0; i < models.size(); i++) {
        if (models[i].isSequenceValid(behSeq)) {
            double count = models[i].getActualRating();            
            if (selAction == -1 || count < best) {
                best = count;
                selAction = i;
            }
        }      
    }

    return selAction;
}

void DTWInference::copyModel(DTWInference *copy){
    copy->actualProb=actualProb;
    copy->actualSequenceLength= actualSequenceLength;
    copy->seqLength=seqLength;
    copy->trainLength=trainLength;
    
    copy->dataTrain = new double***[trainLength];
//    for (int i = 0; i < trainLength; i++) {
//          copy->dataTrain[i] = new double**[seqLength];
//        for (int j = 0; j < seqLength; j++) {
//              copy->dataTrain[i][j] = new double*[15];
//            for (int k = 0; k < 15; k++)
//                  copy->dataTrain[i][j][k] = new double[3];
//        }
//    }
     for (int i = 0; i <trainLength; i++) {    
          copy->dataTrain[i] = new double**[seqLength];
        for (int j = 0; j < seqLength; j++) {   
             copy->dataTrain[i][j] = new double*[15];
            for (int k = 0; k < 15; k++){
                 copy->dataTrain[i][j][k] = new double[3];            
                for (int l = 0; l < 3; l++)
                  copy->dataTrain[i][j][k][l] = dataTrain[i][j][k][l];
            }
        }
    }
}

  void copyModels(vector<DTWInference> *copy, vector<DTWInference> *origin){
      copy->resize(origin->size());
      
       for (int i = 0; i < origin->size(); i++) {
//           DTWInference obj;          
           (*origin)[i].copyModel(&(*copy)[i]);           
//           (*copy)[i]= obj;
       }      
  }