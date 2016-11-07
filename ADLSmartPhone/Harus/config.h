/* 
 * File:   config.h
 * Author: olmin
 *
 * Created on 29 de enero de 2013, 10:53
 */

#ifndef CONFIG_H
#define	CONFIG_H

#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <fstream>
#include <libio.h>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <math.h>

/* MySQL Connector/C++ specific headers */
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/metadata.h>
#include <cppconn/resultset_metadata.h>
#include <cppconn/exception.h>
#include <cppconn/warning.h>

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/noncopyable.hpp>

using namespace boost;
using namespace std;
using namespace sql;

#define DBHOST "tcp://127.0.0.1:3306"
#define USER "olmin"
#define PASSWORD "dover"
//#define PASSWORD "ano7"
#define DATABASE "harusdbCOPY"
//#define DATABASE "harusdb"


#define Train_Path "../UCI_HAR_Dataset/train/InertialSignals/"
#define Test_Path "../UCI_HAR_Dataset/test/InertialSignals/"
#define Train_Label_File "../UCI_HAR_Dataset/train/y_train.txt"
#define Test_Label_File "../UCI_HAR_Dataset/test/y_test.txt"
#define BodyAccX "body_acc_x.txt"
#define BodyAccY "body_acc_y.txt"
#define BodyAccZ "body_acc_z.txt"
#define BodyGyroX "body_gyro_x.txt"
#define BodyGyroY "body_gyro_y.txt"
#define BodyGyroZ "body_gyro_z.txt"

#define Sequence_Length 128
#define Vector_Size 6

// Configuration Parameters 
/*!Number of different actions*/
#define Num_Actions 6

#define Compression_Rate 2
#define Num_Clusters 14

//#define INFERENCE_CLASS "HMM"
#define INFERENCE_CLASS "OlmoAutomata"

/*! Threshold OlmoAutomata*/                 
#define ThresholdBelong 0.7

/*!Multistart max iterations*/
#define Max_Iterations 1000 

#define Max_Inference_Iterations 500

//type of the data
#define Training 1
#define Test 2

//if save and read data must be done
#define SAVE_DATA 1
#define READ_DATA 0
#define SAVE_RESULTS 1

#define DELTA 0.001

 extern Driver *driver;
 extern Connection *con;
 extern Savepoint *savept;
    
//to be used in execution time if we want to change it
extern int numClusters;
//to be used in execution time if we want to change it
extern int numStates;
//to be used in execution time if we want to change it
extern int compressionRate;

/*!List of actions that can be done*/
static const string listActions[Num_Actions] = {
     "WALKING",
      "WALKING_UPSTAIRS",
        "WALKING_DOWNSTAIRS",
        "SITTING",
        "STANDING",
        "LAYING"
};


#endif	/* CONFIG_H */

