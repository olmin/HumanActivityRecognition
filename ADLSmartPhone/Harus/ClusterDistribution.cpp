/* 
 * File:   ClusterDistribution.cpp
 * Author: olmin
 * 
 * Created on 4 de noviembre de 2011, 18:31
 */

#include "ClusterDistribution.h"



float ClusterDistribution::getProbBelong(vector<float> skel){    
    if(standardDeviation==0)
        return 0;   
  //  cout<<"(dist,sd): ("<<dist<<", "<<standardDeviation <<")"<<endl;
   float res= pdf(normalDist, this->mean.calcDistance(skel));///this->standardDeviation);   
 
   return res;//+0.0001;//laplace
}
 
void ClusterDistribution::addObs(vector<float> skel){  
    float dist=this->mean.calcDistance(skel);
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
    
vector<ClusterDistribution> getClustersDistribution(ClusterData clusters,  HarusDataSet *data){
 vector<ClusterDistribution> clusterDist;
 clusterDist.reserve(numClusters);

 //first create the clustDistribution vector from the clusters
 for(int i=0;i<numClusters;i++){
     ClusterDistribution newDist(clusters.clusters[i]);     
     clusterDist.push_back(newDist);
 }
 for (int i=0;i< data->dataSet.size();i++) { 
     for (int j=0;j< data->dataSet[i].sequence.size();j++) {       
         clusterDist[data->dataSet[i].idsCluster[j]].addObs(data->dataSet[i].sequence[j]);         
     }
 }
 
 //finally we compute the standar deviation
 for(int i=0;i<numClusters;i++){
     clusterDist[i].calcStandarDeviation();
 }
 
 return clusterDist;
}