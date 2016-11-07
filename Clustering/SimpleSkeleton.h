/* 
 * File:   SimpleSkeleton.h
 * Author: olmin
 *
 * Created on 27 de octubre de 2011, 12:41
 */

#include "config.h"
#include "vec3.h"

#ifndef SIMPLESKELETON_H
#define	SIMPLESKELETON_H

using namespace sql;

class SimpleSkeleton {
private:
        /*!Calculates the distance of the two skeletons*/
    float distanceCityBlock (SimpleSkeleton other){
        float dist=0;
        dist+=fabs(this->height - other.height);
        for(int i=0;i<Num_Angles;i++){
            dist+=fabs(this->angles[i] - other.angles[i]);
        }        
        return dist;
    }
    
    float distanceEuclidean(SimpleSkeleton other) {   
     float dist=0;
        dist+=(this->height - other.height)*(this->height - other.height);
        for(int i=0;i<Num_Angles;i++){
            dist+=(this->angles[i] - other.angles[i])*(this->angles[i] - other.angles[i]);
        }        
        dist= sqrt(dist);
    return dist;
    }
     
    float distanceMax(SimpleSkeleton other) {
      float distance, actualdist;

      distance = fabs(this->height - other.height);
//      distance=FLT_MIN;
        for (int j = 0; j < Num_Angles; j++) {
            actualdist = fabs(this->angles[j] - other.angles[j]);
            if (actualdist > distance)
                distance = actualdist;           
        }
      return distance;
     }
    
    float distanceAnglesPath(SimpleSkeleton other) {

        float distanceThis[Num_Angles];
        float distanceOther[Num_Angles];

        //      distance = fabs(this->height - other.height);
        //      distance=FLT_MIN;
        float pi= 3.1415926;
        
        for (int j = 0; j < Num_Angles; j++) {            
            float angleThis = ((this->angles[j]*(Max_Angle - Min_Angle))+ Min_Angle)* pi / 180;
            float angleOther =((other.angles[j]*(Max_Angle - Min_Angle))+ Min_Angle)* pi / 180;
            distanceThis[j] = cos(angleThis );
            distanceOther[j] = cos(angleOther);
            if (j != 0) {
                distanceThis[j - 1] += sin(angleThis);
                distanceOther[j - 1] += sin(angleOther);
            }
        }
        float dist = 0;
        for (int i = 0; i < Num_Angles; i++) {
            dist += (distanceThis[i] - distanceOther[i])*(distanceThis[i] - distanceOther[i]);
        }
        dist = sqrt(dist);
        return dist;   
     }
public:

    SimpleSkeleton() {
    }
    int idBehaviour;  
    int idCluster;
    int frame;
    float height;
    float angles[Num_Angles];
    
    bool operator<(SimpleSkeleton other){
        
        return this->idBehaviour<other.idBehaviour;
    }
    
     
    float distance(SimpleSkeleton other) {
        if(DistanceMeasureUsed=="Max")
            return distanceMax(other);
        else if (DistanceMeasureUsed=="Euclidean")
            return distanceEuclidean(other);    
         else if (DistanceMeasureUsed=="CityBlock")
            return distanceCityBlock(other);    
           else if (DistanceMeasureUsed=="AnglesPath")
            return distanceAnglesPath(other);    
    }

    
//    float distance(SimpleSkeleton other) {
//      float actualdist;
//
////      float* maxDist= new float[3];
//      //float* maxDist= new float[2];;
////      vector<float> maxDist(2, fabs(this->height - other.height));
////      maxDist.resize(3,0);
//      
////      maxDist[0] = FLT_MIN;
//      float maxDist0 =fabs(this->height - other.height);
//      float maxDist1 = FLT_MIN;
////      maxDist[2] = FLT_MIN;
//      
//        for (int j = 0; j < Num_Angles; j++) {
//            actualdist = fabs(this->angles[j] - other.angles[j]);
//            if (actualdist > maxDist0)
//                maxDist0 = actualdist; 
//            else if (actualdist > maxDist1)
//                maxDist1 = actualdist; 
////            else if (actualdist > maxDist[2])
////                maxDist[2] = actualdist; 
//        }
//      if(maxDist1== FLT_MIN)
//          maxDist1=maxDist0;
//      return (maxDist0+maxDist1)/2;//+maxDist[2];
////      delete[] maxDist;
////      maxDist=NULL;
////      return actualdist;
//    }
    
    /*!normalizes the skeleton*/
    void normalizeSkel() {
        this->height = (this->height - Min_Height) / (Max_Height - Min_Height);
        for (int i = 0; i < Num_Angles; i++)
            this->angles[i] = (this->angles[i] - Min_Angle) / (Max_Angle - Min_Angle);
    }
    
};

SimpleSkeleton fillSkeletonFromDB(int idBeh,ResultSet *resSkeletons);

SimpleSkeleton fillSkeletonFromDB(int idBeh, ResultSet *resSkeletons, double userHeight);

list<SimpleSkeleton> getAllSimpleSkeleton(Connection *con);

vec3 string2Point3D(string stringPoint);

#endif	/* SIMPLESKELETON_H */

