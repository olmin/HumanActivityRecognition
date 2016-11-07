/* 
 * File:   BehaviourDetection.cpp
 * Author: olmin
 * 
 * Created on 11 de enero de 2013, 12:24
 */

#include "BehaviourDetection.h"


//using namespace cv;

BehaviourDetection::BehaviourDetection(int argc, char **argv) {
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
        SimpleSkeleton skel;
        clusters.resize(numClusters, skel);
        
        getClustersFromDB(con, &clusters);                  
           
         vector< vector <int> > allSequences;
         
         //This is for training the system with the data saved in the DB
        for (int idAction = 0; idAction < Num_Actions; idAction++) {           
            
            string dataset=(InferenceClassUsed == "HMM")? "HMMTrainingSet": "OlmoTrainingSet";            
          
            list<BehaviourSequence> dataTraining = getTrainedData(con,idAction, dataset);                 
            
           if (InferenceClassUsed == "HMM") {
                //Initialize the HMM with the training data we have
//                HMM* new hmm(numStates, clusters, &dataTraining);
                //save the model for the behaviour "i"
                models.push_back(new HMM(numStates, clusters, &dataTraining));
                
            } else if (InferenceClassUsed == "OlmoAutomata") {               
                
                //Create the automata with the training data
                allSequences.clear();
                for (list<BehaviourSequence>::iterator itTraining = dataTraining.begin(); itTraining != dataTraining.end(); itTraining++) {
                    allSequences.push_back(itTraining->sequence);
                }        
                
                /*Creating the automata*/
//                OlmoAutomata* new OlmoAutomata(allSequences);
//                automata->startAutomata();
                //save the model for the behaviour "i"
                models.push_back(new OlmoAutomata(allSequences));     
               
            }           
        }
         
        con -> releaseSavepoint(savept);
//        cout << "\tCommitting outstanding updates to the database .." << endl;
//        con -> commit();
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
        
    } catch (std::runtime_error &e) {

        cout << "ERROR: runtime_error in " << __FILE__;
        cout << " (" << __func__ << ") on line " << __LINE__ << endl;
        cout << "ERROR: " << e.what() << endl;
    }
}

string BehaviourDetection::addFrame(SimpleSkeleton skel) {
     int selAction;
     double best;        
                
    toResume.push_back(skel);  
    
    //2. Now we resume it with paa
    if (toResume.size() == compressionRate) {
        
        SimpleSkeleton paaSkel = getPAASkeletons(toResume);
//        paaSkel.height=0;
        paaSkel.normalizeSkel();
        toResume.clear();
         
        //3. Cluster the data
        //we assign each frame skeleton to a cluster
        assignSkeleton2Clusters(clusters, &paaSkel);
        
        selAction = -1;
        best = 0;
        
        for (int action = 0; action < Num_Actions; action++) {           
            models[action].addNewState(paaSkel.idCluster - 1);   
//             double count = models[action].getActualRating();   
//             cout<<"Count for action:"<<action<<", count:"<<count<<endl;
            if (models[action].isFinalState()) {
                double count = models[action].getActualRating();   
//                cout<<"is final state:"<<count<<endl;
                if (selAction == -1 || count > best) {
                    best = count;
                    selAction = action;
                }
            }            
        }
        if (selAction != -1) {           
           // cout << "Actionsel:" << listActions[selAction]  << endl;
            return listActions[selAction];  
        }else{
            return "none";
        }
    }
    return "";

}
