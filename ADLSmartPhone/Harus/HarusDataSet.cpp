/* 
 * File:   HarusDataSet.cpp
 * Author: olmin
 * 
 * Created on 29 de enero de 2013, 10:46
 */

#include <ios>


#include "HarusDataSet.h"
#include "Common.h"

HarusDataSet::HarusDataSet(string path2files, string fileNameLabels) {    

    std::ostringstream s1;
    s1<< path2files<< BodyAccX;       
    std::ifstream bodyAccX(s1.str().c_str());  
   
    std::ostringstream s2;
    s2<< path2files<< BodyAccY;                      
    std::ifstream bodyAccY(s2.str().c_str());  
     std::ostringstream s3;
    s3<< path2files<< BodyAccZ;        
    std::ifstream bodyAccZ(s3.str().c_str());  
     std::ostringstream s4;
    s4<< path2files<< BodyGyroX;        
    std::ifstream bodyGyroX(s4.str().c_str());  
     std::ostringstream s5;
    s5<< path2files<< BodyGyroY;        
    std::ifstream bodyGyroY(s5.str().c_str());  
     std::ostringstream s6;
    s6<< path2files<< BodyGyroZ;        
    std::ifstream bodyGyroZ(s6.str().c_str());  
    
  
    std::ifstream labelsFile(fileNameLabels.c_str());
    
    string lineLabel;   
    string lineAccX, lineAccY, lineAccZ;   
    string lineGyroX, lineGyroY, lineGyroZ;          
    
    while (getline(bodyAccX, lineAccX) && 
            getline(bodyAccY, lineAccY) && 
            getline(bodyAccZ, lineAccZ) && 
            getline(bodyGyroX, lineGyroX) && 
            getline(bodyGyroY, lineGyroY) && 
            getline(bodyGyroZ, lineGyroZ)  && 
            getline(labelsFile, lineLabel)   ) {
           
        std::stringstream labelstream(lineLabel);
        int idAction; 
        labelstream>> idAction;
        
        std::stringstream streamAccX(lineAccX);
        std::stringstream streamAccY(lineAccY); 
        std::stringstream streamAccZ(lineAccZ); 
        std::stringstream streamGyroX(lineGyroX); 
        std::stringstream streamGyroY(lineGyroY); 
        std::stringstream streamGyroZ(lineGyroZ); 
        
        //for each loop we save one HarusData     
        vector<vector<float> > allSeqs;
        for(int i=0;i<Sequence_Length;i++){  
              vector<float> data(Vector_Size, 0);   
              streamAccX>> data[0];
              streamAccY>> data[1];
              streamAccZ>> data[2];
              streamGyroX>> data[3];
              streamGyroY>> data[4];
              streamGyroZ>> data[5]; 
              
              allSeqs.push_back(data);       
        }        
        
        HarusData newData(idAction, allSeqs);
        this->dataSet.push_back(newData);
    }
    bodyAccX.close();
    bodyAccY.close();
    bodyAccZ.close();
    bodyGyroX.close();
    bodyGyroY.close();
    bodyGyroZ.close();
    labelsFile.close();
    
    cout<<"Data readed:"<< this->dataSet.size()*Sequence_Length<<endl;

}

vector<float> compressSimpleVector(vector<vector<float> > simpleVector) {
       
    vector<float> compressData;
    
    int size=simpleVector.size();      
    compressData.resize(Vector_Size, 0);
    for (int i = 0; i < Vector_Size; i++) {
        compressData[i] = 0;
        for (int j = 0; j < size; j++) {
            compressData[i] += simpleVector[j][i];
        }
        compressData[i] /= size;
    }     
    return compressData;
}


