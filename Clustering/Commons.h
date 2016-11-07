/* 
 * File:   Commons.h
 * Author: olmin
 *
 * Created on 27 de octubre de 2011, 14:03
 */

#ifndef COMMONS_H
#define	COMMONS_H

#include "config.h"
#include "SimpleSkeleton.h"
#include "BehaviourSequence.h"


using namespace std;
using namespace sql;

void connectSQL();

void disconnectSQL();

 string getTimes(vector<double>& times, vector<int>& counts);
 
list<int> getListIdBehaviours(Connection *con);

/*! Return the list of all ids. If type= Training or Test "otherListIds" will have the rest of ids not in the returned list */
vector<int> getListIdBehFromAction(Connection *con, int idAction, int type, list<int> *otherListIds );

/*! Returns the kind of action from the listActions. E.g. 0 if walk; 1 if sit down, etc.*/
int getTypeAction(Connection *con, int idAction);

void addSkeletonsFromIdBehaviour(Connection *con, int idBehaviour,list<SimpleSkeleton> *listSkel );

/*!Gets the skeleton data of the PAASkeleton table in DB*/
list<SimpleSkeleton> getDataSkeletons(Connection *con);

/*!Gets the skeleton data of the PAASkeleton table in DB from selected action. type= ALL, Training, Test, Validation*/
list<SimpleSkeleton> getDataSkeletonsFromAction(Connection *con, int idAction, int type);

void assignSkeleton2Clusters(SimpleSkeleton* clusters, SimpleSkeleton *dataSkeletons);
void assignSkeleton2Clusters(vector<SimpleSkeleton> clusters, SimpleSkeleton *dataSkeleton);

void assignSkeletons2Clusters(SimpleSkeleton clusters[], list<SimpleSkeleton> *dataSkeletons);
void assignSkeletons2Clusters(vector<SimpleSkeleton> clusters, list<SimpleSkeleton> *dataSkeletons);

void getClustersFromDB(Connection *con, SimpleSkeleton clusters[]);
void getClustersFromDB(Connection *con, vector<SimpleSkeleton> *clusters);

float calcClustersDistance(SimpleSkeleton clusters[]);
float calcClustersDistance(vector<SimpleSkeleton> clusters);

bool equalClusters(SimpleSkeleton clust1[],SimpleSkeleton clust2[]);
bool equalClusters(vector<SimpleSkeleton> clust1, vector<SimpleSkeleton> clust2);

SimpleSkeleton* getCopyClusters(SimpleSkeleton* copy, SimpleSkeleton* clusters);

void saveSelectedAction(Connection *con, int selectedAction, int idBehaviour);

list<BehaviourSequence> getBehavioursSequences(Connection *con, int idAction, int type);
list<BehaviourSequence> getBehavioursSequencesWithoutSkel(Connection *con, int idAction, int type);

list<BehaviourSequence> setTrainingAndTest(list<BehaviourSequence> *dataAction,
        list<BehaviourSequence> *dataTrainingGlobal, list<BehaviourSequence> *dataTestGlobal);

 BehaviourSequence getBehaviour(Connection *con, int idBehaviour);
 
 vector<int> getIdsVector(string ids);
 
 list<BehaviourSequence> getTrainedData(Connection *con,int idAction);
 
 void saveValidationDataKTest(Connection *con, int errors,       
                 vector<int> * errorsByAction, vector<int> *errorsByActionSel);
 
  void saveKTestTrainingData(Connection *con, string idsTraining, 
         int bestErrorTrain, int bestErrorTest,
         int worstErrorTrain, int worstErrorTest,
         int meanErrorTrain, int meanErrorTest,
         vector<int> *errorsTrainByAction, vector<int> *errorsTrainByActionSel,
         vector<int> *errorsTestByAction, vector<int> *errorsTestByActionSel);
  
 string getIdsTraining(list<BehaviourSequence> *dataTraining);
 
 
 
 void saveValidationData(Connection *con,double total, double errors,       
                 vector<int> * errorsByAction, vector<int> *errorsByActionSel, double timeLearning, double timeDetection);
 
 vector<int> getSequence(string sequence);
 
 void setIdCluster2Skeletons(BehaviourSequence *beh);
 
  void saveTrainingData(Connection *con, string idsTraining, 
         double bestErrorTrain, double bestErrorTest,
         double worstErrorTrain, double worstErrorTest,
         double meanErrorTrain, double meanErrorTest,
          vector<int> *errorsTrainByAction, vector<vector<int> >*errorsTrainByActionSel,
         vector<int> *errorsTestByAction, vector<vector<int> > *errorsTestByActionSel,
        string timeLearn, string timeDetect);
 
  
  void  addObjects2Behaviour(BehaviourSequence *beh);
  int getObjectType(int idBehaviour, int object_id);
  void includeNullActions(list<BehaviourSequence> &dataTestGlobal);
  
  int getMaxLength(list<BehaviourSequence>& dataTraining);
int getMinLength(list<BehaviourSequence>& dataTraining);

#endif	/* COMMONS_H */

