/* 
 * File:   HMM.cpp
 * Author: olmin
 * 
 * Created on 4 de noviembre de 2011, 18:15
 */

#include "HMM.h"
//#include"../Clustering/Commons.h"

/*!Must be initialize with the number of states and the Clusters Distribution*/
HMM::HMM(int numStates, vector<SimpleSkeleton> clusters, list<BehaviourSequence> *dataSeq, int id) {
    this->idAction=id;
    this->actualProb = 0;
    //this->numStates = numStates;
    //calculate the clusters distribution for that behaviour
    this->clustDist = getClustersDistribution(dataSeq, clusters);
    stateTrans.resize(numStates, vector<double>(numStates, 0));
    initialStateDist.resize(numStates, 0);
    obsProbDist.resize(numStates, vector<double>(numStates,0));
    fwdVars.resize(numStates, 0);
    this->initialize(dataSeq);

}

void HMM::train(vector<SimpleSkeleton> clusters, list<BehaviourSequence> dataSeq, int id){
     this->idAction=id;
   this->actualProb = 0;
    //this->numStates = numStates;
    //calculate the clusters distribution for that behaviour
    this->clustDist = getClustersDistribution(&dataSeq, clusters);
    stateTrans.resize(numStates, vector<double>(numStates, 0));
    initialStateDist.resize(numStates, 0);
    obsProbDist.resize(numStates, vector<double>(numStates,0));
    fwdVars.resize(numStates, 0);
    this->initialize(&dataSeq);
}

/*! Initializes de HMM: the initialStateDistribution, the stateTransition matrix and the observation probabilistic distribution */
void HMM::initialize(list<BehaviourSequence> *dataSeq) {

    setStateTransition(dataSeq);

    setInitialStateDist(dataSeq);

    setObsProbDist(dataSeq);
    
    list<BehaviourSequence>::iterator it;
    vector<int> observations;
    for(it=dataSeq->begin();it!=dataSeq->end();it++){   
        sequenceLength=0;
        for(int i=0;i<it->sequence.size();i++){
           observations.push_back(it->sequence[i]); 
           sequenceLength++;
        }
    }
     sequenceWindow.resize(sequenceLength,-1);
     
     execBaumWelch(&observations);      
      
     //now we see the lowest prob to belong to the cluster
     double lowest=0;
     for(it=dataSeq->begin();it!=dataSeq->end();it++){
         setObservationsAlpha(&(it->sequence));
         double actual=getActualRating();
         if(actual<lowest)
             lowest=actual;
     }
     thresholdBelong=lowest;  
}

void HMM::setStateTransition(list<BehaviourSequence> *data) {
  //stateTrans.resize(numStates, vector<double>(numStates, 0));
 
  list<BehaviourSequence>::iterator it = data->begin();

    for (; it != data->end(); it++) {
       
        int size = it->sequence.size() - 1;        
        for (int i = 0; i < size; i++) {
            //every time we from i to (i+1) we sum one             
            stateTrans[it->sequence[i]][it->sequence[i + 1]]++;            
        }
    }
    int sum;
    // cout<<"StateTrans: "<< data->front().idAction << endl;
    for (int i = 0; i < numStates; i++) {
        sum = 0;
        //we sum the times it goes from i to somewhere else
        for (int j = 0; j < numStates; j++) {
            sum += stateTrans[i][j];
        }
        //now we divide everyone by the sum
        for (int j = 0; j < numStates; j++) {
            if (sum != 0)
                stateTrans[i][j] /= sum;
            stateTrans[i][j]+=0.001;//laplace correction
            
            //   cout<<stateTrans[i][j]<<" ";
        }
        //   cout<<endl;
    }
}

void HMM::setInitialStateDist(list<BehaviourSequence> *data) {
    //initialStateDist.resize(numStates, 0);
    int count = 0;

    list<BehaviourSequence>::iterator it = data->begin();

    for (; it != data->end(); it++) {
        initialStateDist[it->sequence[0]]++; //we take the first symbol  
        count++;
    }
    // cout<<"Initial: "<< data->front().idAction << endl;
    for (int i = 0; i < numStates; i++) {
        initialStateDist[i] /= count;                
        initialStateDist[i]+=0.001;
        //    cout<<initialStateDist[i]<<" ";
    }
    // cout<<endl;
}

