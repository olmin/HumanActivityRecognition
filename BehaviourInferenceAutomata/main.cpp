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


#include "Automata.h"
#include "OctaveAutomata.h"
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
        kRate=(argc >= 4 ? atoi(argv[3]) : K_Test_Factor);
        
        numStates =  numClusters;
        
        cout << "Executing with Num_Clusters: " << numClusters
                << ", Num_States: " << numStates 
                << ", CompressionRate: " << compressionRate 
                 << ", and K_RATE: " << kRate 
                << endl;
        
        // First we get the clusters distribution
        //we get the clusters from DB
        SimpleSkeleton clusters[numClusters];
        getClustersFromDB(con, clusters);
              
        //Creating and Initializing one HMM for each action
        vector<Automata> models;
        
               //this will hold all the training data, to test error after the HMM creation
        list<BehaviourSequence> dataTrainingGlobal;
        list<BehaviourSequence>::iterator itTraining;
        list<BehaviourSequence> dataTestGlobal;
        list<BehaviourSequence>::iterator itTest;
        
        vector<int> errorsTrainByAction(Num_Actions,0);
        vector<int> errorsTrainByActionSel(Num_Actions,0);
        vector<int> errorsTestByAction(Num_Actions,0);
        vector<int> errorsTestByActionSel(Num_Actions,0);
        
        vector<int> globalErrorsTrainByAction(Num_Actions,0);
        vector<int> globalErrorsTrainByActionSel(Num_Actions,0);
        vector<int> globalErrorsTestByAction(Num_Actions,0);
        vector<int> globalErrorsTestByActionSel(Num_Actions,0);
        
        int bestErrorTest=100;
        int bestErrorTrain=200;
        int worstErrorTest=0;
        int worstErrorTrain=0;
        int meanErrorTest=0;
        int meanErrorTrain=0;
           
        string idsTrainingSel;
        
        srand(time(NULL));
    
        for(int i=0;i<Max_Inference_Iterations;i++){    
            dataTrainingGlobal.clear();
            dataTestGlobal.clear();
            models.clear();
            errorsTrainByAction.clear();
            errorsTrainByAction.resize(Num_Actions,0);     
            errorsTrainByActionSel.clear();
            errorsTrainByActionSel.resize(Num_Actions,0);           
            errorsTestByAction.clear();
            errorsTestByAction.resize(Num_Actions,0);     
            errorsTestByActionSel.clear();
            errorsTestByActionSel.resize(Num_Actions,0);    
        
            for(int idAction=0;idAction<Num_Actions;idAction++) {  
                
                list<BehaviourSequence> dataAction= getBehavioursSequences(con,idAction,TrainingData);

                list<BehaviourSequence> dataTraining=            
                setTrainingAndTest(&dataAction,&dataTrainingGlobal,&dataTestGlobal);                   


                //Create the automata with the training data
                vector< vector <int> > allSequences(dataTraining.size());
                for (itTraining = dataTraining.begin(); itTraining != dataTraining.end(); itTraining++) {
                    allSequences.push_back(itTraining->sequence);
                }
                
                /*Creating the automata through Octave*/
                OctaveAutomata oct(allSequences, kRate);
                
                Automata automata(oct.getAlphabet(), oct.getNumStates(), oct.getTransitionMatrix(), oct.getInitStates(), oct.getFinalStates());
                
//                stringstream name;
//                name << "automata" << i;
//                automata.createDotFile(name.str());
                
                //save the model for the behaviour "i"
                models.push_back(automata);                 
            }

            
            int errorTrain=0;
            for(itTraining=dataTrainingGlobal.begin(); itTraining!=dataTrainingGlobal.end();itTraining++){

                int selectedAction=compareAutomatas(models, itTraining->sequence);

    //             cout<<selectedAction<<", "<<itTraining->idAction << endl;
                if(selectedAction==-1 || selectedAction!=itTraining->idAction){
                    errorTrain++;     
                    errorsTrainByAction[itTraining->idAction]++;
                    if(selectedAction!=-1)
                        errorsTrainByActionSel[selectedAction]++;
                    else
                        cout<<"Selected Training Action NULL, when identifying action: "<< itTraining->idAction << endl;
                }

            }
            int errorTest=0;
            for(itTest=dataTestGlobal.begin(); itTest!=dataTestGlobal.end();itTest++){

                int selectedAction=compareAutomatas(models, itTest->sequence);

    //             cout<<selectedAction<<", "<<itTest->idAction << endl;
                if(selectedAction==-1 || selectedAction!=itTest->idAction){
                    errorTest++;               
                    errorsTestByAction[itTest->idAction]++;
                     if(selectedAction!=-1)
                        errorsTestByActionSel[selectedAction]++; 
                     else
                        cout<<"Selected Test Action NULL, when identifying action: "<< itTest->idAction << endl;
                }

            }     

//            cout<< "Error of Training: "<<errorTrain<<"/"<<dataTrainingGlobal.size()
//                    <<", and error of test: "<<errorTest<<"/"<<dataTestGlobal.size() <<
//                    endl;
            
            /******** MEAN RESULTS*********/
            meanErrorTest+=errorTest;
            meanErrorTrain+=errorTrain;
            
            /******** WORST RESULTS*********/
            if ((errorTest + errorTrain)>(worstErrorTest + worstErrorTrain)) {
                worstErrorTest = errorTest;
                worstErrorTrain = errorTrain;               

            } else if ((errorTest + errorTrain) == (worstErrorTest + worstErrorTrain)) {
                if (errorTest > worstErrorTest) {
                    worstErrorTest = errorTest;
                    worstErrorTrain = errorTrain;                    
                }
            }
            
            /******** BEST RESULTS*********/
            if ((errorTest + errorTrain)<(bestErrorTest + bestErrorTrain)) {
                bestErrorTest = errorTest;
                bestErrorTrain = errorTrain;
                for (int idAction = 0; idAction < Num_Actions; idAction++) {
                    //train
                    globalErrorsTrainByAction[idAction] = errorsTrainByAction[idAction];
                    globalErrorsTrainByActionSel[idAction] = errorsTrainByActionSel[idAction];
                    //test
                    globalErrorsTestByAction[idAction] = errorsTestByAction[idAction];
                    globalErrorsTestByActionSel[idAction] = errorsTestByActionSel[idAction];
                }
                idsTrainingSel = getIdsTraining(&dataTrainingGlobal);

            } else if ((errorTest + errorTrain) == (bestErrorTest + bestErrorTrain)) {
                if (errorTest < bestErrorTest) {
                    bestErrorTest = errorTest;
                    bestErrorTrain = errorTrain;
                    for (int idAction = 0; idAction < Num_Actions; idAction++) {
                        //train
                        globalErrorsTrainByAction[idAction] = errorsTrainByAction[idAction];
                        globalErrorsTrainByActionSel[idAction] = errorsTrainByActionSel[idAction];
                        //test
                        globalErrorsTestByAction[idAction] = errorsTestByAction[idAction];
                        globalErrorsTestByActionSel[idAction] = errorsTestByActionSel[idAction];
                    }
                    idsTrainingSel = getIdsTraining(&dataTrainingGlobal);
                }
            }           
        }//end loop training
        
        //get the mean
         meanErrorTest/=Max_Inference_Iterations;
         meanErrorTrain/=Max_Inference_Iterations;
         
         //saving results
         saveKTestTrainingData(con, idsTrainingSel, 
                 bestErrorTrain, bestErrorTest,
                 worstErrorTrain, worstErrorTest,
                 meanErrorTrain, meanErrorTest,
                 &globalErrorsTrainByAction, &globalErrorsTrainByActionSel,
                 &globalErrorsTestByAction, &globalErrorsTestByActionSel);
                 
         cout<<endl<<
                 "The best solution was:Error of Training: "<<
                 bestErrorTrain<<"/"<<dataTrainingGlobal.size()
                    <<", and error of test: "<<bestErrorTest<<"/"<<dataTestGlobal.size()<<
                    endl;
         
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