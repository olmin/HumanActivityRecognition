/* 
 * File:   main.cpp
 * Author: olmin
 *
 * Created on 27 de octubre de 2011, 12:38
 */

/*local includes*/
#include <libio.h>

#include "../Clustering/config.h"
#include "../Clustering/Commons.h"
#include "../Clustering/BehaviourSequence.h"
#include "../Clustering/ClusterData.h"
#include "ClusterDistribution.h"
#include "KFoldCrossValidation.h"
#include "HMM.h"
#include "../BehaviourInferenceOlmoAutomata/OlmoAutomata.h"

using namespace std;
using namespace sql;
   
Driver *driver;
    Connection *con;
    Savepoint *savept;  
    
/*
 * 
 */
void connectSQL(){
       con=driver -> connect(DBHOST, USER, PASSWORD);
     /* turn off the autocommit */
        con -> setAutoCommit(0);

        /* select appropriate database schema */
        con -> setSchema(DATABASE);
        savept = con -> setSavepoint("SAVEPT1");               
}
 
 void disconnectSQL(){
       con -> releaseSavepoint(savept);
        cout << "\tCommitting outstanding updates to the database .." << endl;
        con -> commit();

        /* Clean up */
        con -> close();
 }

int main(int argc, char** argv) {

//    Driver *driver;
//    Connection *con;
//    Savepoint *savept;     
    
     
    try {
        driver = get_driver_instance();

        /* create a database connection using the Driver */
       
       connectSQL();// -> connect(DBHOST, USER, PASSWORD);

        /* alternate syntax using auto_ptr to create the db connection */
        //auto_ptr  con (driver -> connect(url, user, password));

//        /* turn off the autocommit */
//        con -> setAutoCommit(0);
//
//        /* select appropriate database schema */
//        con -> setSchema(DATABASE);
//        savept = con -> setSavepoint("SAVEPT1");

        
        /***************************START**************************************************/

        //Before anything set the global vars correctly
       /* initiate variables */
        numClusters = (argc >= 2 ? atoi(argv[1]) : Num_Clusters);
        compressionRate = (argc >= 3 ? atoi(argv[2]) : CompressionRate);
        numStates =  numClusters;
        
        cout << "Executing with Num_Clusters: " << numClusters
                << ", Num_States: " << numStates 
                << ", and CompressionRate: " << compressionRate 
                << endl;
        
   // First we get the clusters distribution
        //we get the clusters from DB
        SimpleSkeleton newSkel;
        vector<SimpleSkeleton> clusters(numClusters,newSkel);       
        getClustersFromDB(con, &clusters);
        disconnectSQL();
//        SimpleSkeleton clusters[numClusters];
//        getClustersFromDB(con, clusters);
              
        //Creating and Initializing one HMM for each action
         vector<InferenceClassInterface*> models;
          vector<InferenceClassInterface*> bestModels;
//        vector<HMM> models;
//        vector<HMM> bestModels; 
       
               //this will hold all the training data, to test error after the HMM creation
        list<BehaviourSequence> dataTrainingGlobal;
        list<BehaviourSequence>::iterator itTraining;
        list<BehaviourSequence> dataTestGlobal;
        list<BehaviourSequence>::iterator itTest;
        
        vector<int> errorsTrainByAction(Num_Actions,0);
        vector<vector<int> > errorsTrainByActionSel(Num_Actions, vector<int>(Num_Actions, 0));
        vector<int> errorsTestByAction(Num_Actions,0);
        vector<vector<int> > errorsTestByActionSel(Num_Actions, vector<int>(Num_Actions, 0));
        vector<int> errorsValidationByAction(Num_Actions,0);
        vector<vector<int> > errorsValidationByActionSel(Num_Actions, vector<int>(Num_Actions, 0));
        
//        vector<vector<int> > errorsByUser(Num_Users, vector<int>(Num_Actions,0));
//        vector<vector<double> > globalErrorsByUser(Num_Users,vector<double>(Num_Actions, 0));
         
        vector<int> globalErrorsTrainByAction(Num_Actions,0);
        vector<vector<int> > globalErrorsTrainByActionSel(Num_Actions, vector<int>(Num_Actions, 0));
        vector<int> globalErrorsTestByAction(Num_Actions,0);
        vector<vector<int> > globalErrorsTestByActionSel(Num_Actions, vector<int>(Num_Actions, 0));
         
        vector<int> globalErrorsValidationByAction(Num_Actions,0);
        vector<vector<int> > globalErrorsValidationByActionSel(Num_Actions, vector<int>(Num_Actions, 0));
        
        vector<int> globalErrorsByAction(Num_Actions,0);
        vector<vector<int> > globalErrorsByActionSel(Num_Actions, vector<int>(Num_Actions, 0));
//        
//        vector<double> avgPoints(Num_Actions,0);
//        vector<double> avgErr(Num_Actions,0);
        
        vector< vector <int> > allSequences;               
        
          
            int bestErrorValidation=9999;
            int bestGlobalErrorTrain=9999;          
            int worstErrorValidation = 0;
            int worstErrorTrain = 0;
            double meanErrorValidation = 0;
            double meanErrorTrain = 0;
            
            string idsTrainingSel;

            srand(time(NULL));   

            list<BehaviourSequence> dataValidation;        
            list<BehaviourSequence>::iterator itValidation;
            int iterations;
            for(iterations=0;iterations<Max_Inference_Iterations;iterations++){
              
                int bestErrorTest = 9999;
                int bestErrorTrain = 9999;
               
                
                 dataValidation.clear();
                 connectSQL();
                 vector<vector< vector<BehaviourSequence> > > kFoldsVector= getKFoldsVector(con, &dataValidation); 
                 disconnectSQL();
                 errorsValidationByAction.clear();
                 errorsValidationByAction.resize(Num_Actions,0);     
                 errorsValidationByActionSel.clear();
                 errorsValidationByActionSel.resize(Num_Users,vector<int>(Num_Actions,0));  

                for(int k=0;k<K_Fold;k++){    
                    dataTrainingGlobal.clear();
                    dataTestGlobal.clear();
                    models.clear();
                    errorsTrainByAction.clear();
                    errorsTrainByAction.resize(Num_Actions,0);     
                    errorsTrainByActionSel.clear();
                    errorsTrainByActionSel.resize(Num_Users,vector<int>(Num_Actions,0));       
                    errorsTestByAction.clear();
                    errorsTestByAction.resize(Num_Actions,0);     
                    errorsTestByActionSel.clear();
                    errorsTestByActionSel.resize(Num_Users,vector<int>(Num_Actions,0));  
//                    errorsByUser.clear();
//                    errorsByUser.resize(Num_Users,vector<int>(Num_Actions,0));

                for (int idAction = 0; idAction < Num_Actions; idAction++) {
                    
                    vector< vector <int> > allSequences;
                    string dataset = (InferenceClassUsed == "HMM") ? "HMMTrainingSet" : "OlmoTrainingSet";
                   // list<BehaviourSequence> dataTraining = getTrainedData(con, idAction, dataset);

                      list<BehaviourSequence> dataTraining=            
                        setTrainingAndTestKFold(kFoldsVector[idAction], k,&dataTrainingGlobal,&dataTestGlobal);        
                      
                    InferenceClassInterface *inferenceObj;

                    if (InferenceClassUsed == "OlmoAutomata") {
                        //Create the automata with the training data
                        allSequences.clear();
                        for (list<BehaviourSequence>::iterator itTraining = dataTraining.begin(); itTraining != dataTraining.end(); itTraining++) {
                            allSequences.push_back(itTraining->sequence);
                        }

                        inferenceObj = new OlmoAutomata(allSequences);

                    } else if(InferenceClassUsed == "HMM"){
                        //is HMM                
                        inferenceObj = new HMM(numStates, clusters, &dataTraining);
                    }

                    //save the model for the behaviour "i"
                    models.push_back(inferenceObj);                                              

//
//                        //Create the automata with the training data
//        //                allSequences.clear();                               
//        //                for (itTraining = dataTraining.begin(); itTraining != dataTraining.end(); itTraining++) {                 
//        //                    allSequences.push_back(itTraining->sequence);                 
//        //                }              
//
//                        /*Creating the hmm*/                       
//                        HMM hmm(numStates, clusters, &dataTraining);  
//
//        //                stringstream name;
//        //                name << "automata" << i;
//        //                automata.createDotFile(name.str());
//
//                        //save the model for the behaviour "i"
//                        models.push_back(hmm);                 
                    }


                    int errorTrain=0;
                    for(itTraining=dataTrainingGlobal.begin(); itTraining!=dataTrainingGlobal.end();itTraining++){

                        int selectedAction= compareModels(models, &(itTraining->sequence));
                       
//                        selectedAction=compareHMM(models, &(itTraining->sequence));
                       
                          if(selectedAction!=-1)
                                errorsTrainByActionSel[itTraining->idAction][selectedAction]++;

                        if(selectedAction==-1  || selectedAction!=itTraining->idAction){
                            errorTrain++;     
                            errorsTrainByAction[itTraining->idAction]++;
        //                    if(selectedAction!=-1)
        //                        errorsTrainByActionSel[itTraining->idAction][selectedAction]++;

//                            errorsByUser[itTraining->idUser][itTraining->idAction]++;
                        }

                    }
                    int errorTest=0;
                    for(itTest=dataTestGlobal.begin(); itTest!=dataTestGlobal.end();itTest++){

                       int selectedAction= compareModels(models, &(itTest->sequence));
//                        if(InferenceClassUsed == "HMM"){
//                            selectedAction=compareHMM(models, &(itTest->sequence));
//                        }else if(InferenceClassUsed=="OlmoAutomata"){
//                            compareAutomatas(models, itTest->sequence);
//                        }

                        if(selectedAction!=-1)
                                errorsTestByActionSel[itTest->idAction][selectedAction]++; 

                        if(selectedAction== -1 || selectedAction!=itTest->idAction){
                            errorTest++;               
                            errorsTestByAction[itTest->idAction]++;
        //                     if(selectedAction!=-1)
        //                        errorsTestByActionSel[itTest->idAction][selectedAction]++; 

//                            errorsByUser[itTest->idUser][itTest->idAction]++;
                        }

                    }
             
//
//                    for (int idUser = 1; idUser < Num_Users; idUser++) {               
//                        for (int idAction = 0; idAction < Num_Actions; idAction++) {
//                            globalErrorsByUser[idUser][idAction]+=errorsByUser[idUser][idAction];
//                        }
//                    }
                   
                    /******** BEST RESULTS*********/
        //            if ((errorTest + errorTrain) <= (bestErrorTest + bestErrorTrain)) {
                        if (errorTest+errorTrain < bestErrorTest+bestErrorTrain || 
                                (errorTest+errorTrain == bestErrorTest+bestErrorTrain && errorTest<bestErrorTest)) {

                            cout<<"Better result, "<<errorTrain<<"/"<<errorTest<<endl;

                            bestErrorTest = errorTest;
                            bestErrorTrain = errorTrain;
                            for (int idAction = 0; idAction < Num_Actions; idAction++) {
                                //train
                                globalErrorsTrainByAction[idAction] = errorsTrainByAction[idAction];                       
                                //test
                                globalErrorsTestByAction[idAction] = errorsTestByAction[idAction];
                                for (int idActionSel = 0; idActionSel < Num_Actions; idActionSel++) {
                                    globalErrorsTrainByActionSel[idAction][idActionSel] = errorsTrainByActionSel[idAction][idActionSel];
                                    globalErrorsTestByActionSel[idAction][idActionSel] = errorsTestByActionSel[idAction][idActionSel];
                                }
                            }
                            idsTrainingSel = getIdsTraining(&dataTrainingGlobal);                        
                            bestModels=models;
                            if(bestErrorTest+bestErrorTrain==0)//exit loop k-fold
                                break;
                        }
        //            }           
                }//end loop training KFold


//             for (int idUser = 1; idUser < Num_Users; idUser++) {               
//                    for (int idAction = 0; idAction < Num_Actions; idAction++) {
//                        globalErrorsByUser[idUser][idAction]/=(double)K_Fold;
//                    }
//                }

                //Now we check the validation data with the best result
                int errorValidation=0;
                for(itValidation=dataValidation.begin(); itValidation!=dataValidation.end();itValidation++){

                       int selectedAction= compareModels(bestModels, &(itValidation->sequence));
//                        if(InferenceClassUsed == "HMM"){
//                            selectedAction=compareHMM(bestModels, &(itValidation->sequence));
//                        }else if(InferenceClassUsed=="OlmoAutomata"){
//                            compareAutomatas(bestModels, itValidation->sequence);
//                        }
                     
                    if(selectedAction!=-1)
                            errorsValidationByActionSel[itValidation->idAction][selectedAction]++; 

                    if(selectedAction== -1 || selectedAction!=itValidation->idAction){
                        errorValidation++;               
                        errorsValidationByAction[itValidation->idAction]++;
    //                     if(selectedAction!=-1)
    //                        errorsValidationByActionSel[itValidation->idAction][selectedAction]++; 

//                        errorsByUser[itValidation->idUser][itValidation->idAction]++;
                    }
                }
                
                int globalErrorTrain=bestErrorTest+bestErrorTrain;
                   
                /******** MEAN RESULTS*********/
                meanErrorValidation+=errorValidation;
                meanErrorTrain+=globalErrorTrain;
                /******** WORST RESULTS*********/
                if (errorValidation>worstErrorValidation)
                    worstErrorValidation = errorValidation;    
                if (globalErrorTrain> worstErrorTrain)
                    worstErrorTrain = globalErrorTrain;
                

 cout << "              result Validation, " << globalErrorTrain << "/" << errorValidation << endl;
 
                if (errorValidation < bestErrorValidation || (errorValidation == bestErrorValidation && globalErrorTrain < bestGlobalErrorTrain)) {

                    cout << "Better result Validation, " << globalErrorTrain << "/" << errorValidation << endl;

                    bestErrorValidation = errorValidation;
                    bestGlobalErrorTrain = globalErrorTrain;
                    for (int idAction = 0; idAction < Num_Actions; idAction++) {
                        //train
                        globalErrorsByAction[idAction] = globalErrorsTrainByAction[idAction]+globalErrorsTestByAction[idAction];
                        //validation
                        globalErrorsValidationByAction[idAction] = errorsValidationByAction[idAction];
                        for (int idActionSel = 0; idActionSel < Num_Actions; idActionSel++) {
                            globalErrorsByActionSel[idAction][idActionSel] = globalErrorsTrainByActionSel[idAction][idActionSel]+globalErrorsTestByActionSel[idAction][idActionSel];
                            globalErrorsValidationByActionSel[idAction][idActionSel] = errorsValidationByActionSel[idAction][idActionSel];
                        }
                    }
    //                idsTrainingSel = getIdsTraining(&dataTrainingGlobal);
                    if(bestErrorValidation+bestGlobalErrorTrain==0)//exit loop k-fold
                                break;
                }    
               
            }//end loop Inference iterations
            
             //get the mean
             meanErrorValidation/=(double)iterations;
             meanErrorTrain/=(double)iterations;

             //saving results
             connectSQL();
             saveTrainingData(con, idsTrainingSel, 
                     bestGlobalErrorTrain, bestErrorValidation,
                     worstErrorTrain, worstErrorValidation,
                     meanErrorTrain, meanErrorValidation,
                     &globalErrorsByAction, &globalErrorsByActionSel,
                     &globalErrorsValidationByAction, &globalErrorsValidationByActionSel);

             cout<<endl<<
                     "The best solution was:Error of Training: "<<
                    bestGlobalErrorTrain<<"/"<<dataTrainingGlobal.size()+dataTestGlobal.size()
                        <<", and error of validation: "<<bestErrorValidation<<"/"<<dataValidation.size()<<
                        endl;

             cout<<"mean error train: "<<meanErrorTrain<<", mean error validation: "<< meanErrorValidation<< endl;


         
        /****************************END*************************************************/
        
         disconnectSQL();
//        con -> releaseSavepoint(savept);
//        cout << "\tCommitting outstanding updates to the database .." << endl;
//        con -> commit();
//
//        /* Clean up */
//        con -> close();
        delete con;

    } catch (SQLException &e) {
        cout << "ERROR: SQLException in " << __FILE__;
        cout << " (" << __func__ << ") on line " << __LINE__ << endl;
        cout << "ERROR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << ")" << endl;

        cout << "\tRolling back until the last save point \"SAVEPT1\" .." << endl;
        con -> rollback(savept);

        if (e.getErrorCode() == 1047) {
            /*
            Error: 1047 SQLSTATE: 08S01 (ER_UNKNOWN_COM_ERROR)
            Message: Unknown command
             */
            cout << "\nYour server does not seem to support Prepared Statements at all. ";
            cout << "Perhaps MYSQL < 4.1?" << endl;
        }

        return EXIT_FAILURE;
    } catch (std::runtime_error &e) {

        cout << "ERROR: runtime_error in " << __FILE__;
        cout << " (" << __func__ << ") on line " << __LINE__ << endl;
        cout << "ERROR: " << e.what() << endl;

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;       
}