void HMM::setObsProbDist(list<BehaviourSequence> *behs) {
    //obsProbDist.resize(numStates, vector<double>(numStates,0));
        
    vector<int> count(numStates,0);
    
    list<BehaviourSequence>::iterator it;

    for (it = behs->begin(); it != behs->end(); it++) {        
        for (int i = 0; i < numStates ; i++) {
            list<SimpleSkeleton>::iterator itFrame=it->skeletons.begin();
            //every time we from i to (i+1) we sum one             
            for(int k=0;k<it->sequence.size() - 1;k++){
                float prob=clustDist[i].getProbBelong(*itFrame);
                obsProbDist[i][it->sequence[k]]+=prob;
                
               // cout<<"(i,j)= ("<<it->sequence[i]<<","<<j<<")   prob:"<<prob<<endl;
                count[it->sequence[k]]++;
                itFrame++;
            }
            
        }
    }   
   // cout<<"ObsTrans: "<< behs->front().idAction << endl;
    for (int i = 0; i < numStates; i++) {
        //now we divide everyone by the sum
        for (int k = 0; k < numStates; k++) {            
            if (count[k] != 0)
                obsProbDist[i][k] /= count[k];            
            obsProbDist[i][k] += 0.001;
        }
    }        
}

 bool HMM::isSequenceValid(vector<int> *behSeq){
     setObservationsAlpha(behSeq);
 }
 
 bool HMM::isSequenceValid(BehaviourSequence behSequence){
     return isSequenceValid(&(behSequence.sequence));
 }
 
/*!Iterates the HMM after the observations*/
void HMM::setObservations(list<SimpleSkeleton> *observation) {
//    setObservationsAlpha(observation);
    //init
    actualProb = 0;
    //then we iterate for each frame 
    list<SimpleSkeleton> ::iterator itFrame;
    itFrame = observation->begin();
    //step 1, initializa
    setInitialForwdVar(*itFrame);
    itFrame++;

    //step 2, iterate for every Observation
    for (; itFrame != observation->end(); itFrame++) {
        for (int actualState = 0; actualState < numStates; actualState++){
            fwdVars[actualState] = iterateForwdVar(*itFrame, actualState);
        }
    }

    //step 3, final probability    
    for (int i = 0; i < numStates; i++) {       
        actualProb += fwdVars[i];
    }   
  //  cout<<endl<<"ActualProb: "<<actualProb<< ", idbeh: "<<observation->front().idBehaviour <<endl;
}
void HMM::setInitialForwdVar(SimpleSkeleton dataObs) {
    //fwdVars.resize(numStates, 0);
    for (int i = 0; i < numStates; i++) {
        fwdVars[i] = initialStateDist[i] * clustDist[i].getProbBelong(dataObs);
    }
}
double HMM::iterateForwdVar(SimpleSkeleton dataObs, int actualState) {
//    vector<double> newVars;
//    newVars.resize(numStates, 0);
    double agr = 0;
    for (int i = 0; i < numStates; i++) {
        agr += fwdVars[i] * stateTrans[i][actualState];
    }
   
    return agr * clustDist[actualState].getProbBelong(dataObs);
}


/*!Iterates the HMM after the observations*/
void HMM::setObservationsAlpha(vector<int> *observation) {
    setFwdVars(observation);
}

void HMM::setFwdVars(vector<int> *observations) {
    int i, j; /* state indices */
    int t; /* time index */

    double sum; /* partial sum */
    /* 1. Initialization */
    //vector<int>::iterator it = observations->begin();        
    int timeLength=observations->size();
    alpha.clear();
    scale.clear();
    alpha.resize(timeLength, vector<double>(numStates,0));
    scale.resize(timeLength,0);    
     /* 1. Initialization */   
    for (i = 0; i <numStates; i++){
        alpha[0][i] = initialStateDist[i] * obsProbDist[i][(*observations)[0]];//clustDist[i].getProbBelong(*it);
        scale[0]+=alpha[0][i];
    }
    for (i = 0; i <numStates; i++) 
	alpha[0][i] /= scale[0]; 
    
    /* 2. Induction */
    for (t = 0; t < timeLength-1; t++) {
        //it++;  
        scale[t+1] = 0.0;      
        for (j = 0; j < numStates; j++) {
            sum = 0.0;
            for (i = 0; i <numStates; i++)
                sum += alpha[t][i]* (stateTrans[i][j]);
              
            alpha[t + 1][j] = sum * (obsProbDist[j][(*observations)[t+1]]);
            scale[t+1] += alpha[t+1][j];            
        }
       
        for (j = 0; j <numStates; j++) 
            alpha[t+1][j] /= scale[t+1];
    }

    /* 3. Termination */
    actualProb = 0.0;
    for (t = 0; t <timeLength; t++){       
	actualProb += log(scale[t]);      
    }
    
//    cout<<endl<<"ActualProb: "<<actualProb<<endl;
}   

