/* 
 * File:   Common.cpp
 * Author: olmin
 * 
 * Created on 29 de enero de 2013, 12:06
 */

#include "Common.h"

void openMysqlSession(){
    /* create a database connection using the Driver */
        con = driver -> connect(DBHOST, USER, PASSWORD);

        /* alternate syntax using auto_ptr to create the db connection */
        //auto_ptr  con (driver -> connect(url, user, password));

        /* turn off the autocommit */
        con -> setAutoCommit(0);

        /* select appropriate database schema */
        con -> setSchema(DATABASE);
        savept = con -> setSavepoint("SAVEPT1");
    
}

void closeMysqlSession(){
       con -> releaseSavepoint(savept);
       cout << "\tCommitting outstanding updates to the database .." << endl;
       con -> commit();

       /* Clean up */
       con -> close();
}

void saveResults(vector<vector<int> > errors, int dataType) {
    openMysqlSession();
    
    cout<<"Saving results for dataType:"<<dataType<<endl;

    PreparedStatement *prep_stmt;
    prep_stmt = con -> prepareStatement(
            "INSERT INTO `DetectionResults`(`inference_type`, `data_type`, `compression_rate`, `num_clusters`, `errors_by_action`, `errors_by_action_selected`) "
            "VALUES (?,?,?,?,?,?)"
            );

    std::ostringstream errorsByAction, errorsByActionSelected;
    for (int j = 0;j< Num_Actions; j++) {
        errorsByAction << " " << errors[0][j];
        errorsByActionSelected << " " << errors[1][j];
    }
    

    prep_stmt -> setString(1, INFERENCE_CLASS);
    prep_stmt -> setInt(2, dataType);
    prep_stmt -> setInt(3, compressionRate);
    prep_stmt -> setInt(4, numClusters);
    prep_stmt -> setString(5, errorsByAction.str());
    prep_stmt -> setString(6, errorsByActionSelected.str());
    prep_stmt-> executeUpdate();
    delete prep_stmt;
    
    closeMysqlSession();
}

vector<vector<int> > classify(vector<InferenceClassInterface*> models, SimpleDataSet data){    
     vector< vector<int> > errors;
     errors.resize(2, vector<int>(Num_Actions,0));
     
     for(int i=0; i< data.dataSet.size();i++){
                int actionSelected=compareModels(models,data.dataSet[i].sequence);                
                if(actionSelected!=data.dataSet[i].idAction){
                    errors[1][actionSelected-1]++;
                    errors[0][data.dataSet[i].idAction-1]++;
                }
     }     
     return errors;
}
int compareModels(vector<InferenceClassInterface*> models, vector<int> behSeq) {
    int selAction;
    double maxProb = -10e+10;

    for (int i = 0; i < Num_Actions; i++) {
        models[i]->checkNewSequence(behSeq);
        double prob = models[i]->getActualProbability();
        //cout<<"Prob idBeh:"<<behaviour->front().idBehaviour <<",model: "<< i << ",prob: "<<prob<<endl;
        if (maxProb < prob) {
            maxProb = prob;
            selAction = models[i]->getIdAction();
        }
    }
    return selAction;
}

InferenceClassInterface* getInferenceObject(){
    
    InferenceClassInterface *inferenceObj;
      if(INFERENCE_CLASS =="HMM"){
            inferenceObj= new HMMDataSet;                
      }else if(INFERENCE_CLASS =="OlmoAutomata"){
          inferenceObj= new OlmoAutomata;                       
      }            
    return inferenceObj;
}


void createInferenceHMMData(vector<InferenceClassInterface*> *models, ClusterData clusters, HarusDataSet mainDataSet) {

    assignClusters2Skeletons(clusters.clusters, &mainDataSet);
    vector<HarusDataSet> dataSplit = splitActions(mainDataSet);

    for (int i = 0; i < Num_Actions; i++) {               
        HMMDataSet* inference = new HMMDataSet;
        inference->startInference(clusters, dataSplit[i]);
        inference->save();
        models->push_back(inference);
    }
}

void createInferenceOlmoAutomataData(vector<InferenceClassInterface*> *models,SimpleDataSet trainDataSet){

    vector<SimpleDataSet> dataSplit = splitActions(trainDataSet);
    
    for (int i = 1; i <= Num_Actions; i++) {
        vector<vector<int> > rawData = getRawData(dataSplit[i - 1]);
        cout << "DAta for action:" << i << ", " << rawData.size() << endl;
        OlmoAutomata* inference = new OlmoAutomata(rawData);
        inference->setIdAction(i);
        inference->save();
        models->push_back(inference);
    }
}

void createInferenceData(vector<InferenceClassInterface*> *models, ClusterData clusters, HarusDataSet mainDataSet, SimpleDataSet trainDataSet){
    
    if(INFERENCE_CLASS =="HMM"){
            createInferenceHMMData(models, clusters, mainDataSet);                
      }else if(INFERENCE_CLASS =="OlmoAutomata"){
          createInferenceOlmoAutomataData(models, trainDataSet);                       
      }         
    
}