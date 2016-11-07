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
#include "../Inference/ClusterDistribution.h"
#include "../Inference/HMM.h"

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
        SimpleSkeleton clusters[numClusters];
        getClustersFromDB(con, clusters);
              
        //Creating and Initializing one HMM for each action
        vector<HMM> models;
              
        //this will hold all the training data, to test error after the HMM creation
        list<BehaviourSequence> dataValidationGlobal;
        list<BehaviourSequence>::iterator itVal;
              
        vector<int> errorsByAction(Num_Actions,0);
        vector<int> errorsByActionSel(Num_Actions,0);     

        for (int idAction = 0; idAction < Num_Actions; idAction++) {
            list<BehaviourSequence> dataValidation = getBehavioursSequences(con,idAction,ValidationData);
           
            for(list<BehaviourSequence>::iterator it=dataValidation.begin();
                    it!=dataValidation.end();it++){
                dataValidationGlobal.push_back(*it);
            }
            
            list<BehaviourSequence> dataTraining = getTrainedData(con,idAction);

            //Initialize the HMM with the training data we have
            HMM hmm(numStates, clusters, &dataTraining);

            //save the model for the behaviour "i"
            models.push_back(hmm);
        }

        int errors = 0;
        for (itVal = dataValidationGlobal.begin(); itVal != dataValidationGlobal.end(); itVal++) {
            int selectedAction = compareHMM(models, &(itVal->sequence));
            //             cout<<selectedAction<<", "<<itTraining->idAction << endl;
            if (selectedAction != itVal->idAction) {
                errors++;
                errorsByAction[itVal->idAction]++;
                errorsByActionSel[selectedAction]++;
            }
        }    
        
         //saving results
         saveValidationData(con,errors, &errorsByAction, &errorsByActionSel);
                 
         cout<<endl<<
                 "The end, total errors: "<<errors<<", out of "<< dataValidationGlobal.size()<<
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

