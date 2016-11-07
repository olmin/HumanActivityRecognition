/* 
 * File:   SimpleDataSet.h
 * Author: olmin
 *
 * Created on 29 de enero de 2013, 11:08
 */

#ifndef SIMPLEDATASET_H
#define	SIMPLEDATASET_H

#include "config.h"
#include "HarusDataSet.h"
#include "ClusterData.h"

class SimpleData {
public:
    int idAction;
    vector<int> sequence;
};

class SimpleDataSet {
public:
    SimpleDataSet(){}
    SimpleDataSet(HarusDataSet dataSet, ClusterData clusters);
    void save(int type);
    void read(int type);
//private:
    vector<SimpleData> dataSet;

};

vector<SimpleDataSet> splitActions(SimpleDataSet data);

vector<vector<int> > getRawData (SimpleDataSet data);

#endif	/* SIMPLEDATASET_H */