void HMM::setBackwVars(vector<int> *observations) {
    int i, j; /* state indices */
    int time; /* time index */
    double sum;

//    vector<int>::iterator it = observations->end();
    /* 1. Initialization */
    int timeLength=observations->size();
    betha.clear();
    betha.resize(timeLength, vector<double>(numStates,0));
    
    for (i = 0; i <numStates; i++)
        betha[timeLength-1][i] = 1.0/scale[timeLength-1];

    /* 2. Induction */
    for (time = timeLength - 2; time >= 0; time--) {
        for (i = 0; i < numStates; i++) {
            sum = 0.0;
            for (j = 0; j < numStates; j++){
                sum += stateTrans[i][j] *
                    obsProbDist[j][(*observations)[time+1]] * betha[time + 1][j];
            }
            betha[time][i] = sum/scale[time];           
        }
       // it--;
    }    
}

//
//void HMM::setInitialBackwdVar() {
//    backwdVars.resize(numStates, 0);
//    for (int i = 0; i < numStates; i++) {
//        backwdVars[i] = 1;
//    }
//}
//
//double HMM::iterateBackwdVar(SimpleSkeleton nextObs, int actualState) {
//
//    double agr = 0;
//    for (int j = 0; j < numStates; j++) {
//        agr += stateTrans[actualState][j] * clustDist[j].getProbBelong(nextObs);
//    }
//    //backwdVars[actualState]=
//    return agr;
//}

//double HMM::getGamma(int time, int actualState, list<SimpleSkeleton> *observations) {
//   // setObservations(observations);
//    double probObserv = actualProb;
//    list<SimpleSkeleton>::iterator it = observations->begin();
//
//    if(time==0)
//        cout<<"stop"<<endl;
//    setInitialForwdVar(*it);
//    for (int i = 0; i < time; i++) {
//        it++;
//        for (int state = 0; state < numStates; state++){
//            fwdVars[state] = iterateForwdVar(*it, state); 
////            if(state==8)
////                cout<<"fwdvar (i,val): ("<<state<< ", "<<fwdVars[state]<<")"<<endl;
//        }
//    }
//
//    it = observations->end();
//    setInitialBackwdVar();
//    for (int j = observations->size(); j >= time; j--) {
//        it--;
//        for (int state = 0; state < numStates; state++){
//            backwdVars[state] = iterateBackwdVar(*it, state);           
//        }
//    }

vector<vector<double> > HMM::getGamma(vector<int> *observations) {

    vector<vector<double> > gamma(observations->size(), vector<double>(numStates, 0));
    double denominator;

    for (int time = 0; time < observations->size(); time++) {
        denominator = 0.0;
        for (int j = 0; j < numStates; j++) {
            gamma[time][j] = alpha[time][j] * betha[time][j];
            denominator += gamma[time][j];        
        }
               
        for (int i = 0; i < numStates; i++)
            gamma[time][i] = gamma[time][i] / denominator;
    }
    return gamma;
}
    
  

//double HMM::getEpsilon(int time, int actualState, int nextState, list<SimpleSkeleton> *observations) {
//   // setObservations(observations);
//    double probObserv = actualProb;
//    list<SimpleSkeleton>::iterator it = observations->begin();
//    int i, j;
//    setInitialForwdVar(*it);
//    for (i = 0; i < time; i++) {
//        it++;
//        for (int state = 0; state < numStates; state++){
//            fwdVars[state] = iterateForwdVar(*it, state);             
//        }
//    }
//
//    it = observations->end();
//    setInitialBackwdVar();
//    for (j = observations->size(); j > time; j--) {
//        it--;
//        for (int state = 0; state < numStates; state++){
//            backwdVars[state] = iterateBackwdVar(*it, state);            
//        }
//    }
//   
//    return stateTrans[actualState][nextState] * clustDist[nextState].getProbBelong(*it) *
//            fwdVars[actualState] * backwdVars[nextState] / probObserv;
//}

