/* 
 * File:   main.cpp
 * Author: olmin
 *
 * Created on 1  de octubre de 20 1 2, 1 2:44
 */

#include "../Clustering/config.h"
#include "../Clustering/Commons.h"
#include "../Clustering/BehaviourSequence.h"
#include "../Clustering/ClusterData.h"


#include "KFoldCrossValidation.h"
#include "OlmoAutomata.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace sql;


/*
 * 
 */
int main(int argc, char** argv) {   
   
    Driver *driver;
    Connection *con;
    Savepoint *savept;     
    
    
     
    try {
        driver = get_driver_instance();

        /* create a database connection using the Driver */
        con = driver -> connect(DBHOST, USER, PASSWORD);

        /* alternate syntax using auto_ptr to create the db connection */
        //auto_ptr  con (driver -> connect(url, user, password));

        /* turn off the autocommit */
        con -> setAutoCommit(0);

        /* select appropriate database schema */
        con -> setSchema(DATABASE);
        savept = con -> setSavepoint("SAVEPT1 ");

        
        /***************************START**************************************************/

        //Before anything set the global vars correctly
       /* initiate variables */
        numClusters = (argc >= 2 ? atoi(argv[1 ]) : Num_Clusters);
        compressionRate = (argc >= 3 ? atoi(argv[2]) : CompressionRate);
//        kRate=(argc >= 4 ? atoi(argv[3]) : K_Test_Factor);
        
        numStates =  numClusters;
        
        cout << "Executing with Num_Clusters: " << numClusters
                << ", Num_States: " << numStates 
                << ", CompressionRate: " << compressionRate 
//                 << ", and K_RATE: " << kRate 
                << endl;
        
        // First we get the clusters distribution
        //we get the clusters from DB
//        SimpleSkeleton clusters[numClusters];
//        getClustersFromDB(con, clusters);
              
        //Creating and Initializing one HMM for each action
        vector<OlmoAutomata> models;
        vector<OlmoAutomata> bestModels;
        
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
                
        
//         for(int userId=1;userId<Num_Users;userId++){             
 //            actualIdUser=userId;
//              cout<< "Executing UserId: " << actualIdUser <<endl<<endl;
          
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
                 vector<vector< vector<BehaviourSequence> > > kFoldsVector= getKFoldsVector(con, &dataValidation); 

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
                    

                    for(int idAction=0;idAction<Num_Actions;idAction++) {  

               //         list<BehaviourSequence> dataAction= getBehavioursSequencesWithoutSkel(con,idAction,TrainingData);

                        list<BehaviourSequence> dataTraining=            
                        setTrainingAndTestKFold(kFoldsVector[idAction], k,&dataTrainingGlobal,&dataTestGlobal);    
                        

                        //Create the automata with the training data
                        allSequences.clear();                               
                        for (itTraining = dataTraining.begin(); itTraining != dataTraining.end(); itTraining++) {                                  
                            allSequences.push_back(itTraining->sequence);                 
                        }              

                               
                        /*Creating the automata through Octave*/
                        OlmoAutomata automata(allSequences);
                   
        //                stringstream name;
        //                name << "automata" << i;
        //                automata.createDotFile(name.str());

                        //save the model for the behaviour "i"
                        models.push_back(automata);           
                        
                    }


                    int errorTrain=0;
                    for(itTraining=dataTrainingGlobal.begin(); itTraining!=dataTrainingGlobal.end();itTraining++){

                        int selectedAction=compareAutomatas(models, itTraining->sequence);

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

                        int selectedAction=compareAutomatas(models, itTest->sequence);

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

                    int selectedAction=compareAutomatas(bestModels, itValidation->sequence);

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
             saveOlmoTrainingData(con, idsTrainingSel, 
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
        
//               //this will hold all the training data, to test error after the HMM creation
//        list<BehaviourSequence> dataTrainingGlobal;
//        list<BehaviourSequence>::iterator itTraining;
//        list<BehaviourSequence> dataTestGlobal;
//        list<BehaviourSequence>::iterator itTest;
//        
//        vector<int> errorsTrainByAction(Num_Actions,0);
//        vector<vector<int> > errorsTrainByActionSel(Num_Actions, vector<int>(Num_Actions, 0));
//        vector<int> errorsTestByAction(Num_Actions,0);
//        vector<vector<int> > errorsTestByActionSel(Num_Actions, vector<int>(Num_Actions, 0));
//        
//        vector<vector<int> > errorsByUser(Num_Users, vector<int>(Num_Actions,0));
//        vector<vector<double> > globalErrorsByUser(Num_Users,vector<double>(Num_Actions, 0));
//         
//        vector<int> globalErrorsTrainByAction(Num_Actions,0);
//        vector<vector<int> > globalErrorsTrainByActionSel(Num_Actions, vector<int>(Num_Actions, 0));
//        vector<int> globalErrorsTestByAction(Num_Actions,0);
//        vector<vector<int> > globalErrorsTestByActionSel(Num_Actions, vector<int>(Num_Actions, 0));
////        
////        vector<double> avgPoints(Num_Actions,0);
////        vector<double> avgErr(Num_Actions,0);
//        
//        vector< vector <int> > allSequences;
//        
//        int bestErrorTest=9999;
//        int bestErrorTrain=9999;
//        int worstErrorTest=0;
//        int worstErrorTrain=0;
//        double meanErrorTest=0;
//        double meanErrorTrain=0;
//           
//        string idsTrainingSel;
//        
//        srand(time(NULL));
//    
//        for(int i=0;i<Max_Inference_Iterations;i++){    
//            dataTrainingGlobal.clear();
//            dataTestGlobal.clear();
//            models.clear();
//            errorsTrainByAction.clear();
//            errorsTrainByAction.resize(Num_Actions,0);     
//            errorsTrainByActionSel.clear();
//            errorsTrainByActionSel.resize(Num_Users,vector<int>(Num_Actions,0));       
//            errorsTestByAction.clear();
//            errorsTestByAction.resize(Num_Actions,0);     
//            errorsTestByActionSel.clear();
//            errorsTestByActionSel.resize(Num_Users,vector<int>(Num_Actions,0));  
//            errorsByUser.clear();
//            errorsByUser.resize(Num_Users,vector<int>(Num_Actions,0));
//        
//            for(int idAction=0;idAction<Num_Actions;idAction++) {  
//                
//                list<BehaviourSequence> dataAction= getBehavioursSequencesWithoutSkel(con,idAction,TrainingData);
//
//                list<BehaviourSequence> dataTraining=            
//                setTrainingAndTest(&dataAction,&dataTrainingGlobal,&dataTestGlobal);                   
//
//
//                //Create the automata with the training data
//                allSequences.clear();                               
//                for (itTraining = dataTraining.begin(); itTraining != dataTraining.end(); itTraining++) {                 
//                    allSequences.push_back(itTraining->sequence);                 
//                }              
//                
//                /*Creating the automata through Octave*/
//                OlmoAutomata automata(allSequences);
//              
////                stringstream name;
////                name << "automata" << i;
////                automata.createDotFile(name.str());
//                
//                //save the model for the behaviour "i"
//                models.push_back(automata);                 
//            }
//
//            
//            int errorTrain=0;
//            for(itTraining=dataTrainingGlobal.begin(); itTraining!=dataTrainingGlobal.end();itTraining++){
//
//                int selectedAction=compareAutomatas(models, itTraining->sequence);
//
//                  if(selectedAction!=-1)
//                        errorsTrainByActionSel[itTraining->idAction][selectedAction]++;
//                
//                if(selectedAction==-1  || selectedAction!=itTraining->idAction){
//                    errorTrain++;     
//                    errorsTrainByAction[itTraining->idAction]++;
////                    if(selectedAction!=-1)
////                        errorsTrainByActionSel[itTraining->idAction][selectedAction]++;
//                    
//                    errorsByUser[itTraining->idUser][itTraining->idAction]++;
//                }
//
//            }
//            int errorTest=0;
//            for(itTest=dataTestGlobal.begin(); itTest!=dataTestGlobal.end();itTest++){
//                
//                int selectedAction=compareAutomatas(models, itTest->sequence);
//                
//                if(selectedAction!=-1)
//                        errorsTestByActionSel[itTest->idAction][selectedAction]++; 
//
//                if(selectedAction== -1 || selectedAction!=itTest->idAction){
//                    errorTest++;               
//                    errorsTestByAction[itTest->idAction]++;
////                     if(selectedAction!=-1)
////                        errorsTestByActionSel[itTest->idAction][selectedAction]++; 
//                    
//                    errorsByUser[itTest->idUser][itTest->idAction]++;
//                }
//
//            }
//            
//
//            /******** MEAN RESULTS*********/
//            meanErrorTest+=errorTest;
//            meanErrorTrain+=errorTrain;
//
//            for (int idUser = 1; idUser < Num_Users; idUser++) {               
//                for (int idAction = 0; idAction < Num_Actions; idAction++) {
//                    globalErrorsByUser[idUser][idAction]+=errorsByUser[idUser][idAction];
//                }
//            }
//            
//            /******** WORST RESULTS*********/
//            if ((errorTest + errorTrain)>(worstErrorTest + worstErrorTrain)) {
//                worstErrorTest = errorTest;
//                worstErrorTrain = errorTrain;               
//
//            } else if ((errorTest + errorTrain) == (worstErrorTest + worstErrorTrain)) {
//                if (errorTest > worstErrorTest) {
//                    worstErrorTest = errorTest;
//                    worstErrorTrain = errorTrain;                    
//                }
//            }
//            
//            /******** BEST RESULTS*********/
////            if ((errorTest + errorTrain) <= (bestErrorTest + bestErrorTrain)) {
//                if (errorTest < bestErrorTest || (errorTest == bestErrorTest && errorTrain<bestErrorTrain)) {
//                    
//                    cout<<"Better result, "<<errorTrain<<"/"<<errorTest<<endl;
//                    
//                    bestErrorTest = errorTest;
//                    bestErrorTrain = errorTrain;
//                    for (int idAction = 0; idAction < Num_Actions; idAction++) {
//                        //train
//                        globalErrorsTrainByAction[idAction] = errorsTrainByAction[idAction];                       
//                        //test
//                        globalErrorsTestByAction[idAction] = errorsTestByAction[idAction];
//                        for (int idActionSel = 0; idActionSel < Num_Actions; idActionSel++) {
//                            globalErrorsTrainByActionSel[idAction][idActionSel] = errorsTrainByActionSel[idAction][idActionSel];
//                            globalErrorsTestByActionSel[idAction][idActionSel] = errorsTestByActionSel[idAction][idActionSel];
//                        }
//                    }
//                    idsTrainingSel = getIdsTraining(&dataTrainingGlobal);
//                }
////            }           
//        }//end loop training
//
//        //get the mean
//         meanErrorTest/=(double)Max_Inference_Iterations;
//         meanErrorTrain/=(double)Max_Inference_Iterations;
//         
//         for (int idUser = 1; idUser < Num_Users; idUser++) {               
//                for (int idAction = 0; idAction < Num_Actions; idAction++) {
//                    globalErrorsByUser[idUser][idAction]/=(double)Max_Inference_Iterations;
//                }
//            }
//         
//         //saving results
//         saveOlmoTrainingData(con, idsTrainingSel, 
//                 bestErrorTrain, bestErrorTest,
//                 worstErrorTrain, worstErrorTest,
//                 meanErrorTrain, meanErrorTest,
//                 &globalErrorsTrainByAction, &globalErrorsTrainByActionSel,
//                 &globalErrorsTestByAction, &globalErrorsTestByActionSel);
                 
//         cout<<endl<<
//                 "The best solution was:Error of Training: "<<
//                bestErrorTrain<<"/"<<dataTrainingGlobal.size()
//                    <<", and error of test: "<<bestErrorTest<<"/"<<dataTestGlobal.size()<<
//                    endl;
//         
//         cout<<"mean error train: "<<meanErrorTrain<<", mean error test: "<< meanErrorTest<< endl;
         
         
//         cout<<endl<<"Errors by user: ";
//         for (int idUser = 1; idUser < Num_Users; idUser++) {
//             cout<<endl<<idUser<<": ";
//             double total=0;
//             for (int idAction = 0; idAction < Num_Actions; idAction++) {
//                 cout<<globalErrorsByUser[idUser][idAction]<<" , ";
//                 total+=globalErrorsByUser[idUser][idAction];
//             }
//             cout<<total;
//         }
//         cout<<endl<<endl;
         
//         cout<<"Walk     sit down       stand up from chair     bend down       bend up         twist right     twist left"<<endl<<endl;
//         double finalAvg=0;
//         for(int action=0;action<Num_Actions;action++){
//             cout<<avgPoints[action]<<"       ";
//             finalAvg+=avgPoints[action];
//         }
//         finalAvg/=Num_Actions;
//         cout<<"finalAvg: "<<finalAvg<<endl<<endl;
//         
//         cout<<endl<<endl;
//         for(int action=0;action<Num_Actions;action++){
//             cout<<globalErrorsTrainByAction[action]<<"       ";
//         }
//         cout<<endl<<endl;
//         
//          cout<<"Walk     sit down       stand up from chair     bend down       bend up         twist right     twist left"<<endl<<endl;
//         for(int action=0;action<Num_Actions;action++){
//             cout<<globalErrorsTrainByActionSel[action]<<"       ";
//         }
//         cout<<endl;
         
        /****************************END*************************************************/
         
        
        
        con -> releaseSavepoint(savept);
        cout << "\tCommitting outstanding updates to the database .." << endl;
        con -> commit();

        /* Clean up */
        con -> close();
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