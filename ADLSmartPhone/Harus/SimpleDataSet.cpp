/* 
 * File:   SimpleDataSet.cpp
 * Author: olmin
 * 
 * Created on 29 de enero de 2013, 11:08
 */

#include "SimpleDataSet.h"
#include "ClusterData.h"
#include "Common.h"

SimpleDataSet::SimpleDataSet(HarusDataSet dataSet, ClusterData clusters) {
    
    assignClusters2Skeletons(clusters.clusters, &dataSet);
    
    for(int i=0;i <dataSet.dataSet.size();i++){
        SimpleData newData;
        newData.idAction=dataSet.dataSet[i].idAction;
        newData.sequence=dataSet.dataSet[i].idsCluster;
        this->dataSet.push_back(newData);
    }
}


void SimpleDataSet::save(int type){        
    openMysqlSession();
    
    cout<<"Saving:"<<this->dataSet.size()<<" simpledata"<<endl;
    
    PreparedStatement *prep_stmt;
    prep_stmt = con -> prepareStatement(            
            "INSERT INTO `SimpleDataSet`(`id_action`, `compression_rate`, `num_clusters`, `sequence` ,`id`,`type` ) VALUES (?,?,?,?,?,?)"
            );
    
    for(int i=0;i< dataSet.size();i++) {
        std::ostringstream s1;

        for (int j = 0; j< dataSet[i].sequence.size() ; j++) {
            s1 <<" "<< dataSet[i].sequence[j];

        }       
        
        prep_stmt -> setInt(1,  dataSet[i].idAction);       
        prep_stmt -> setInt(2, compressionRate);
        prep_stmt -> setInt(3, numClusters);        
        prep_stmt -> setString(4, s1.str());
        prep_stmt -> setInt(5, i);   
        prep_stmt -> setInt(6, type);  
        prep_stmt-> executeUpdate();
        prep_stmt-> clearParameters();
    }
    
    delete prep_stmt;    
    closeMysqlSession();
}

void SimpleDataSet::read(int type){  
    openMysqlSession();
    
    this->dataSet.clear();
    
    ResultSet *resSet;
    PreparedStatement *prep_stmt;

    /* create a statement object */
    prep_stmt = con -> prepareStatement("SELECT * FROM `SimpleDataSet` WHERE `compression_rate`=? AND `num_clusters`=? AND `type`=?");
    prep_stmt->setInt(1, compressionRate);  
    prep_stmt->setInt(2, numClusters);      
    prep_stmt->setInt(3, type);      
    resSet = prep_stmt->executeQuery();

    while (resSet->next()) {       
        std::stringstream datastream(resSet->getString("sequence"));        
        //for each loop we save one sequence
        vector<int> sequence;
        while (!datastream.eof()) {
            int state;
            datastream >> state;
            sequence.push_back(state);
        }        
        SimpleData newSD;
        newSD.sequence=sequence;
        newSD.idAction=resSet->getInt("id_action");
        this->dataSet.push_back(newSD);
    }
    
    delete resSet;
    delete prep_stmt;
    
    closeMysqlSession();
    
    cout<<"SimpleData readad: "<<this->dataSet.size()<<" data, each sequence length:"<<this->dataSet[0].sequence.size()<<endl;
    
}


vector<SimpleDataSet> splitActions(SimpleDataSet data){
 
    vector<SimpleDataSet> dataActions;
    dataActions.resize(Num_Actions);    
    for(int i =0;i< data.dataSet.size();i++){
       // int index= rand() % data.dataSet.size();
        dataActions[data.dataSet[i].idAction-1].dataSet.push_back(data.dataSet[i]);                
    }    
    return dataActions;   
}

vector<vector<int> > getRawData (SimpleDataSet data){
    vector<vector<int> > rawData;
    rawData.resize(data.dataSet.size());
    for(int i=0;i<rawData.size();i++){
        rawData[i].resize(data.dataSet[i].sequence.size());
        rawData[i]=data.dataSet[i].sequence;
    }    
    return rawData;    
}