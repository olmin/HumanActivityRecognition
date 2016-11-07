/* 
 * File:   OlmoAutomata.cpp
 * Author: olmin
 * 
 * Created on 22 de noviembre de 2012, 12:18
 */

#include "OlmoAutomata.h"


void OlmoAutomata::train(vector<SimpleSkeleton> clusters,list<BehaviourSequence> dataTraining,int id){   
    vector <vector <int> > data;
    for (list<BehaviourSequence>::iterator it = dataTraining.begin(); it != dataTraining.end(); it++) {
        data.push_back(it->sequence);
    }
    train(data,id);
}

 void OlmoAutomata::train(vector <vector <int> > data,int id){    
    idAction=id;
    transitionMatrix.resize(numStates, vector<int>(numStates,0));
//    initStates.resize(numStates,0);
//    finalStates.resize(numStates,0);
    sequenceLength=data[2].size();
    sequenceWindow.resize(sequenceLength,-1);       
    actualHits=0;
    actualErrors=0;
    actualSequenceLength=0;
    
    stateMatrix.resize(sequenceLength, vector<int>(numStates,0));
    
    vector <vector <int> > countMat;
     
    for(int i=0; i< data.size();i++){
        int lastState;
        countMat.clear();
        countMat.resize(numStates, vector<int>(numStates,0));
        
        for(int j=0; j< data[i].size();j++){
            int state= data[i][j];  
            
            stateMatrix[j][state]=1;        
    
//            if(j==0){
//                //first state
//                initStates[state]=1;
//            }else{
             if(j!=0){   
//                if(j==data[i].size()-1){
//                    //final state
//                    finalStates[state]=1;
//                }
                //transition from states
                countMat[lastState][state]++;
            }
            
            lastState=state;
        }
        //updating general matrix
        for (int k = 0; k < numStates; k++) {
            for (int m = 0; m < numStates; m++) {
                if (countMat[k][m] > transitionMatrix[k][m]) {
                    transitionMatrix[k][m] = countMat[k][m];                    
                }
            }
        }
    }
//    minPuntBelong=1000;
//    maxPuntBelong=0;
//   for(int i=0; i< data.size();i++){
        minPuntBelong=1;
      calcSequenceRating(data[2]);
       minPuntBelong=actualBelongRate;
//       if(actualBelongRate<minPuntBelong)
//           minPuntBelong=actualBelongRate;
//       if(actualBelongRate>maxPuntBelong)
//           maxPuntBelong=actualBelongRate;
//           
//   }
//  cout<<"Max,min:"<<maxPuntBelong<<", "<<minPuntBelong<<", seqLenght:"<<sequenceLength<<endl;   
    startAutomata();
 }
 
//OlmoAutomata::OlmoAutomata(vector <vector<int> > data) {
//    train(data); 
// 
//}


void OlmoAutomata::startAutomata(){   
    actualHits=0;
    actualErrors=0;
    actualSequenceLength=0;
    actualBelongRate=-1000;
    sequenceWindow.clear();
    sequenceWindow.resize(sequenceLength,-1);  
}


double OlmoAutomata::getActualRating(){ 
    return actualBelongRate;    
}

void OlmoAutomata::calcSequenceRating(vector<int> sequence){    
    
    actualHits=0;
    actualErrors=0;  
    //actualSequenceLength= sequence.size();
    
    //initialize
    transMatrCopy=transitionMatrix;  
   
    
    for(int i=0; i< sequence.size(); i++){        
        
        int state=sequence[i];
        
        if(i<sequenceLength && stateMatrix[i][state]==1){
//                    if(i<sequenceLength && (stateMatrix[i][state]==1 || 
//                                                (i>0 && stateMatrix[i-1][state]==1) ||
//                                                (i<sequenceLength-1 && stateMatrix[i+1][state]==1)
//                            )){
//                                                (i>1 && stateMatrix[i-2][state]==1) ||
//                                                (i<sequenceLength-2 && stateMatrix[i+2][state]==1) 
//                                                )){
                   
                actualHits++;
        }
//        if(i==0){
//            if(initStates[state]==1){     //check initial state           
//                actualHits+=5;                
//            }else{                
//                actualErrors++;
//            }
//            
//        }else  if(i==(sequence.size()-1)){
//            if(finalStates[state]==1){
//                actualHits+=5;                
//             }else{               
//                actualErrors++;
//            }
//        }             
        
        if(i!=0 && i<sequenceLength){            
            if(transMatrCopy[actualState][state]>0){
                actualHits++;               
                transMatrCopy[actualState][state]--;                
            }else{              
                actualErrors++;                                
            }
        }        
        actualState=state;        
    }
 
//    int diff= sequenceLength-sequence.size();
    actualBelongRate=(double)(actualHits)/(sequenceLength*2);// -(double)actualErrors/sequenceLength;  
    actualBelongRate/=minPuntBelong; 
}

