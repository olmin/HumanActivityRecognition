/* 
 * File:   main.cpp
 * Author: olmin 
 *
 * Created on 27 de octubre de 2011, 12:38
 */

/*local includes*/
#include <libio.h>
#include <sys/time.h>

#include "../Clustering/config.h"
#include "../Clustering/Commons.h"
#include "../Clustering/BehaviourSequence.h"
#include "../Clustering/ClusterData.h"
#include "../Clustering/SimpleSkeleton.h"
#include "../Clustering/PAA.h"

#include "../Inference_global/configInference.h"

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
        savept = con -> setSavepoint("SAVEPT1");

        
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
//        SimpleSkeleton clusters[numClusters];
//        getClustersFromDB(con, clusters); 
         SimpleSkeleton newSkel;
        vector<SimpleSkeleton> clusters(numClusters,newSkel);       
        getClustersFromDB(con, &clusters);
              
        //Creating and Initializing one HMM for each action
        vector<InferenceClassInterface*> models;
        
        double timeLearning, timeLearningIni;
        double timeDetection, timeDetectionIni;
        timeval stop, start;       
              
        //this will hold all the training data, to test error after the HMM creation
//        list<BehaviourSequence> dataValidationGlobal;
//        list<BehaviourSequence>::iterator itVal;
              
//        vector<int> errorsByAction(Num_Actions,0);
//        vector<int> errorsByActionSel(Num_Actions,0);     

         vector< vector <int> > allSequences;

          gettimeofday(&start, NULL);
          
          cout<<"Training Start"<<endl;
         //This is for training the system with the data saved in the DB         
        for (int idAction = 0; idAction < Num_Actions; idAction++) {  
            
//            string dataset=(InferenceClassUsed == "HMM")? "HMMTrainingSet": "OlmoTrainingSet";
            list<BehaviourSequence> dataTraining = getTrainedData(con,idAction);
                  
            //save the model for the behaviour "i"
            InferenceClassInterface *infObj = getInferenceModel();
            infObj->train(clusters, dataTraining);
            models.push_back(infObj);
        }
          cout<<"Training Finished"<<endl;
          gettimeofday(&stop, NULL);
         timeLearning=(double)( (stop.tv_sec - start.tv_sec)*1000) + (double)( (stop.tv_usec - start.tv_usec)/1000);
//         timeLearning+= (double)0.1* (double)( (stop.tv_usec - start.tv_usec)/1000);

        //1. Now I want all data
        list<SimpleSkeleton> allSkel= getAllSimpleSkeleton(con);
        vector<SimpleSkeleton> toResume;        
        vector<int> idsSeqs;
        vector<int> errorsTrainByAction(Num_Actions,0);
        vector<int> errorsTrainByActionSel(Num_Actions,0);
        int errors = 0;  
        int hits=0;
        int selAction;
        double best;        
        bool firstTime=true;
        int count=allSkel.size();
        
         gettimeofday(&start, NULL);
        
        for (list<SimpleSkeleton>::iterator itSkel = allSkel.begin(); itSkel != allSkel.end(); itSkel++){
            toResume.push_back(*itSkel);
            count--;
             //2. Now we resume it with paa
            if(toResume.size()==compressionRate || count==0){               
                
                SimpleSkeleton paaSkel= getPAASkeletons(toResume);
//                paaSkel.height=0.5;
//                paaSkel.normalizeSkel();
                toResume.clear();                
                
                //3. Cluster the data
                //we assign each frame skeleton to a cluster
                assignSkeleton2Clusters(clusters, &paaSkel);
                idsSeqs.push_back(itSkel->idBehaviour);
          
                selAction=-1;
                best = 0;
                int realAction = getTypeAction(con, itSkel->idBehaviour);

                for (int action = 0; action < Num_Actions; action++) {
                    if (firstTime)
                        models[action]->startAutomata();

//                    models[action]->addNewState(paaSkel.idCluster -1);
                     models[action]->addNewState(paaSkel);
                    if (models[action]->isFinalState()) {
//                        double count = models[action].getActualRating();
                        if (selAction == -1 || count > best) {
                            best = count;
                            selAction = action;
                        }
                    }
                }
                firstTime=false;
                if (selAction != -1) {
//                    if (selAction != realAction) {
//                        cout << "Actionsel and real:" << selAction << "," << realAction << ", count, " << best << ", real count: " << models[realAction].getActualRating() << endl;
//                    }
                    if (selAction != realAction) {
                        errors++;
                        errorsTrainByAction[realAction]++;
                        errorsTrainByActionSel[selAction]++;
                    } else
                        hits++;
                }
            }
        }              
        gettimeofday(&stop, NULL);
        timeDetection=(double)( (stop.tv_sec - start.tv_sec)*1000) + (double)( (stop.tv_usec - start.tv_usec)/1000);
//        timeDetection+= (double)0.1* (double)( (stop.tv_usec - start.tv_usec)/1000);
        
        cout<<endl<<"total frames:"<< idsSeqs.size()<<endl<<endl;
        
        double total=(double)hits+errors;
        
         //saving results
         saveValidationData(con,total/idsSeqs.size(),(double)errors/total, &errorsTrainByAction, &errorsTrainByActionSel, timeLearning, timeDetection);
                 
         cout<<endl<<
                 "The end, total errors: "<<errors<<" out of "<< total<<endl<<endl;
         
         cout<<"Walk     sit down       stand up from chair     bend down       bend up         twist right     twist left"<<endl<<endl;
         for(int action=0;action<Num_Actions;action++){
             cout<<errorsTrainByAction[action]<<"       ";
         }
         cout<<endl<<endl;
         
          cout<<"Walk     sit down       stand up from chair     bend down       bend up         twist right     twist left"<<endl<<endl;
         for(int action=0;action<Num_Actions;action++){
             cout<<errorsTrainByActionSel[action]<<"       ";
         }
         cout<<endl;
         
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

