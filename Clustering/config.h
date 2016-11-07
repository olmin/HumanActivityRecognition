/* 
 * File:   config.h
 * Author: olmin
 *
 * Created on 27 de octubre de 2011, 12:42
 */
/* Standard C++ headers */
#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <stdexcept>
#include <list>
#include <math.h>
#include <cstdlib>
#include <vector>

#include <fstream>
#include <hash_map>

#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <float.h>

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

#ifndef CONFIG_H
#define	CONFIG_H

#define DBHOST "tcp://127.0.0.1:3306"
#define USER "olmin"
#define PASSWORD "dover"
#define DATABASE "My_CAD120_db"



/*!Kinect tilt angle to be set*/
#define KINECT_TILT_ANGLE 10.0

/*! Number of different object types*/
#define Num_Objects 10
#define Max_Objects_Per_Action 5

#define Num_Joints 15
/*! Number of joints angles. */
#define Num_Angles 11
/*!Number of different actions*/
#define Num_Actions 10
/*!Number of different actions*/
//#define Num_Users 4
/*! Number of total clusters*/
#define Num_Clusters 110
/*! CompressionRate for the PAA algorithm*/
#define CompressionRate 2
/*! Actual id User for the execution*/
#define ActualIdUser 1

/*! K for the K testing algorithm*/
#define K_Test_Factor 6

/*! Number of joints angles + height. */
#define Vector_Size Num_Angles +1
#define Num_States Num_Clusters
#define Vector_Size_DTW (4+Max_Objects_Per_Action+Num_Objects+1)

/*!Multistart max iterations*/
#define Max_Iterations 250
#define Max_KMeans_Loop 200
#define Max_Inference_Iterations 1
#define K_Fold 4

#define AllData 0
#define TrainingData 1
#define TestData 2
#define ValidationData 3

#define TrainingRate 100
#define TestRate 0
#define ValidationRate 0                      

/*! Threshold OlmoAutomata*/                 
#define ThresholdBelong 0.63

//#define InferenceClassUsed "OlmoAutomata"
//#define InferenceClassUsed "HMM"
#define InferenceClassUsed "DTW"
//#define InferenceClassUsed "DTWRAW"

#define DistanceMeasureUsed "Euclidean"
//#define DistanceMeasureUsed "Max"
//#define DistanceMeasureUsed "CityBlock"
//#define DistanceMeasureUsed "AnglesPath"

#define DELTA 0.001

/*!Values min and max of height and angles*/
#define Close_Object_Dist 530
#define Med_Object_Dist 800

#define Min_Object_Dist 0 //-710 //-160
#define Max_Object_Dist 2100 //350 //840
#define Min_Height 0 //-710 //-160
#define Max_Height 640 //350 //840
#define Min_Angle 0
#define Max_Angle 180

using namespace sql;
 extern Driver *driver;
 extern Connection *con;
 extern Savepoint *savept;
 
extern int kRate;
//to be used in execution time if we want to change it
extern int numClusters;
//to be used in execution time if we want to change it
extern int numStates;
//to be used in execution time if we want to change it
extern int compressionRate;
//to be used in execution time if we want to change it
extern int actualIdUser;

using namespace std;

/*!List of actions that can be done*/
//static const string listActions[Num_Actions] = {
//    "walk",
//    "sit down",
//    "stand up from chair",
//    "bend down",
//    "bend up",
//    "twist right",
//    "twist left"
//};
static const string listActions[Num_Actions] = {    
        "reaching",
        "moving",
        "pouring",
        "eating",
        "drinking",
        "opening",
        "placing",
        "closing",
        "cleaning" ,	
	"null"
};

static const string listAngles[Num_Angles] = { 
    "ang0",// angle head-torso with the vertical	
    "ang1",// angle left elbow-shoulders	
    "ang2", //angle left hand-elbow	
    "ang3",// angle left knee-hips	
    "ang4",// angle left ankle-knee	
    "ang5",// angle right elbow-shoulders	
    "ang6",// angle right hand-elbow	
    "ang7",// angle right knee-hips	
    "ang8",// angle right ankle-knee	
    "ang9",// angle of rotation of the shoulders	
    "ang10",// angle of rotation of the hip
};

#endif	/* CONFIG_H */

