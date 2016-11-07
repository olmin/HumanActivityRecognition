/* 
 * File:   main.cpp
 * Author: olmin
 *
 * Created on 27 de octubre de 2011, 12:38
 */

/*local includes*/
#include "config.h"
#include "ClusterData.h"
#include "Commons.h"
#include "KMeans.h"
#include "PAA.h"
#include "SimpleSkeleton.h"

using namespace std;
using namespace sql;
/*
 * 
 */
int main(int argc, char** argv) {

    Driver *driver;
    Connection *con;
    Savepoint *savept;     
    
    /* initiate variables */
    numClusters=(argc >= 2 ? atoi(argv[1]) : Num_Clusters);
    compressionRate=(argc >= 3 ? atoi(argv[2]) : CompressionRate);
    
    cout<<"Executing with Num_Clusters: "<<numClusters
            <<", and CompressionRate: " <<compressionRate
            <<", and Distance used:"<<DistanceMeasureUsed<<endl;
 
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
        
        startPAA(con);
        
        con -> releaseSavepoint(savept);
        cout << "\tCommitting outstanding updates to the database .." << endl;
        con -> commit();
         savept = con -> setSavepoint("SAVEPT1");
         
        srand(time(0));
        
        startKmeans(con);

         con -> releaseSavepoint(savept);
        cout << "\tCommitting outstanding updates to the database .." << endl;
        con -> commit();
         savept = con -> setSavepoint("SAVEPT1");
         
        startClusteringData(con);        
      
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

