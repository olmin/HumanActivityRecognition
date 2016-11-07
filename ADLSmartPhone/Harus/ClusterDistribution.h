/* 
 * File:   ClusterDistribution.h
 * Author: olmin
 *
 * Created on 4 de noviembre de 2011, 18:31
 */

#ifndef CLUSTERDISTRIBUTION_H
#define	CLUSTERDISTRIBUTION_H

#include "ClusterData.h"
#include "config.h"
#include "SimpleDataSet.h"
#include <boost/math/distributions/normal.hpp> // for normal_distribution
  using boost::math::normal; // typedef provides default type is double.
  
  
class ClusterDistribution {
public:
    ClusterDistribution(){}
    
    ClusterDistribution(Cluster cluster){
        countObs=0;
        temporalAgrDist=0;
         this->mean=cluster;
    }
    
    ClusterDistribution(Cluster cluster, float standardDeviationIn){        
         this->mean=cluster;
         normalDist=normal(0,standardDeviationIn);
    }
    
    float getProbBelong(vector<float> skel);
    
//    void addMean(SimpleSkeleton cluster){    
//        this->mean=cluster;
//    }
    
    void addObs(vector<float> skel);
    
    /*!calcs standardDEviation param from the data agregated*/
    void calcStandarDeviation();
    
//private:
    Cluster mean;//the cluster centroide
    float standardDeviation;
    normal normalDist;
    int countObs;//count of obs added
    float temporalAgrDist;//temporal aggregate distance
};

vector<ClusterDistribution> getClustersDistribution(ClusterData clusters,  HarusDataSet *data);

#endif	/* CLUSTERDISTRIBUTION_H */

