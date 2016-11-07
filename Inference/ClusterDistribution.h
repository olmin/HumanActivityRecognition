/* 
 * File:   ClusterDistribution.h
 * Author: olmin
 *
 * Created on 4 de noviembre de 2011, 18:31
 */

#ifndef CLUSTERDISTRIBUTION_H
#define	CLUSTERDISTRIBUTION_H

#include "../Clustering/config.h"
#include "../Clustering/SimpleSkeleton.h"
#include "../Clustering/BehaviourSequence.h"
#include <boost/math/distributions/normal.hpp> // for normal_distribution
  using boost::math::normal; // typedef provides default type is double.
  
  
class ClusterDistribution {
public:
    ClusterDistribution(SimpleSkeleton cluster){
        countObs=0;
        temporalAgrDist=0;
         this->mean=cluster;
    }
    float getProbBelong(SimpleSkeleton skel);
    
//    void addMean(SimpleSkeleton cluster){    
//        this->mean=cluster;
//    }
    
    void addObs(SimpleSkeleton skel);
    
    /*!calcs standardDEviation param from the data agregated*/
    void calcStandarDeviation();
    
private:
    SimpleSkeleton mean;//the cluster centroide
    float standardDeviation;
    normal normalDist;
    int countObs;//count of obs added
    float temporalAgrDist;//temporal aggregate distance
};

vector<ClusterDistribution> getClustersDistribution( list<BehaviourSequence> *data, vector<SimpleSkeleton> clusters);

#endif	/* CLUSTERDISTRIBUTION_H */