void OlmoAutomata::addNewState(SimpleSkeleton skel){    
     addNewState(skel.idCluster-1);
}
void OlmoAutomata::addNewState(int newState){     
    
    if(actualSequenceLength<sequenceLength){        
        sequenceWindow[actualSequenceLength++]=newState;        
        if(actualSequenceLength==sequenceLength){          
            //we must start the algorithm           
            calcSequenceRating(sequenceWindow);                                  
        }
    }else{         
        sequenceWindow.erase(sequenceWindow.begin());
        sequenceWindow.push_back(newState);        
        calcSequenceRating(sequenceWindow); 
    }     
}

bool OlmoAutomata::isSequenceValid(vector<int> *sequence){
   startAutomata();
   calcSequenceRating(*sequence); 
   
    return isFinalState();
}

 bool OlmoAutomata::isSequenceValid(BehaviourSequence behSequence){
     return isSequenceValid(&(behSequence.sequence));
 }

bool OlmoAutomata::isFinalState(){   
    return actualBelongRate<=1 && actualBelongRate>=ThresholdBelong;
}

int compareAutomatas(vector<OlmoAutomata> automs, vector<int> sequence){
    int selAction=-1;
    double best=-100;

    for (int i = 0; i < automs.size(); i++) {
        if (automs[i].isSequenceValid(&sequence)) {
            double count = automs[i].getActualRating();
            if (selAction == -1 || count > best) {
                best = count;
                selAction = i;
            }
        }      
    }

    return selAction;
}

void saveOlmoTrainingData(Connection *con, string idsTraining, 
         int bestErrorTrain, int bestErrorTest,
         int worstErrorTrain, int worstErrorTest,
         double meanErrorTrain, double meanErrorTest,
         vector<int> *errorsTrainByAction, vector<vector<int> >*errorsTrainByActionSel,
         vector<int> *errorsTestByAction, vector<vector<int> > *errorsTestByActionSel){
     
    PreparedStatement *prep_stmt;
    std::ostringstream s1,s2,s3,s4;
    
    for(int i=0;i<Num_Actions;i++){
        s1 << (*errorsTrainByAction)[i] << ",";               
        s3 << (*errorsTestByAction)[i] << ",";
        for(int j=0;j<Num_Actions;j++){
            s2 << (*errorsTrainByActionSel)[i][j] << ",";
            s4 << (*errorsTestByActionSel)[i][j] << ",";
        }
        s2<<endl;
        s4<<endl;
    }
    string errorsTrainByAct, errorsTrainByActSel, errorsTestByAct, errorsTestByActSel;
    
    errorsTrainByAct= s1.str();
    errorsTrainByActSel=s2.str();
    errorsTestByAct= s3.str();
    errorsTestByActSel=s4.str();
       
    prep_stmt = con -> prepareStatement(
            "INSERT INTO `OlmoTrainingSet`(`num_clusters`, `compression_rate`, "
            "`training_rate`, `test_rate`,`best_errors_training`, `best_errors_test`,"
            " `worst_errors_training`, `worst_errors_test`, `mean_errors_training`, "
            "`mean_errors_test`, `errors_training_by_action`, `errors_training_by_action_selected`, "
            "`errors_test_by_action`, `errors_test_by_action_selected`, `ids_training`) "
            "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)"             
            );

    prep_stmt -> setInt(1, numClusters);
    prep_stmt -> setInt(2, compressionRate);
    prep_stmt -> setInt(3, TrainingRate);
    prep_stmt -> setInt(4, TestRate);
    prep_stmt -> setInt(5, bestErrorTrain);
    prep_stmt -> setInt(6, bestErrorTest);
    prep_stmt -> setInt(7, worstErrorTrain);
    prep_stmt -> setInt(8, worstErrorTest);
    prep_stmt -> setDouble(9, meanErrorTrain);
    prep_stmt -> setDouble(10, meanErrorTest);
    prep_stmt -> setString(11, errorsTrainByAct.substr(0, errorsTrainByAct.size() - 1));
    prep_stmt -> setString(12, errorsTrainByActSel.substr(0, errorsTrainByActSel.size() - 1));
    prep_stmt -> setString(13, errorsTestByAct.substr(0, errorsTestByAct.size() - 1));
    prep_stmt -> setString(14, errorsTestByActSel.substr(0, errorsTestByActSel.size() - 1));
    prep_stmt -> setString(15, idsTraining);
    
    prep_stmt->executeUpdate();
    
    delete prep_stmt;
 
 }
