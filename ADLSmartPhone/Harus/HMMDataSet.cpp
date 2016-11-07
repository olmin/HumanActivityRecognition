/* 
 * File:   HMMDataSet.cpp
 * Author: olmin
 * 
 * Created on 29 de enero de 2013, 11:51
 */

#include "HMMDataSet.h"
#include "Common.h"

void HMMDataSet:: startInference(ClusterData clusters, HarusDataSet data){
    this->idAction= data.dataSet[0].idAction;
    
    cout<<"Initializing inference HMM for action:"<<idAction<<", data size:"<<data.dataSet.size()<<endl;    
    
     this->actualProb = 0;
    //this->numStates = numStates;
    //calculate the clusters distribution for that behaviour
    this->clustDist = getClustersDistribution(clusters, &data);
    stateTrans.resize(numStates, vector<double>(numStates, 0));
    initialStateDist.resize(numStates, 0);
    obsProbDist.resize(numStates, vector<double>(numStates,0));
    fwdVars.resize(numStates, 0);   
    
    this->initialize(&data);    
}


void HMMDataSet::save(){
    openMysqlSession();
    
    cout<<"Saving the HMM params infered"<<endl;
    
     PreparedStatement *prep_stmt;
    prep_stmt = con -> prepareStatement(            
            "INSERT INTO `HMMDataSet`(`num_clusters`, `compression_rate`, `id_action`, `state_trans`, `initial_state_dist`, `obs_prob_dist`, `cluster_distribution`)"
            " VALUES (?,?,?,?,?,?,?)"
            );

    std::ostringstream sClustDist;
    for (int j = 0; j < numClusters; j++) {
        sClustDist << " " << clustDist[j].mean.idCluster;
         for (int k = 0; k < Vector_Size; k++) {
                sClustDist << " " << clustDist[j].mean.center[k];
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

    prep_stmt-> executeUpdate();   
    delete prep_stmt;     
    
    closeMysqlSession();
}

void HMMDataSet::read(int idAction){ 
    openMysqlSession();
    
    cout<<"Reading HMMData for action:"<<idAction<<endl;
    this->idAction=idAction;
    
    ResultSet *resSet;
    PreparedStatement *prep_stmt;

    /* create a statement object */
    prep_stmt = con -> prepareStatement("SELECT * FROM `HMMDataSet` WHERE `compression_rate`=? AND `num_clusters`=? AND `id_action`=?");
    prep_stmt->setInt(1, compressionRate);  
    prep_stmt->setInt(2, numClusters);  
    prep_stmt->setInt(3, idAction);    
    resSet = prep_stmt->executeQuery();

    while (resSet->next()) {
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
             cluster.mean.center.resize(Vector_Size);
            for (int k = 0; k < Vector_Size; k++) {
                cluster_distribution >> cluster.mean.center[k];
            }             
            cluster_distribution >> cluster.standardDeviation;           
            clustDist.push_back(cluster);
        }        
    }
    
    delete resSet;
    delete prep_stmt;
    closeMysqlSession();
    
}


/*! Initializes de HMM: the initialStateDistribution, the stateTransition matrix and the observation probabilistic distribution */
void HMMDataSet::initialize(HarusDataSet *dataSeq) {
    
    setStateTransition(dataSeq);
    setInitialStateDist(dataSeq);
    setObsProbDist(dataSeq);

    vector<int> observations;
    for(int i=0; i< dataSeq->dataSet.size();i++){   
        sequenceLength=0;
        for(int j=0;j<dataSeq->dataSet[i].idsCluster.size();j++){
           observations.push_back(dataSeq->dataSet[i].idsCluster[j]); 
           sequenceLength++;
        }
    }     
    sequenceWindow.resize(sequenceLength,-1);
       
     execBaumWelch(&observations);                 
     //now we see the lowest prob to belong to the cluster
     double lowest=0;
    for(int i=0; i< dataSeq->dataSet.size();i++){   
         setObservationsAlpha(&(dataSeq->dataSet[i].idsCluster));
         double actual=getActualProbability();
         if(actual<lowest)
             lowest=actual;
     }
     thresholdBelong=lowest;  
}

void HMMDataSet::setStateTransition(HarusDataSet *dataSeq) {
 
     for(int i=0; i< dataSeq->dataSet.size();i++){          
        int size = dataSeq->dataSet[i].idsCluster.size() - 1;        
        for (int j = 0; j < size; j++) {
            //every time we from i to (i+1) we sum one             
            stateTrans[dataSeq->dataSet[i].idsCluster[j]][dataSeq->dataSet[i].idsCluster[j + 1]]++;            
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

void HMMDataSet::setInitialStateDist(HarusDataSet *dataSeq) {
    //initialStateDist.resize(numStates, 0);
    int count = 0;

    for(int i=0; i< dataSeq->dataSet.size();i++){   
        initialStateDist[dataSeq->dataSet[i].idsCluster[0]]++; //we take the first symbol  
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

void HMMDataSet::setObsProbDist(HarusDataSet *dataSeq) {
    //obsProbDist.resize(numStates, vector<double>(numStates,0));
        
    vector<int> count(numStates,0);
  
    for(int i=0; i< dataSeq->dataSet.size();i++){          
        for (int j = 0; j < numStates ; j++) {
           
            //every time we from i to (i+1) we sum one             
            for(int k=0;k<dataSeq->dataSet[i].sequence.size() - 1;k++){
                float prob=clustDist[j].getProbBelong(dataSeq->dataSet[i].sequence[k]);
                obsProbDist[j][dataSeq->dataSet[i].idsCluster[k]]+=prob;
                
               // cout<<"(i,j)= ("<<it->sequence[i]<<","<<j<<")   prob:"<<prob<<endl;
                count[dataSeq->dataSet[i].idsCluster[k]]++;                
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

/*!Iterates the HMM after the observations*/
void HMMDataSet::setObservations(HarusData *observation) {
//    setObservationsAlpha(observation);
    //init
    actualProb = 0;
    //then we iterate for each frame 
    
    //step 1, initializa
    setInitialForwdVar(observation->sequence[0]);
       //step 2, iterate for every Observation
    for (int i=1; i< observation->sequence.size(); i++) {
        for (int actualState = 0; actualState < numStates; actualState++){
            fwdVars[actualState] = iterateForwdVar(observation->sequence[i], actualState);
        }
    }

    //step 3, final probability    
    for (int i = 0; i < numStates; i++) {       
        actualProb += fwdVars[i];
    }   
  //  cout<<endl<<"ActualProb: "<<actualProb<< ", idbeh: "<<observation->front().idBehaviour <<endl;
}

void HMMDataSet::setInitialForwdVar(vector<float> dataObs) {
    //fwdVars.resize(numStates, 0);
    for (int i = 0; i < numStates; i++) {
        fwdVars[i] = initialStateDist[i] * clustDist[i].getProbBelong(dataObs);
    }
}
double HMMDataSet::iterateForwdVar(vector<float> dataObs, int actualState) {
//    vector<double> newVars;
//    newVars.resize(numStates, 0);
    double agr = 0;
    for (int i = 0; i < numStates; i++) {
        agr += fwdVars[i] * stateTrans[i][actualState];
    }
   
    return agr * clustDist[actualState].getProbBelong(dataObs);
}

void HMMDataSet::checkNewSequence(vector<int> sequence){
     setFwdVars(&sequence);
}

/*!Iterates the HMM after the observations*/
void HMMDataSet::setObservationsAlpha(vector<int> *observation) {
    setFwdVars(observation);
}

void HMMDataSet::setFwdVars(vector<int> *observations) {
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

void HMMDataSet::setBackwVars(vector<int> *observations) {
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


vector<vector<double> > HMMDataSet::getGamma(vector<int> *observations) {

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
    
double*** HMMDataSet::getEpsilon(vector<int> *observations) {
    
    double ***epsilon;//(observations->size(),
                                             //  vector<vector<double> >(numStates, vector<double>(numStates, 0)));   

    // Allocate memory
    epsilon = new double**[observations->size()];
    for (int i = 0; i < observations->size(); ++i) {
        epsilon[i] = new double*[numStates];
        for (int j = 0; j < numStates; ++j)
            epsilon[i][j] = new double[numStates];
    }
     
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

bool HMMDataSet::execBaumWelch(vector<int> *observations) {
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
        double ***epsilon= getEpsilon(observations);        
                        
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
        
         // De-Allocate memory to prevent memory leak
        for (int i = 0; i < observations->size(); ++i) {
            for (int j = 0; j < numStates; ++j)
                delete [] epsilon[i][j];

            delete [] epsilon[i];
        }
        delete [] epsilon;
    }
 // cout<<"BaumWelch finished init and final prob:"<<initProb<<", "<<actualProb<<endl;
}

//int compareHMM(vector<HMMDataSet> models, SimpleDataSet *behSeq) {
//    int selAction;
//    double maxProb = 0;
//
//    for (int i = 0; i < Num_Actions; i++) {
//        models[i].setObservations(behSeq);
//        double prob = models[i].getActualProbability();
//        //cout<<"Prob idBeh:"<<behaviour->front().idBehaviour <<",model: "<< i << ",prob: "<<prob<<endl;
//        if (maxProb < prob) {
//            maxProb = prob;
//            selAction = i;
//        }
//    }
//    return selAction;
//}

int compareHMM(vector<HMMDataSet> models, vector<int> behSeq) {
    int selAction;
    double maxProb = -10e+10;

    for (int i = 0; i < Num_Actions; i++) {
        models[i].checkNewSequence(behSeq);
        double prob = models[i].getActualProbability();
        //cout<<"Prob idBeh:"<<behaviour->front().idBehaviour <<",model: "<< i << ",prob: "<<prob<<endl;
        if (maxProb < prob) {
            maxProb = prob;
            selAction = models[i].getIdAction();
        }
    }
    return selAction;
}

//
//
// void saveTrainingData(Connection *con, string idsTraining, 
//         int bestErrorTrain, int bestErrorTest,
//         int worstErrorTrain, int worstErrorTest,
//         int meanErrorTrain, int meanErrorTest,
//         vector<int> *errorsTrainByAction, vector<int> *errorsTrainByActionSel,
//         vector<int> *errorsTestByAction, vector<int> *errorsTestByActionSel){
//     
//    PreparedStatement *prep_stmt;
//    std::ostringstream s1,s2,s3,s4;
//    
//    for(int i=0;i<Num_Actions;i++){
//        s1 << (*errorsTrainByAction)[i] << ",";
//        s2 << (*errorsTrainByActionSel)[i] << ",";
//        s3 << (*errorsTestByAction)[i] << ",";
//        s4 << (*errorsTestByActionSel)[i] << ",";
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
//    prep_stmt -> setInt(9, meanErrorTrain);
//    prep_stmt -> setInt(10, meanErrorTest);
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
 
 void HMMDataSet::addNewState(int newState){    
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


bool HMMDataSet::isFinalState(){   
//    cout<<"Final?:"<<actualProb<<endl;
    return actualProb>=thresholdBelong;
}