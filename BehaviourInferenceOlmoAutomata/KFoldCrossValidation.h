/* 
 * File:   KFoldCrossValidation.h
 * Author: olmin
 *
 * Created on 9 de mayo de 2013, 12:56
 */

#ifndef KFOLDCROSSVALIDATION_H
#define	KFOLDCROSSVALIDATION_H

#include "../Clustering/config.h"
#include "../Clustering/BehaviourSequence.h"
#include "../Clustering/Commons.h"

using namespace std;
using namespace sql;

vector<vector< vector<BehaviourSequence> > >  getKFoldsVector(Connection *con,list<BehaviourSequence> *validationData);

list<BehaviourSequence> setTrainingAndTestKFold(vector< vector<BehaviourSequence> >  data, int kFold, 
        list<BehaviourSequence> *dataTrainingGlobal, list<BehaviourSequence> *dataTestGlobal);

#endif	/* KFOLDCROSSVALIDATION_H */

