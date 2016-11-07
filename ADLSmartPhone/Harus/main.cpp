/* 
 * File:   main.cpp
 * Author: olmin
 *
 * Created on 29 de enero de 2013, 10:44
 */


#include "HarusDataSet.h"
#include "ClusterData.h"
#include "SimpleDataSet.h"
#include "config.h"
#include "HMMDataSet.h"
#include "Common.h"
#include "InferenceClassInterface.h"
#include "OlmoAutomata.h"

using namespace std;
using namespace sql;

/*
 * 
 */
int main(int argc, char** argv) {

    //Before anything set the global vars correctly
    /* initiate variables */
    numClusters = (argc >= 2 ? atoi(argv[1]) : Num_Clusters);
    compressionRate = (argc >= 3 ? atoi(argv[2]) : Compression_Rate);
    numStates = numClusters;

    cout << "Executing with Num_Clusters: " << numClusters
            << ", Num_States: " << numStates
            << ", and CompressionRate: " << compressionRate
            << endl;

   try {
        driver = get_driver_instance();        

        //1. We read the data from the files
        HarusDataSet mainDataSet;

        if (SAVE_DATA) {
            mainDataSet= HarusDataSet(Train_Path, Train_Label_File);

            //2. Compress the data with paa algorithm
            mainDataSet.compressPAA();
            mainDataSet.save(Training);
        }
        if (READ_DATA) {
            mainDataSet.read(Training);
        }

        //3. Cluster the data and find clusters
        ClusterData clusters;
        if (SAVE_DATA) {
            clusters= ClusterData(mainDataSet);
            clusters.save();
        }
        if (READ_DATA) {
            clusters.read();
        }

        //4. Quantization of the initial data
        SimpleDataSet trainDataSet;
        if (SAVE_DATA) {
            trainDataSet= SimpleDataSet(mainDataSet, clusters);
            trainDataSet.save(Training);
        }
        if (READ_DATA) {
            trainDataSet.read(Training);
        }
             
        //5. Inference and Classification
        vector<InferenceClassInterface*> models;      
          
        //Creating Inference First Time
        if (SAVE_DATA) {
            createInferenceData(&models, clusters, mainDataSet, trainDataSet);
        }

        if (READ_DATA) {
            //reading the inference models, Second or further time
            for (int i = 1; i <= Num_Actions; i++) {
                InferenceClassInterface *inferenceObj = getInferenceObject();
                inferenceObj->read(i);
                models.push_back(inferenceObj);
            }
        }
        
        //6. Now check for errors in classification, training and test dataset
        vector<vector<int> > errorsTrain = classify(models,trainDataSet);
        
        for(int i=0;i<2;i++){
            cout<<"Errors Train"<<i<<" :"<<endl;
            for(int j=0;j<Num_Actions;j++)
                 cout<<errorsTrain[i][j]<<",";
            
            cout<<endl<<endl;
        }
        
        //8. Finally, We save the results
        if (SAVE_RESULTS) {
            saveResults(errorsTrain, Training);
        }
        //7. Now we do all with the test data set
        HarusDataSet testDS;
        if (SAVE_DATA) {
            testDS= HarusDataSet(Test_Path, Test_Label_File);
            testDS.compressPAA();
            testDS.save(Test);
        }
        if (READ_DATA) {
            testDS.read(Test);
        }

        SimpleDataSet testDataSet;
        if (SAVE_DATA) {
            testDataSet=SimpleDataSet(testDS, clusters);
            testDataSet.save(Test);
        }
        if (READ_DATA) {
            testDataSet.read(Test);
        }

        vector<vector<int> > errorsTest = classify(models, testDataSet);
        for (int i = 0; i < 2; i++) {
            cout << "Errors Test" << i << " :" << endl;
            for (int j = 0; j < Num_Actions; j++)
                cout << errorsTest[i][j] << ",";

            cout << endl << endl;
        }

        //8. Finally, We save the results
        if (SAVE_RESULTS) {
            saveResults(errorsTest, Test);
        }
        
     
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

