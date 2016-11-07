/* 
 * File:   HarusData.h
 * Author: olmin
 *
 * Created on 29 de enero de 2013, 10:46
 */

#ifndef HARUSDATASET_H
#define	HARUSDATASET_H


#include "config.h"

class HarusData {
public:
    HarusData(){}
    HarusData(int idAction,vector<vector<float> > sequence){
        this->idAction=idAction;
        this->sequence=sequence;
    }
    //void compressPAA();   
//protected:
    int idAction;
    vector<vector<float> > sequence;    
    vector<int> idsCluster;
};

class HarusDataSet {
public:
    HarusDataSet(){}
    HarusDataSet(string path2files,string fileNameLabels);
    void compressPAA();
    void save(int type);
    void read(int type);
//private:
    vector<HarusData> dataSet;    
//    vector<HarusData> paaData;
};

vector<HarusDataSet> splitActions(HarusDataSet data);

#endif	/* HARUSDATASET_H */

