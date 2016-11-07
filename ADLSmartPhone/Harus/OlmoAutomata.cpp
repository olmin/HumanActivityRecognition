/* 
 * File:   OlmoAutomata.cpp
 * Author: olmin
 * 
 * Created on 22 de noviembre de 2012, 12:18
 */

#include "OlmoAutomata.h"

OlmoAutomata::OlmoAutomata(vector <vector<int> > data) {
    
    transitionMatrix.resize(numStates, vector<int>(numStates,0));
    initStates.resize(numStates,0);
    finalStates.resize(numStates,0);
    sequenceLength=data[2].size();
    sequenceWindow.resize(sequenceLength,-1);       
    actualHits=0;
    actualErrors=0;
    actualSequenceLength=0;
    
    vector <vector <int> > countMat;
     
    for(int i=0; i< data.size();i++){
        int lastState;
        countMat.clear();
        countMat.resize(numStates, vector<int>(numStates,0));
        
        for(int j=0; j< data[i].size();j++){
            int state= data[i][j];
            
            if(j==0){
                //first state
                initStates[state]=1;
            }else{
                
                if(j==data[i].size()-1){
                    //final state
                    finalStates[state]=1;
                }
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


void OlmoAutomata::startAutomata(){   
    actualHits=0;
    actualErrors=0;
    actualSequenceLength=0;
    actualBelongRate=-1000;
    sequenceWindow.clear();
    sequenceWindow.resize(sequenceLength,-1);  
}

 void OlmoAutomata::startInference(ClusterData clusters, HarusDataSet trainDataSet){
     
 }

void OlmoAutomata::save() {
    openMysqlSession();
    
  cout<<"Saving the OlmoAutomata params infered"<<endl;
    
     PreparedStatement *prep_stmt;
    prep_stmt = con -> prepareStatement(            
            "INSERT INTO `OlmoAutomataDS`(`num_clusters`, `compression_rate`, `id_action`, `transition_matrix`, "
            "`init_states`, `final_states`,`sequence_length`, `min_punt_belong`)"
            " VALUES (?,?,?,?,?,?,?,?)"
            );

   
    std::ostringstream sTransMatrix;
    for (int i = 0; i < numClusters; i++) {
        for (int j = 0; j < numClusters; j++) {
            sTransMatrix << " " << transitionMatrix[i][j];
        } 
    }
    std::ostringstream sInitState;
    for (int j = 0; j < numClusters; j++) {
        sInitState << " " << initStates[j];
    }   
    
    std::ostringstream sFinalState;
    for (int i = 0; i < numClusters; i++) {        
            sFinalState << " " << finalStates[i];        
    }
    
    prep_stmt -> setInt(1, numClusters);        
    prep_stmt -> setInt(2, compressionRate);    
    prep_stmt -> setInt(3, idAction);  
    prep_stmt -> setString(4, sTransMatrix.str());
    prep_stmt -> setString(5, sInitState.str());
    prep_stmt -> setString(6, sFinalState.str());
    prep_stmt -> setInt(7, sequenceLength);  
    prep_stmt -> setDouble(8, minPuntBelong);  
    
    prep_stmt-> executeUpdate();   
    delete prep_stmt;     
    
    closeMysqlSession();
}

void OlmoAutomata::read(int idAction) {
    openMysqlSession();
    
    cout<<"Reading OlmoAutomata for action:"<<idAction<<endl;
    this->idAction=idAction;
    
    ResultSet *resSet;
    PreparedStatement *prep_stmt;

    /* create a statement object */
    prep_stmt = con -> prepareStatement("SELECT * FROM `OlmoAutomataDS` WHERE `compression_rate`=? AND `num_clusters`=? AND `id_action`=?");
    prep_stmt->setInt(1, compressionRate);  
    prep_stmt->setInt(2, numClusters);  
    prep_stmt->setInt(3, idAction);    
    resSet = prep_stmt->executeQuery();

    while (resSet->next()) {
        std::stringstream sTransMatrix(resSet->getString("transition_matrix"));
        std::stringstream sInitState(resSet->getString("init_states"));
        std::stringstream sFinalState(resSet->getString("final_states"));      
        sequenceLength=resSet->getInt("sequence_length");
        minPuntBelong=resSet->getDouble("min_punt_belong");
        
        transitionMatrix.resize(numStates, vector<int>(numStates, 0));
        initStates.resize(numStates, 0);        
        finalStates.resize(numStates,0);
               
        for (int i = 0; i < numClusters; i++) {
            for (int j = 0; j < numClusters; j++) {
                sTransMatrix >> transitionMatrix[i][j];
            }
        }
       
        for (int j = 0; j < numClusters; j++) {
            sInitState >> initStates[j];
        }
       
        for (int i = 0; i < numClusters; i++) {           
                sFinalState>> finalStates[i];            
        }      
    }   
   
    delete resSet;    
    delete prep_stmt;
    
    closeMysqlSession();
    
    startAutomata();
    
    cout<<"DONE for action:"<<idAction<<endl;
}

void OlmoAutomata::checkNewSequence(vector<int> sequence) {
    calcSequenceRating(sequence);
}

double OlmoAutomata::getActualProbability() {
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
        
        if(i==0){
            if(initStates[state]==1)     //check initial state           
                actualHits++;          
            else
                actualErrors++;
            
        }else  if(i==(sequence.size()-1)){
            if(finalStates[state]==1)
                actualHits++;
            else
                actualErrors++;
        }             
        if(i!=0){            
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
    actualBelongRate=(double)(actualHits)/sequenceLength;// -(double)actualErrors/sequenceLength;
    actualBelongRate/=minPuntBelong; 
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


bool OlmoAutomata::isSequenceValid(vector<int> sequence){
   startAutomata();
   calcSequenceRating(sequence); 
   
    return isFinalState();
}

bool OlmoAutomata::isFinalState(){   
    return actualBelongRate<=1 && actualBelongRate>=ThresholdBelong;
}

int compareAutomatas(vector<OlmoAutomata> automs, vector<int> sequence){
    int selAction=-1;
    double best=-100;

    for (int i = 0; i < automs.size(); i++) {
        if (automs[i].isSequenceValid(sequence)) {
            double count = automs[i].getActualProbability();
            if (selAction == -1 || count > best) {
                best = count;
                selAction = i;
            }
        }      
    }

    return selAction;
}