vector<vector<vector<double> > > HMM::getEpsilon(vector<int> *observations) {
   
    vector<vector<vector<double> > > epsilon(observations->size(),
                                               vector<vector<double> >(numStates, vector<double>(numStates, 0)));
    double sum;
   // vector<int>::iterator it = observations->begin();
    for (int time = 0; time < observations->size()-1; time++) {
        sum = 0.0;
        //it++;
        for (int i = 0; i < numStates; i++)
            for (int j = 0; j < numStates; j++) {
                epsilon[time][i][j] = alpha[time][i] * betha[time + 1][j]
                        *stateTrans[i][j]
                        *obsProbDist[j][(*observations)[time+1]];
                sum += epsilon[time][i][j];
            }
               
        for (int i = 0; i <numStates; i++)
            for (int j = 0; j <numStates; j++)
                epsilon[time][i][j] /= sum;
    }       
   
   return epsilon;
}

bool HMM::execBaumWelch(vector<int> *observations) {
    //initialize
   
    double oldProb = 0;  
    double delta=10e+70;    
   
    setObservationsAlpha(observations);
  //  double initProb=actualProb;
    while (delta > DELTA) {
        oldProb=actualProb;
        
//        vector<vector<double> > newStateTrans(numStates, vector<double>(numStates, 0));
//        vector<double> newInitialStateDist(numStates, 0);
       // vector<float> newObsProbDist(numStates, 0);


//        for (int i = 0; i < numStates; i++) {
//            newInitialStateDist[i] = getGamma(1, i, observations);
//        }
       // setFwdVars(observations);
        setBackwVars(observations);
        vector<vector<double> > gamma= getGamma(observations);
        vector<vector<vector<double> > > epsilon= getEpsilon(observations);
                        
        //2.reestimate stateTrans         
        double sumGamma, sumEpsilon,denominatorB,numeratorB;
        for (int i = 0; i < numStates; i++) {
            int timeLength=observations->size();
            
            //1.reestimate newInitialStateTrans           
            initialStateDist[i] = .001+ 0.999*gamma[0][i]; //getGamma(0, i, observations);
            
             //2.numerator  
             sumGamma = 0;
             for (int time = 0; time < timeLength - 1; time++) {
                    sumGamma += gamma[time][i];                   
             }
            
            //3.denominator
            for (int j = 0; j < numStates; j++) {                
                sumEpsilon = 0;
                for (int time = 0; time < timeLength - 1; time++) {                   
                    sumEpsilon += epsilon[time][i][j];//getEpsilon(time, i, j, observations);               
                }                 
                stateTrans[i][j] = .001+ 0.999*sumEpsilon / sumGamma;                
            }

            denominatorB = sumGamma + gamma[timeLength- 1][i];
            for (int k = 0; k < numStates; k++) {
                //vector<int>::iterator it=observations->begin();
                numeratorB = 0.0;
                for (int t = 0; t < timeLength; t++) {
                    if ((*observations)[t] == k)
                        numeratorB += gamma[t][i];
                    //it++;
                }
               obsProbDist[i][k] = .001 +
                        .999 * numeratorB / denominatorB;
               
            }
            
        }//end reestimate stateTrans

            
//        stateTrans=newStateTrans;
//        initialStateDist=newInitialStateDist;

      
        setObservationsAlpha(observations);      
        delta=actualProb-oldProb;
    }
 // cout<<"BaumWelch finished init and final prob:"<<initProb<<", "<<actualProb<<endl;
}

int compareHMM(vector<HMM> models, list<SimpleSkeleton> *behSeq) {
    int selAction;
    double maxProb = 0;

    for (int i = 0; i < Num_Actions; i++) {
        models[i].setObservations(behSeq);
        double prob = models[i].getActualProbability();
        //cout<<"Prob idBeh:"<<behaviour->front().idBehaviour <<",model: "<< i << ",prob: "<<prob<<endl;
        if (maxProb < prob) {
            maxProb = prob;
            selAction = i;
        }
    }
    return selAction;
}

int compareHMM(vector<HMM> models, vector<int> *behSeq) {
    int selAction;
    double maxProb = -10e+10;

    for (int i = 0; i < Num_Actions; i++) {
        models[i].setObservationsAlpha(behSeq);
        double prob = models[i].getActualProbability();
        //cout<<"Prob idBeh:"<<behaviour->front().idBehaviour <<",model: "<< i << ",prob: "<<prob<<endl;
        if (maxProb < prob) {
            maxProb = prob;
            selAction = i;
        }
    }
    return selAction;
}

