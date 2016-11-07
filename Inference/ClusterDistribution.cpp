/* 
 * File:   ClusterDistribution.cpp
 * Author: olmin
 * 
 * Created on 4 de noviembre de 2011, 18:31
 */

#include "ClusterDistribution.h"



float ClusterDistribution::getProbBelong(SimpleSkeleton skel){    
    if(standardDeviation==0)
        return 0;   
  //  cout<<"(dist,sd): ("<<dist<<", "<<standardDeviation <<")"<<endl;
   float res= pdf(normalDist, this->mean.distance(skel));///this->standardDeviation);   
 
   return res;//+0.0001;//laplace
}
 
void ClusterDistribution::addObs(SimpleSkeleton skel){  
    float dist=this->mean.distance(skel);
    this->temporalAgrDist+=(dist*dist);
    this->countObs++;    
}

/*!calcs standardDEviation param from the data agregated*/
void ClusterDistribution::calcStandarDeviation(){
 
    if(this->countObs>1){
        this->standardDeviation=sqrt(this->temporalAgrDist / (this->countObs-1));        
         normalDist=normal(0,this->standardDeviation);
    }else if(this->countObs==1){
         this->standardDeviation=sqrt(this->temporalAgrDist);
         normalDist=normal(0,this->standardDeviation);
    }else{
        this->standardDeviation=0;
    }           
}
    
vector<ClusterDistribution> getClustersDistribution( list<BehaviourSequence> *data, vector<SimpleSkeleton> clusters){
 vector<ClusterDistribution> clusterDist;
 clusterDist.reserve(numClusters);
 
 //first create the clustDistribution vector from the clusters
 for(int i=0;i<numClusters;i++){
     ClusterDistribution newDist(clusters[i]);     
     clusterDist.push_back(newDist);
 }
 
 list<BehaviourSequence>::iterator itBeh;
 for (itBeh = data->begin(); itBeh != data->end(); itBeh++) {
 
     list<SimpleSkeleton>::iterator itSkel;
     for (itSkel = itBeh->skeletons.begin(); itSkel != itBeh->skeletons.end(); itSkel++) {         
         clusterDist[itSkel->idCluster].addObs(*itSkel);         
     }
 }
 
 //finally we compute the standar deviation
 for(int i=0;i<numClusters;i++){
     clusterDist[i].calcStandarDeviation();
 }
 
 return clusterDist;
}