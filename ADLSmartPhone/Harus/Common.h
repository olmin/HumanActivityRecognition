/* 
 * File:   Common.h
 * Author: olmin
 *
 * Created on 29 de enero de 2013, 12:06
 */

#ifndef COMMON_H
#define	COMMON_H

#include "config.h"
#include "InferenceClassInterface.h"
#include "HMMDataSet.h"
#include "OlmoAutomata.h"
#include "ClusterData.h"
#include "SimpleDataSet.h"
#include "HarusDataSet.h"

void openMysqlSession();

void closeMysqlSession();

void saveResults(vector<vector<int> > errors, int dataType);

int compareModels(vector<InferenceClassInterface*> models, vector<int> behSeq);

vector<vector<int> > classify(vector<InferenceClassInterface*> models, SimpleDataSet data);

InferenceClassInterface* getInferenceObject();

void createInferenceData(vector<InferenceClassInterface*> *models, ClusterData clusters, HarusDataSet mainDataSet, SimpleDataSet trainDataSet) ;

#endif	/* COMMON_H */

