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
#include "../Inference_global/ClusterDistribution.h"
#include "../Inference_global/KFoldCrossValidation.h"

#include "configInference.h"
#include <sys/time.h>

using namespace std;

int main(int argc, char** argv) {

//    Driver *driver;
//    Connection *con;
//    Savepoint *savept;     
    
     
    try {      
       connectSQL();// -> connect(DBHOST, USER, PASSWORD);

        /***********************/

        //Before anything set the global vars correctly
       /* initiate variables */
        numClusters = (argc >= 2 ? atoi(argv[1]) : Num_Clusters);
        compressionRate = (argc >= 3 ? atoi(argv[2]) : CompressionRate);
        numStates =  numClusters;
        
        cout << "Executing with Num_Clusters: " << numClusters
                << ", Num_States: " << numStates 
                << ", and CompressionRate: " << compressionRate 
                << ", and DistanceMeasureUsed: " << DistanceMeasureUsed 
                << ", and InferenceClassUsed: " << InferenceClassUsed 
                << ", and DATABASE: " << DATABASE
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

        vector<double> timeLearning;
        timeLearning.resize(Num_Actions,0);        
        vector<int> learnCount;
        learnCount.resize(Num_Actions,0);
        vector<double> timeDetection;
        timeDetection.resize(Num_Actions,0);
        vector<int> detectCount;
        detectCount.resize(Num_Actions,0);
        timeval stop, start;       
        
               //this will hold all the training data, to test error after the HMM creation
        list<BehaviourSequence> dataTrainingGlobal;
//        list<BehaviourSequence>::iterator itTraining;
        list<BehaviourSequence> dataTestGlobal;
//        list<BehaviourSequence>::iterator itTest;
        
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
        
          
            double bestErrorValidation=0;
            double bestGlobalErrorTrain=0;          
            double worstErrorValidation = 1;
            double worstErrorTrain = 1;
            double meanErrorValidation = 0;
            double meanErrorTrain = 0;
            
            string idsTrainingSel;

            srand(time(NULL));   

            list<BehaviourSequence> dataValidation;        
            list<BehaviourSequence>::iterator itValidation;
            int iterations;
           
             
            for(iterations=0;iterations<Max_Inference_Iterations;iterations++){
              
                double bestErrorTest = 0;
                double bestErrorTrain = 0;
      
                 dataValidation.clear();
                 connectSQL();
                 vector<vector< vector<BehaviourSequence> > > kFoldsVector= getKFoldsVector(con, &dataValidation); 
//                 vector<vector< vector<BehaviourSequence> > > kFoldsVector= getKFoldsByUsers(con, dataValidation); 
                 disconnectSQL();
                 errorsValidationByAction.clear();
                 errorsValidationByAction.resize(Num_Actions,0);     
                 errorsValidationByActionSel.clear();
                 errorsValidationByActionSel.resize(Num_Actions,vector<int>(Num_Actions,0));  

                
                 
                for(int k=0;k<K_Fold;k++){    
                    dataTrainingGlobal.clear();
                    dataTestGlobal.clear();
                    models.clear();
                    errorsTrainByAction.clear();
                    errorsTrainByAction.resize(Num_Actions,0);     
                    errorsTrainByActionSel.clear();
                    errorsTrainByActionSel.resize(Num_Actions,vector<int>(Num_Actions,0));       
                    errorsTestByAction.clear();
                    errorsTestByAction.resize(Num_Actions,0);     
                    errorsTestByActionSel.clear();
                    errorsTestByActionSel.resize(Num_Actions,vector<int>(Num_Actions,0));  
//                    errorsByUser.clear();
//                    errorsByUser.resize(Num_Users,vector<int>(Num_Actions,0));

                for (int idAction = 0; idAction < Num_Actions; idAction++) {
                    
                    gettimeofday(&start, NULL);
                        
                    vector< vector <int> > allSequences;

                      list<BehaviourSequence> dataTraining=            
                        setTrainingAndTestKFold(kFoldsVector[idAction], k, dataTrainingGlobal, dataTestGlobal);                                 
                    
//                      if(idAction!=Num_Actions-1){//null action
                        //save the model for the behaviour "i"
                        InferenceClassInterface *infObj=getInferenceModel();  
                        infObj->train(clusters,dataTraining,idAction);
                        models.push_back(infObj);     
//                      }

                      gettimeofday(&stop, NULL);
                    timeLearning[idAction]+=( (stop.tv_sec - start.tv_sec)) + ( (stop.tv_usec - start.tv_usec)/1000000.0);
                    learnCount[idAction]++;                    
                    }
 
//                    connectSQL();
                    double errorTrain=0;                    
                    for( list<BehaviourSequence>::iterator itTraining=dataTrainingGlobal.begin(); itTraining!=dataTrainingGlobal.end();itTraining++){
                         
                          gettimeofday(&start, NULL);
                          
                        int selectedAction= compareModels(models, *itTraining);
//                          int selectedAction= compareModels(models, *itTraining, k+1, 1);

                          if(selectedAction==-1)
                                selectedAction=Num_Actions-1;//acion null      
                       
                        
                        errorsTrainByActionSel[itTraining->idAction][selectedAction]++;
                         
                        
                        if(selectedAction==-1  || selectedAction!=itTraining->idAction){
                            errorTrain++;     
                            errorsTrainByAction[itTraining->idAction]++;
        //                    if(selectedAction!=-1)
        //                        errorsTrainByActionSel[itTraining->idAction][selectedAction]++;

//                            errorsByUser[itTraining->idUser][itTraining->idAction]++;
                        }
                         gettimeofday(&stop, NULL);
                        detectCount[itTraining->idAction]++;                        
                        timeDetection[itTraining->idAction]+=( (stop.tv_sec - start.tv_sec)) + ( (stop.tv_usec - start.tv_usec)/1000000.0);
                       
                    }
                      
                    double errorTest=0;         
//                    includeNullActions(dataTestGlobal);
                    
                    for( list<BehaviourSequence>::iterator itTest=dataTestGlobal.begin(); itTest!=dataTestGlobal.end();itTest++){                                                          

                        gettimeofday(&start, NULL);                                                
                       int selectedAction= compareModels(models, *itTest);     
                      //   int selectedAction= compareModels(models, *itTest, k+1, 2);
                        
                       if(selectedAction==-1)
                           selectedAction=Num_Actions-1;//acion null                           
                  
                       errorsTestByActionSel[itTest->idAction][selectedAction]++; 
                       
                               
                        if(selectedAction== -1 || selectedAction!=itTest->idAction){
                            errorTest++;               
                            errorsTestByAction[itTest->idAction]++;
        //                     if(selectedAction!=-1)
        //                        errorsTestByActionSel[itTest->idAction][selectedAction]++; 

//                            errorsByUser[itTest->idUser][itTest->idAction]++;
                        }
                         gettimeofday(&stop, NULL);
                        detectCount[itTest->idAction]++;                        
                        timeDetection[itTest->idAction]+=( (stop.tv_sec - start.tv_sec)) + ( (stop.tv_usec - start.tv_usec)/1000000.0);                         
                    }
//                    disconnectSQL();

//
//                    for (int idUser = 1; idUser < Num_Users; idUser++) {               
//                        for (int idAction = 0; idAction < Num_Actions; idAction++) {
//                            globalErrorsByUser[idUser][idAction]+=errorsByUser[idUser][idAction];
//                        }
//                    }
                   
                    /******** BEST RESULTS*********/
                    errorTrain=1- errorTrain/dataTrainingGlobal.size();
                    errorTest=1-errorTest/dataTestGlobal.size();
        //            if ((errorTest + errorTrain) <= (bestErrorTest + bestErrorTrain)) {
                        if (errorTest+errorTrain > bestErrorTest+bestErrorTrain || 
                                (errorTest+errorTrain == bestErrorTest+bestErrorTrain && errorTest>bestErrorTest)) {

                            cout<<"Better result, "<<errorTrain<<"/"<<errorTest<<endl;

                            bestErrorTest = errorTest;                           
                            bestErrorTrain = errorTrain;
                            for (int idAction = 0; idAction < Num_Actions; idAction++) {
                                //train
                                globalErrorsTrainByAction[idAction] = errorsTrainByAction[idAction];                       
                                //test
                                globalErrorsTestByAction[idAction] = errorsTestByAction[idAction];
                                for (int idActionSel = 0; idActionSel < Num_Actions; idActionSel++) {
                                    globalErrorsTrainByActionSel[idAction][idActionSel] += errorsTrainByActionSel[idAction][idActionSel];
                                    globalErrorsTestByActionSel[idAction][idActionSel] += errorsTestByActionSel[idAction][idActionSel];
                                }       
                            }
                            idsTrainingSel = getIdsTraining(&dataTrainingGlobal);                        
//                            bestModels=models;
                            copyModels(bestModels, models);
                            
                            if(bestErrorTest==1 && bestErrorTrain==1)//exit loop k-fold
                                break;
                        }else{//not better result
                          for (int idAction = 0; idAction < Num_Actions; idAction++) {
                                //train
                                globalErrorsTrainByAction[idAction] = errorsTrainByAction[idAction];                       
                                //test
                                globalErrorsTestByAction[idAction] = errorsTestByAction[idAction];
                                for (int idActionSel = 0; idActionSel < Num_Actions; idActionSel++) {
                                    globalErrorsTrainByActionSel[idAction][idActionSel] += errorsTrainByActionSel[idAction][idActionSel];
                                    globalErrorsTestByActionSel[idAction][idActionSel] += errorsTestByActionSel[idAction][idActionSel];
                                }       
                            }
                        cout<<"result, "<<errorTrain<<"/"<<errorTest<<endl;
                        }
                //deleting pointers
                for (int pos = 0; pos < models.size(); pos++)
                    delete models[pos];
                
                  /******** MEAN RESULTS*********/
//                meanErrorValidation+=errorTest;     
//                meanErrorTrain+=errorTrain;
//                   /******** WORST RESULTS*********/
//                if (errorTest<worstErrorValidation)
//                    worstErrorValidation = errorTest;    
//                if (errorTrain< worstErrorTrain)
//                    worstErrorTrain = errorTrain;
          
        //            }           
                }//end loop training KFold

               
                
//             for (int idUser = 1; idUser < Num_Users; idUser++) {               
//                    for (int idAction = 0; idAction < Num_Actions; idAction++) {
//                        globalErrorsByUser[idUser][idAction]/=(double)K_Fold;
//                    }
//                }

                //Now we check the validation data with the best result
                double errorValidation=0;
                for(itValidation=dataValidation.begin(); itValidation!=dataValidation.end();itValidation++){

                      gettimeofday(&start, NULL);     
                      
                       int selectedAction= compareModels(bestModels, *(itValidation));
                       
                      if(selectedAction==-1)
                                selectedAction=Num_Actions-1;//acion null      
                       
                    if(selectedAction!=-1)
                            errorsValidationByActionSel[itValidation->idAction][selectedAction]++; 

                    if(selectedAction== -1 || selectedAction!=itValidation->idAction){
                        errorValidation++;               
                        errorsValidationByAction[itValidation->idAction]++;
    //                     if(selectedAction!=-1)
    //                        errorsValidationByActionSel[itValidation->idAction][selectedAction]++; 

//                        errorsByUser[itValidation->idUser][itValidation->idAction]++;
                    }
                          gettimeofday(&stop, NULL);
                     detectCount[itValidation->idAction]++;                        
                     timeDetection[itValidation->idAction]+=( (stop.tv_sec - start.tv_sec)) + ( (stop.tv_usec - start.tv_usec)/1000000.0);
   
      
                }
                errorValidation=1- errorValidation/dataValidation.size();
                
                double globalErrorTrain=(bestErrorTest+bestErrorTrain*(K_Fold-1))/K_Fold;
//                 int globalErrorTrain=bestErrorTrain;
             //    bestGlobalErrorTrain=globalErrorTrain;
            //     bestErrorValidation=bestErrorTest;
                   
                /******** MEAN RESULTS*********/
                meanErrorValidation+=errorValidation;     
                meanErrorTrain+=globalErrorTrain;
                /******** WORST RESULTS*********/
                if (errorValidation<worstErrorValidation)
                    worstErrorValidation = errorValidation;    
                if (globalErrorTrain< worstErrorTrain)
                    worstErrorTrain = globalErrorTrain;
                

 cout << "              result Validation, " << globalErrorTrain << "/" << errorValidation << endl;
 
                if (errorValidation > bestErrorValidation || (errorValidation == bestErrorValidation && globalErrorTrain > bestGlobalErrorTrain)) {

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
                    if(bestErrorValidation==1 && bestGlobalErrorTrain==1)//exit loop k-fold
                                break;
                }    
               
            }//end loop Inference iterations
            
             //get the mean
             meanErrorValidation/=(double)iterations;
             meanErrorTrain/=(double)iterations;

               
              string timesLearn = getTimes(timeLearning, learnCount);
              string timesDetect = getTimes(timeDetection, detectCount);
              
             //saving results
//             connectSQL();
//             saveTrainingData(con, idsTrainingSel, 
//                     bestGlobalErrorTrain, bestErrorValidation,
//                     worstErrorTrain, worstErrorValidation,
//                     meanErrorTrain, meanErrorValidation,
//                     &globalErrorsByAction, &globalErrorsByActionSel,
//                     &globalErrorsValidationByAction, &globalErrorsValidationByActionSel);
//              saveTrainingData(con, idsTrainingSel, 
//                     bestGlobalErrorTrain, bestErrorValidation,
//                     worstErrorTrain, worstErrorValidation,
//                     meanErrorTrain, meanErrorValidation,
//                     &globalErrorsTrainByAction, &globalErrorsTrainByActionSel,
//                     &globalErrorsTestByAction, &globalErrorsTestByActionSel,
//                      timesLearn, timesDetect);

//             disconnectSQL();
             cout<<endl<<
                     "The best solution was:Error of Training: "<<
                    bestGlobalErrorTrain
                        <<", and error of validation: "<<bestErrorValidation<<
                       //", total: "<<(bestErrorValidation+bestGlobalErrorTrain*(K_Fold-1))/K_Fold<<
                     ", total: "<<(bestErrorValidation*ValidationRate/100+bestGlobalErrorTrain*TrainingRate/100)<<
                        endl;

             cout<<"mean error train: "<<meanErrorTrain
                     <<", mean error validation: "<< meanErrorValidation<< 
                      ", total: "<<(meanErrorValidation*ValidationRate/100+meanErrorTrain*TrainingRate/100)<<
                     endl;

//             for(int i=0;i<Num_Actions;i++){
////                bestModels[i]->save2DB();
//                cout<<"Time Learning action "<<i<<" : "<< timeLearning[i]/(double)learnCount[i]<<endl;
//                 cout<<"Time Detection action "<<i<<" : "<< timeDetection[i]/(double)detectCount[i]<<endl<<endl;
//             }
        std::ostringstream s1,s2,s3,s4;    
        for (int i = 0; i < Num_Actions; i++) {           
            for (int j = 0; j < Num_Actions; j++) {
                s2 << (globalErrorsByActionSel)[i][j] << ",";
                s4 << (globalErrorsValidationByActionSel)[i][j] << ",";
            }
            s2 <<"      "<< globalErrorsByAction[i]<< endl;
            s4 <<"      "<< globalErrorsValidationByAction[i]<< endl;
        }
        cout<<"Training errors: "<<endl<<s2.str()<<endl;
        cout<<"Test errors: "<<endl<<s4.str()<<endl;
         
        /****************************END*************************************************/        
         
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

