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

//#include "../Inference_global/configInference.h"
#include "Skeleton.h"
#include "./DTWInference.h"

#include <string.h>
#include <sys/time.h>

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
                << ", and DistanceMeasureUsed: " << DistanceMeasureUsed 
                << ", and InferenceClassUsed: " << InferenceClassUsed 
                << ", and DATABASE: " << DATABASE
                << endl;
        
   // First we get the clusters distribution
        //we get the clusters from DB
       
//        SimpleSkeleton clusters[numClusters];
//        getClustersFromDB(con, clusters);
              
        //Creating and Initializing one HMM for each action
         vector<DTWInference> models;
         vector<DTWInference> bestModels;
//        vector<HMM> models;
//        vector<HMM> bestModels; 
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
        list<SkeletonSequence> dataTrainingGlobal;
        list<SkeletonSequence>::iterator itTraining;
        list<SkeletonSequence> dataTestGlobal;
        list<SkeletonSequence>::iterator itTest;
        
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

            list<SkeletonSequence> dataValidation;        
            list<SkeletonSequence>::iterator itValidation;
            int iterations;
            for(iterations=0;iterations<Max_Inference_Iterations;iterations++){
              
                int bestErrorTest = 9999;
                int bestErrorTrain = 9999;
               
                
                 dataValidation.clear();
                 connectSQL();
                 vector<vector< vector<SkeletonSequence> > > kFoldsVector= getKFoldsVector(con, &dataValidation); 
                 disconnectSQL();
                 errorsValidationByAction.clear();
                 errorsValidationByAction.resize(Num_Actions,0);     
                 errorsValidationByActionSel.clear();
                 errorsValidationByActionSel.resize(Num_Users,vector<int>(Num_Actions,0));  

                for(int k=0;k<K_Fold;k++){   
                     cout<<"Fold:"<<k<<endl;
                     
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
//                    errorsByUser.resize(Num_Users,vector<int>(Num_Actions,0))
                                       
                for (int idAction = 0; idAction < Num_Actions; idAction++) {
                  cout<<"Inference action:"<<idAction<<endl;
                    gettimeofday(&start, NULL);
                 
//                    vector< vector <int> > allSequences;
////                    string dataset = (InferenceClassUsed == "HMM") ? "HMMTrainingSet" : "OlmoTrainingSet";
//                    list<BehaviourSequence> dataTraining = getTrainedData(con, idAction);

                      list<SkeletonSequence> dataTraining=            
                        setTrainingAndTestKFold(kFoldsVector[idAction], k,&dataTrainingGlobal,&dataTestGlobal);        
                         
                    
                    //save the model for the behaviour "i"
                    DTWInference infObj;                     
                    infObj.train(dataTraining);
                    models.push_back(infObj);                                              

                    
                    gettimeofday(&stop, NULL);
                    timeLearning[idAction]+=( (stop.tv_sec - start.tv_sec)) + ( (stop.tv_usec - start.tv_usec)/1000000.0);
                    learnCount[idAction]++;
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

                 cout<<"Training done"<<endl;
                 
                    int errorTrain=0;
                    for(itTraining=dataTrainingGlobal.begin(); itTraining!=dataTrainingGlobal.end();itTraining++){
                         
                        gettimeofday(&start, NULL);
                    
                        int selectedAction= compareModels(models, *itTraining);
                       
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
                         
                        gettimeofday(&stop, NULL);
                        detectCount[itTraining->idAction]++;                        
                        timeDetection[itTraining->idAction]+=( (stop.tv_sec - start.tv_sec)) + ( (stop.tv_usec - start.tv_usec)/1000000.0);
                        
                    }
                    int errorTest=0;
                    for(itTest=dataTestGlobal.begin(); itTest!=dataTestGlobal.end();itTest++){

                       gettimeofday(&start, NULL);
                         
                       int selectedAction= compareModels(models, *(itTest));
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
                       
                         gettimeofday(&stop, NULL);
                        detectCount[itTest->idAction]++;                        
                        timeDetection[itTest->idAction]+=( (stop.tv_sec - start.tv_sec)) + ( (stop.tv_usec - start.tv_usec)/1000000.0);
   

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
//                            bestModels=models;
                            copyModels(&bestModels, &models);
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

                    gettimeofday(&start, NULL);                         
                    
                       int selectedAction= compareModels(bestModels, *(itValidation));
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
                     gettimeofday(&stop, NULL);
                     detectCount[itValidation->idAction]++;                        
                     timeDetection[itValidation->idAction]+=( (stop.tv_sec - start.tv_sec)) + ( (stop.tv_usec - start.tv_usec)/1000000.0);
   
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
                    if(bestErrorValidation+bestGlobalErrorTrain==0){//exit loop inf.iterations
                        iterations++;
                        break;
                    }
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

             for(int i=0;i<Num_Actions;i++){
                cout<<"Time Learning action "<<i<<" : "<< timeLearning[i]/(double)learnCount[i]<<endl;
                 cout<<"Time Detection action "<<i<<" : "<< timeDetection[i]/(double)detectCount[i]<<endl<<endl;
             }
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