void HarusDataSet::compressPAA(){
    
    vector<HarusData> newDataSet;
    int newLength;
    
    //we loop every different action
    for (int i = 0; i< this->dataSet.size(); i++) {
        vector<vector<float> > toCompress;
        HarusData newHarusData;        
        
        //we loop the sequence of the action
        for (int j = 0; j < this->dataSet[i].sequence.size(); j++) {           
            if (toCompress.size() == compressionRate) {//we compress the data because we reach the compression rate                
                newHarusData.sequence.push_back(compressSimpleVector(toCompress));
                toCompress.clear();
            }            
            toCompress.push_back(this->dataSet[i].sequence[j]);
        }           
        if (toCompress.size() != 0) {
            //the rest of the data      
            newHarusData.sequence.push_back(compressSimpleVector(toCompress));
            toCompress.clear();
        }    
        newHarusData.idAction=this->dataSet[i].idAction;
        newDataSet.push_back(newHarusData); 
        newLength=newHarusData.sequence.size();
    }

    this->dataSet=newDataSet;    
    
    cout<<"Data compressed:"<< this->dataSet.size()*newLength<<endl;
}

void HarusDataSet::save(int type){
    
    openMysqlSession();
    
    //first we check it hasent been saved before
    ResultSet *resSet;
     PreparedStatement *prep_stmt1;
     prep_stmt1 = con -> prepareStatement(
            "SELECT COUNT( * )  FROM `HarusDataSet` WHERE `compression_rate` =?"                 
            );
     prep_stmt1 -> setInt(1,compressionRate);
     resSet = prep_stmt1->executeQuery();
      while (resSet->next()) {       
       int rows=resSet->getInt(1);
       if(rows!=0)//we dont save them
           return;
      }
     
    PreparedStatement *prep_stmt;
    prep_stmt = con -> prepareStatement(
            "INSERT INTO `HarusDataSet`(`id_action`, `data`, `type`,`id`,`compression_rate`) VALUES (?,?,?,?,?)"                 
            );
   
    for(int i=0;i< dataSet.size();i++) {        
        std::ostringstream s1;
        for (int j = 0; j< dataSet[i].sequence.size(); j++) {
            for (int k = 0; k< Vector_Size; k++) {
                s1<<" " << dataSet[i].sequence[j][k];
            } 
        }
        
        prep_stmt -> setInt(1,  dataSet[i].idAction);
        prep_stmt -> setString(2, s1.str());
        prep_stmt -> setInt(3, type);
        prep_stmt -> setInt(4, i);
        prep_stmt -> setInt(5, compressionRate);
        prep_stmt-> executeUpdate();
        prep_stmt-> clearParameters();
    }
    
    delete prep_stmt;     
    
    closeMysqlSession();
}

void HarusDataSet::read(int type){
    openMysqlSession();
    
    this->dataSet.clear();    
    
    ResultSet *resSet;
    PreparedStatement *prep_stmt;

    /* create a statement object */
    prep_stmt = con -> prepareStatement("SELECT * FROM `HarusDataSet` WHERE `compression_rate`=? AND `type`=? ORDER BY `id`");
    prep_stmt->setInt(1, compressionRate);  
    prep_stmt->setInt(2, type);  
    resSet = prep_stmt->executeQuery();

    int seqSize;
    
    while (resSet->next()) {       
        std::stringstream datastream(resSet->getString("data"));        
        vector<vector<float> > sequence;
        //for each loop we save one HarusData
        while (!datastream.eof()) {
            vector<float>  data(Vector_Size, 0);
            for (int k = 0; k< Vector_Size; k++) {
                datastream>> data[k];                
            } 
            sequence.push_back(data);
        }      
        HarusData newData(resSet->getInt("id_action"), sequence);
        seqSize=sequence.size();
        this->dataSet.push_back(newData);
    }
    
    delete resSet;
    delete prep_stmt;
    
    cout<<"Data readed:"<< this->dataSet.size()<<", seq size:"<<seqSize<<endl;
    
    closeMysqlSession();
    
}

vector<HarusDataSet> splitActions(HarusDataSet data){
    
    vector<HarusDataSet> dataActions;
    dataActions.resize(Num_Actions);
    
    for(int i =0;i< data.dataSet.size();i++){
//        int index= rand() % data.dataSet.size();
        dataActions[data.dataSet[i].idAction-1].dataSet.push_back(data.dataSet[i]);                
    }
    
    return dataActions;    
}