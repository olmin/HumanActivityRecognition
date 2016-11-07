/* 
 * File:   ClusterData.h
 * Author: olmin
 *
 * Created on 29 de enero de 2013, 10:59
 */

#ifndef CLUSTERDATA_H
#define	CLUSTERDATA_H

#include "config.h"
#include "HarusDataSet.h"

class Cluster {
public:       
    void setCenter(vector<float> center){
        this->center=center;
    }
    
    float calcDistance(vector<float> other) {
      float actualdist;   
      float distance=0;
        for (int i = 0; i < Vector_Size; i++) {
            actualdist = fabs(center[i] - other[i]);
            if (actualdist > distance)
                distance = actualdist;           
        }
      return distance;
    }
//    float calcDistance(vector<float> point){
//        float distance=0;
//        
//        for(int i=0;i< Vector_Size;i++)
//            distance+=fabs(point[i]-center[i]);
//        
//        return distance;
//    }
    
//private:   
    int idCluster;
    vector<float> center;
};


class ClusterData {
public:
    ClusterData(){};
    ClusterData(HarusDataSet data);
    void save();
    void read();
//private:
    vector<Cluster> clusters;

};

void assignClusters2Skeletons(vector<Cluster> clusters, HarusDataSet *data);


#endif	/* CLUSTERDATA_H */