//void saveTrainingData(Connection *con, string idsTraining, 
//         int bestErrorTrain, int bestErrorTest,
//         int worstErrorTrain, int worstErrorTest,
//         double meanErrorTrain, double meanErrorTest,
//          vector<int> *errorsTrainByAction, vector<vector<int> >*errorsTrainByActionSel,
//         vector<int> *errorsTestByAction, vector<vector<int> > *errorsTestByActionSel){
//     
//    PreparedStatement *prep_stmt;
//    std::ostringstream s1,s2,s3,s4;    
//   
//     for(int i=0;i<Num_Actions;i++){
//        s1 << (*errorsTrainByAction)[i] << ",";               
//        s3 << (*errorsTestByAction)[i] << ",";
//        for(int j=0;j<Num_Actions;j++){
//            s2 << (*errorsTrainByActionSel)[i][j] << ",";
//            s4 << (*errorsTestByActionSel)[i][j] << ",";
//        }
//        s2<<endl;
//        s4<<endl;
//    }
//    string errorsTrainByAct, errorsTrainByActSel, errorsTestByAct, errorsTestByActSel;
//    
//    errorsTrainByAct= s1.str();
//    errorsTrainByActSel=s2.str();
//    errorsTestByAct= s3.str();
//    errorsTestByActSel=s4.str();
//       
//    prep_stmt = con -> prepareStatement(
//            "INSERT INTO `HMMTrainingSet`(`num_clusters`, `compression_rate`, "
//            "`training_rate`, `test_rate`, `best_errors_training`, `best_errors_test`,"
//            " `worst_errors_training`, `worst_errors_test`, `mean_errors_training`, "
//            "`mean_errors_test`, `errors_training_by_action`, `errors_training_by_action_selected`, "
//            "`errors_test_by_action`, `errors_test_by_action_selected`, `ids_training`) "
//            "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)"             
//            );
//
//    prep_stmt -> setInt(1, numClusters);
//    prep_stmt -> setInt(2, compressionRate);
//    prep_stmt -> setInt(3, TrainingRate);
//    prep_stmt -> setInt(4, TestRate);
//    prep_stmt -> setInt(5, bestErrorTrain);
//    prep_stmt -> setInt(6, bestErrorTest);
//    prep_stmt -> setInt(7, worstErrorTrain);
//    prep_stmt -> setInt(8, worstErrorTest);
//    prep_stmt -> setDouble(9, meanErrorTrain);
//    prep_stmt -> setDouble(10, meanErrorTest);
//    prep_stmt -> setString(11, errorsTrainByAct.substr(0, errorsTrainByAct.size() - 1));
//    prep_stmt -> setString(12, errorsTrainByActSel.substr(0, errorsTrainByActSel.size() - 1));
//    prep_stmt -> setString(13, errorsTestByAct.substr(0, errorsTestByAct.size() - 1));
//    prep_stmt -> setString(14, errorsTestByActSel.substr(0, errorsTestByActSel.size() - 1));
//    prep_stmt -> setString(15, idsTraining);
//    
//    prep_stmt->executeUpdate();
//    
//    delete prep_stmt;
// 
// }

 void saveTrainingDataMultiUser(Connection *con, string idsTraining, 
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
            "INSERT INTO `HMMTrainingSet`(`num_clusters`, `compression_rate`, "
            "`training_rate`, `test_rate`, `best_errors_training`, `best_errors_test`,"
            " `worst_errors_training`, `worst_errors_test`, `mean_errors_training`, "
            "`mean_errors_test`, `errors_training_by_action`, `errors_training_by_action_selected`, "
            "`errors_test_by_action`, `errors_test_by_action_selected`, `ids_training`, `user`) "
            "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)"             
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
    prep_stmt -> setInt(16, actualIdUser);
    
    prep_stmt->executeUpdate();
    
    delete prep_stmt;
 
 }
 
 void HMM::addNewState(SimpleSkeleton skel){    
     addNewState(skel.idCluster-1);
}
 
 void HMM::addNewState(int newState){    
    if(actualSequenceLength<sequenceLength){         
        sequenceWindow[actualSequenceLength++]=newState;        
        if(actualSequenceLength==sequenceLength){          
            //we must start the algorithm          
            setObservationsAlpha(&sequenceWindow);        
        }
    }else{         
        sequenceWindow.erase(sequenceWindow.begin());
        sequenceWindow.push_back(newState);        
        setObservationsAlpha(&sequenceWindow); 
    }     
}

