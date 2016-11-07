/* 
 * File:   behaviour.h
 * Author: olmin
 *
 * Created on 13 de septiembre de 2011, 13:15
 */

#ifndef BEHAVIOUR_H
#define	BEHAVIOUR_H


#include <time.h>
#include <iostream>
#include <string>
#include <ntk/gesture/skeleton.h>

/*! Number of joints angles. */
#define Num_Angles 11
 /*! Frames per second */
#define FPS 30
/*!Number of Actions*/
#define NumActions 7

#define KINECT_TILT_ANGLE 10.0

using namespace std;
using namespace cv;
using namespace ntk;

/*!List of actions that can be done*/
static const string listActions[NumActions] = {
    "walk",
    "sit down",
    "stand up from chair",
    "bend down",
    "bend up",
    "twist right",
    "twist left"
};

  /*!List of actions that can be done*/
static const int timeActions[NumActions] = {
    3,
    2,
    2,
    2,
    2,
    2,
    2
};

class SimpleSkeleton{
public:
    SimpleSkeleton(){        
    }

     cv::Point3f pos3D;
     time_t timestamp;
     float angles[Num_Angles];
     vector<Point3f> joints;
};

class Behaviour{
    
private:
   
    /*! Behaviour total duraion in seconds. */
//   enum{BehaviourMaxDuration= 2*60 };//2 minutes

   /*!Specific duration of the behaviour*/
   int specificDuration;
          
public:
    string name;
    time_t dateIni;
    time_t dateEnd;
    int count;
    string comments;
    int user;
    
    vector<SimpleSkeleton> positions;//[BehaviourMaxDuration*FPS];

  /*! Deep copy the Behaviour data. */
  void copyTo(Behaviour& rhs) const;
  
  /*! Initializes de Behaviour class*/
  void initialize(int userId) ;
  
  /*! To start the behaviour*/
  void start() ;
  
  /*! Saves the actual skeleton detected into the Behaviour data*/
  bool saveData(Skeleton* skeleton) ;
  
  /*! Ends the behaviour and saves all its data into the database,*/
  void finish(cv::Mat3b * snapshot) ;
  
  /*!Returns the FPS rate*/
  int getFPS(){return FPS;};
  
  /*!Asks through shell to user the behaviour to be recorded*/
  void getBehaviourName();
 
  
private:
   float calcAngle(cv::Point3f p1Ini, cv::Point3f p1End, 
                cv::Point3f p2Ini, cv::Point3f p2End); 
   float calcAngleRef(cv::Point3f p1Ini, cv::Point3f p1End, 
                cv::Point3f p2Ini, cv::Point3f p2End);
   void saveInDB(std::istream * snapshot);
};

string point3D2String(Point3f point);

#endif	/* BEHAVIOUR_H */

