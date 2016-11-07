/* 
 * File:   ClusterData.cpp
 * Author: olmin
 * 
 * Created on 29 de enero de 2013, 10:59
 */

#include "ClusterData.h"
#include "Common.h"

void assignClusters2Skeletons(vector<Cluster> clusters, HarusDataSet *data) {

    float minDistance;
    int idCluster;

    for (int i=0; i< data->dataSet.size();i++) {         
        for (int j = 0; j < data->dataSet[i].sequence.size(); j++) {
             minDistance = 999999999999999999; //init     
            for (int k = 0; k < numClusters; k++) {
                float distance = clusters[k].calcDistance(data->dataSet[i].sequence[j]);
                if (distance < minDistance) {
                    minDistance = distance;
                    idCluster = clusters[k].idCluster;
                }
            }
            data->dataSet[i].idsCluster.push_back(idCluster);
        }
    }
}

bool equalClusters(vector<Cluster> clust1, vector<Cluster> clust2) {

    for (int i = 0; i < numClusters; i++) {
        if (clust1[i].calcDistance(clust2[i].center) != 0)
            return false;
    }
    return true;
}

void calcKmeans(vector<Cluster> clusters, HarusDataSet data){  
    
    vector<Cluster> copyClusters=clusters;
    
    int count[numClusters];   
    int idCluster;
    int totalCount=0;     
    while(true){//we loop until it converges        
        //Now we start Kmeans by placing each HarusData on one cluster       
        assignClusters2Skeletons(clusters, &data);
        
        //initializing the count
        for (int i = 0; i < numClusters; i++) {
            count[i] = 0;
        }

        //now we calculate the new clusters doing the centroide
       for (int i=0; i< data.dataSet.size();i++) {
            for (int j = 0; j < data.dataSet[i].sequence.size(); j++) {
                idCluster = data.dataSet[i].idsCluster[j];
                if (count[idCluster] == 0) {
                    //initialize              
                    for (int k = 0; k < Vector_Size; k++)
                        clusters[idCluster].center[k] = data.dataSet[i].sequence[j][k];

                } else {//not the first               
                    for (int k = 0; k < Vector_Size; k++)
                        clusters[idCluster].center[k] += data.dataSet[i].sequence[j][k];
                }
                count[idCluster]++; //inserted in that cluster
            }
        }

        //now the mean for each cluster
        for (int i = 0; i < numClusters; i++) {
            if (count[i] != 0) {               
                for (int j = 0; j < Vector_Size; j++)
                    clusters[idCluster].center[j] /= count[i];
            }
        }
        if (equalClusters(copyClusters, clusters) || totalCount==150){           
            break;//then we got the convergence and we stop;
        }
         copyClusters=clusters;
         totalCount++;
    }       
    
}

void calcInitialClusters( vector<Cluster> *clusters, HarusDataSet data){  
  vector<HarusDataSet> spliData= splitActions(data);
   int action=0; 
  for(int i=0;i<clusters->size();i++){             
        int index1 = rand() % spliData[action%Num_Actions].dataSet.size();
        int index2 = rand() % spliData[action%Num_Actions].dataSet[0].sequence.size();
        clusters->at(i).idCluster=i;
        clusters->at(i).center=spliData[action%Num_Actions].dataSet[index1].sequence[index2];        
        action++; 
  }     
}

float calcClustersDistance(vector<Cluster> clusters){    
    float distance, actualdist;

    distance = 999999999999999999;

    //DistanciaActual= Encontrar los centroides de Solucion, i y j, tales que la distancia entre i y j sea mínima
    for (int i = 0; i < numClusters; i++) {
        for (int j = 0; j < numClusters; j++) {
            if (i < j) {
                actualdist = clusters[i].calcDistance(clusters[j].center);
                if (actualdist < distance)
                    distance = actualdist;
            }
        }
    }

    return distance;
}

ClusterData::ClusterData(HarusDataSet data) {
    clusters.resize(numClusters);
    
    /***********************START ALGORITHM***************************************************/
    srand(time(0));
    
    vector<Cluster> bestClusters;
    bestClusters.resize(numClusters);
    float clusterDistance = -99999999999999999;
    float actualDistance;

    for (int iterations = 0; iterations < Max_Iterations; iterations++) { //multistart  
        calcInitialClusters(&clusters, data); //for initial execution            
        calcKmeans(clusters, data);
        actualDistance = calcClustersDistance(clusters);
       
        if (actualDistance > clusterDistance) {
            bestClusters=clusters;
            clusterDistance = actualDistance;
            cout << "Better clusters found, distance: " << clusterDistance << endl;
        }
    }//end iterations MultiStart
    
    clusters=bestClusters;
}

void ClusterData::save(){      
    openMysqlSession();
    
    PreparedStatement *prep_stmt;
    prep_stmt = con -> prepareStatement(
            "INSERT INTO `ClusterData`(`id_cluster`, `data`, `compression_rate`, `num_clusters`) VALUES (?,?,?,?)"                 
            );
    for(int i=0;i< clusters.size();i++) {
        std::ostringstream s1;

        for (int j = 0; j< Vector_Size; j++) {
            s1 << clusters[i].center[j]<<" ";

        }       
        
        prep_stmt -> setInt(1,  i);
        prep_stmt -> setString(2, s1.str());
        prep_stmt -> setInt(3, compressionRate);
        prep_stmt -> setInt(4, numClusters);        
        prep_stmt-> executeUpdate();
        prep_stmt-> clearParameters();
    }
    
    delete prep_stmt;     
    
    closeMysqlSession();
}

void ClusterData::read(){  
    openMysqlSession();
    
    ResultSet *resSet;
    PreparedStatement *prep_stmt;

    /* create a statement object */
    prep_stmt = con -> prepareStatement("SELECT * FROM `ClusterData` WHERE `compression_rate`=? AND `num_clusters`=? ORDER BY `id_cluster`");
    prep_stmt->setInt(1, compressionRate);  
    prep_stmt->setInt(2, numClusters);      
    resSet = prep_stmt->executeQuery();

    while (resSet->next()) {       
        std::stringstream datastream(resSet->getString("data"));        
        //for each loop we save one HarusData
        vector<float> data(Vector_Size, 0);                     
        for(int i=0;i<Vector_Size;i++){          
                datastream>> data[i];
        }        
        Cluster newCluster;
        newCluster.center=data;
        newCluster.idCluster=resSet->getInt("id_cluster");
        this->clusters.push_back(newCluster);
    }
    
    cout<<"Clusters readed:"<<this->clusters.size()<<" clusters"<<endl;
    delete resSet;
    delete prep_stmt;
    
    closeMysqlSession();
}