void HMM::startAutomata(){
    
}

bool HMM::isFinalState(){   
//    cout<<"Final?:"<<actualProb<<endl;
    return actualProb>=thresholdBelong-50;
}
 
// string getIdsTraining(list<BehaviourSequence> *dataTraining){
//     std::ostringstream s1;
// 
//     list<BehaviourSequence>::iterator it;
//    for(it=dataTraining->begin();it!=dataTraining->end();it++)
//        s1<<it->idBehaviour <<",";
//     
//    string idsTraining= s1.str();
//    
//    return idsTraining.substr(0, idsTraining.size() - 1);
// }
 
//  vector<int> getIdsVector(string ids){
//     vector<int> result;
//          
//     while(true){
//        int pos=ids.find(",");  
//        if(pos== -1){
//            result.push_back(atoi(ids.c_str()));
//            return result;        
//        }        
//        result.push_back(atoi(ids.substr(0,pos).c_str()));        
//        ids.erase(0,pos+1);        
//    }  
//    
// }
//  
// list<BehaviourSequence> getTrainedData(Connection *con,int idAction){
//     
//     list<BehaviourSequence> result;
//     
//    ResultSet *resBehaviours;
//    PreparedStatement *prep_stmt;
//     
//      /* create a statement object */
//    prep_stmt = con -> prepareStatement("SELECT  `ids_training` FROM  `HMMTrainingSet` "
//                      "WHERE  `num_clusters` =? AND  `compression_rate` =? ");
//    //begin get the resultset
//    prep_stmt->setInt(1, numClusters);
//    prep_stmt->setInt(2, compressionRate);
//        
//    resBehaviours = prep_stmt -> executeQuery();
//     
//    string ids;
//      /*For each action we get the ids of the behaviours*/
//    while (resBehaviours->next()) {        
//        ids = resBehaviours->getString(1);
//    }    
//     
//    vector<int> idsVector= getIdsVector(ids);   
//    for(int i=0;i < idsVector.size();i++){           
//        BehaviourSequence beh= getBehaviour(con,idsVector[i]);
//        
//        if(beh.idAction==idAction)         
//            result.push_back(beh);
//    }
//    
//    delete resBehaviours;
//    delete prep_stmt;     
//     
//    return result;
//     
// }


// void saveValidationData(Connection *con, int errors,       
//                 vector<int> * errorsByAction, vector<int> *errorsByActionSel){
//     
//     PreparedStatement *prep_stmt;
//             
//    std::ostringstream s1,s2;
//    
//     for(int i=0;i<Num_Actions;i++){
//        s1 << (*errorsByAction)[i] << ",";
//        s2 << (*errorsByActionSel)[i] << ",";       
//    }
//    string errorsByAct, errorsByActSel;
//    
//    errorsByAct= s1.str();
//    errorsByActSel=s2.str();
//       
//    prep_stmt = con -> prepareStatement(
//            "INSERT INTO `ValidationResults`"
//            "(`num_clusters`, `compression_rate`,`validation_rate`,`errors`,"
//            " `errors_by_action`, `errors_by_action_selected`) "
//            "VALUES (?,?,?,?,?,?)"            
//            );
//
//    prep_stmt -> setInt(1, numClusters);
//    prep_stmt -> setInt(2, compressionRate);
//    prep_stmt -> setInt(3, ValidationRate);  
//    prep_stmt -> setInt(4, errors);  
//    prep_stmt -> setString(5, errorsByAct.substr(0, errorsByAct.size() - 1));
//    prep_stmt -> setString(6, errorsByActSel.substr(0, errorsByActSel.size() - 1));
//    
//    prep_stmt->executeUpdate();
//    
//    delete prep_stmt;
// }

//InferenceClassInterface *getInferenceModel(){
//    
//    InferenceClassInterface *inferenceObj;
//      if(InferenceClassUsed =="HMM"){
//            inferenceObj= new HMM;                
//      }        
//    return inferenceObj;
//}