void OlmoAutomata::save2DB() {
    connectSQL();
    
  cout<<"Saving the OlmoAutomata params infered"<<endl;
    
     PreparedStatement *prep_stmt;
    prep_stmt = con -> prepareStatement(            
            "INSERT INTO `OlmoAutomataDS`(`num_clusters`, `compression_rate`, `id_action`, `transition_matrix`, "
            "`state_matrix`, `sequence_length`, `min_punt_belong`, `distance_measure`)"
            " VALUES (?,?,?,?,?,?,?,?,?)"
            );

   
    std::ostringstream sTransMatrix;
    for (int i = 0; i < numClusters; i++) {
        for (int j = 0; j < numClusters; j++) {
            sTransMatrix << " " << transitionMatrix[i][j];
        } 
    }
    std::ostringstream sStateMatrix;     
    for (int i = 0; i < sequenceLength; i++) {
        for (int j = 0; j < sequenceLength; j++) {
            sStateMatrix << " " << stateMatrix[i][j];
        } 
    }
    
    prep_stmt -> setInt(1, numClusters);        
    prep_stmt -> setInt(2, compressionRate);    
    prep_stmt -> setInt(3, idAction);  
    prep_stmt -> setString(4, sTransMatrix.str());
    prep_stmt -> setString(5, sStateMatrix.str());   
    prep_stmt -> setInt(6, sequenceLength);  
    prep_stmt -> setDouble(7, minPuntBelong);  
    prep_stmt -> setString(8, DistanceMeasureUsed);
    prep_stmt-> executeUpdate();   
    delete prep_stmt;     
    
    disconnectSQL();
}

bool OlmoAutomata::readDB(int idAction) {
    connectSQL();
    
    cout<<"Reading OlmoAutomata for action:"<<idAction<<endl;
    this->idAction=idAction;
    
    ResultSet *resSet;
    PreparedStatement *prep_stmt;

    /* create a statement object */
    prep_stmt = con -> prepareStatement("SELECT * FROM `OlmoAutomataDS` WHERE `compression_rate`=? AND `num_clusters`=? AND `id_action`=? AND `distance_measure`=?");
    prep_stmt->setInt(1, compressionRate);  
    prep_stmt->setInt(2, numClusters);  
    prep_stmt->setInt(3, idAction);    
    prep_stmt -> setString(4, DistanceMeasureUsed);
    resSet = prep_stmt->executeQuery();

    bool dataExists=false;
    while (resSet->next()) {
        dataExists=true;
        std::stringstream sTransMatrix(resSet->getString("transition_matrix"));
        std::stringstream sStateMatrix(resSet->getString("state_matrix"));          
        sequenceLength=resSet->getInt("sequence_length");
        minPuntBelong=resSet->getDouble("min_punt_belong");
        
        transitionMatrix.resize(numStates, vector<int>(numStates, 0));
        stateMatrix.resize(sequenceLength, vector<int>(sequenceLength, 0));       
               
        for (int i = 0; i < numClusters; i++) {
            for (int j = 0; j < numClusters; j++) {
                sTransMatrix >> transitionMatrix[i][j];
            }
        }
       
         for (int i = 0; i < sequenceLength; i++) {
            for (int j = 0; j < sequenceLength; j++) {
                sStateMatrix >> stateMatrix[i][j];
            }
        }
    }   
   
    delete resSet;    
    delete prep_stmt;
    
    disconnectSQL();
    
    startAutomata();
    
    return dataExists;
}