void HMM::save2DB(){
    connectSQL();
    
    cout<<"Saving the HMM params infered"<<endl;
    
     PreparedStatement *prep_stmt;
    prep_stmt = con -> prepareStatement(            
            "INSERT INTO `HMMDS`(`num_clusters`, `compression_rate`, `id_action`, `state_trans`, `initial_state_dist`, `obs_prob_dist`, `cluster_distribution`, `distance_measure`)"
            " VALUES (?,?,?,?,?,?,?,?)"
            );

    std::ostringstream sClustDist;
    for (int j = 0; j < numClusters; j++) {
        sClustDist << " " << clustDist[j].mean.idCluster;
        sClustDist << " " << clustDist[j].mean.height;
         for (int k = 0; k < Num_Angles; k++) {
                sClustDist << " " << clustDist[j].mean.angles[k];
         }
        sClustDist << " " <<clustDist[j].standardDeviation;      
    }       
    
    std::ostringstream sStateTrans;
    for (int i = 0; i < numClusters; i++) {
        for (int j = 0; j < numClusters; j++) {
            sStateTrans << " " << stateTrans[i][j];
        } 
    }
    std::ostringstream sInitState;
    for (int j = 0; j < numClusters; j++) {
        sInitState << " " << initialStateDist[j];
    }   
    
    std::ostringstream sObsProbDist;
    for (int i = 0; i < numClusters; i++) {
        for (int j = 0; j < numClusters; j++) {
            sObsProbDist << " " << obsProbDist[i][j];
        } 
    }
    
    prep_stmt -> setInt(1, numClusters);        
    prep_stmt -> setInt(2, compressionRate);    
    prep_stmt -> setInt(3, idAction);  
    prep_stmt -> setString(4, sStateTrans.str());
    prep_stmt -> setString(5, sInitState.str());
    prep_stmt -> setString(6, sObsProbDist.str());
    prep_stmt -> setString(7, sClustDist.str());
    prep_stmt -> setString(8, DistanceMeasureUsed);
    
    prep_stmt-> executeUpdate();   
    delete prep_stmt;     
    
    disconnectSQL();
}

bool HMM::readDB(int idAction){ 
    connectSQL();
    
    cout<<"Reading HMMData for action:"<<idAction<<endl;
    this->idAction=idAction;
    
    ResultSet *resSet;
    PreparedStatement *prep_stmt;

    /* create a statement object */
    prep_stmt = con -> prepareStatement("SELECT * FROM `HMMDS` WHERE `compression_rate`=? AND `num_clusters`=? AND `id_action`=? AND `distance_measure`=?");
    prep_stmt->setInt(1, compressionRate);  
    prep_stmt->setInt(2, numClusters);  
    prep_stmt->setInt(3, idAction);    
    prep_stmt -> setString(4, DistanceMeasureUsed);
    resSet = prep_stmt->executeQuery();

    bool dataExists=false;
    while (resSet->next()) {
        dataExists=true;
        std::stringstream state_trans(resSet->getString("state_trans"));
        std::stringstream initial_state_dist(resSet->getString("initial_state_dist"));
        std::stringstream obs_prob_dist(resSet->getString("obs_prob_dist"));
        std::stringstream cluster_distribution(resSet->getString("cluster_distribution"));
        
        stateTrans.resize(numStates, vector<double>(numStates, 0));
        initialStateDist.resize(numStates, 0);
        obsProbDist.resize(numStates, vector<double>(numStates,0));
        clustDist.resize(numStates);
       
        for (int i = 0; i < numClusters; i++) {
            for (int j = 0; j < numClusters; j++) {
                state_trans >> stateTrans[i][j];
            }
        }
         
        for (int j = 0; j < numClusters; j++) {
            initial_state_dist >> initialStateDist[j];
        }
           
        for (int i = 0; i < numClusters; i++) {
            for (int j = 0; j < numClusters; j++) {
                obs_prob_dist>> obsProbDist[i][j];
            }
        }       
             
        for (int j = 0; j < numClusters; j++) {
            ClusterDistribution cluster;           
            cluster_distribution >> cluster.mean.idCluster;            
            cluster_distribution >> cluster.mean.height;              
//            &(cluster.mean.angles[0])= angles;
            for (int k = 0; k < Num_Angles; k++) {
                cluster_distribution >> cluster.mean.angles[k];
            }             
            cluster_distribution >> cluster.standardDeviation;           
            clustDist.push_back(cluster);
        }        
    }
    
    delete resSet;
    delete prep_stmt;
    disconnectSQL();
    return dataExists;
}