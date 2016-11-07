/* 
 * File:   Skeleton.h
 * Author: olmin
 *
 * Created on 29 de noviembre de 2013, 12:49
 */

#ifndef SKELETON_H
#define	SKELETON_H
#include "../Clustering/config.h"
//#include"../Clustering/vec3.h"

using namespace std;

using namespace sql;

class Skeleton {
public:
   
//     vec3 pos3D;
     int idBehaviour;
     int frame;    
     double **joints;
     
private:

};

class SkeletonSequence {
public:
  
    int idAction;
    int idBehaviour;
    int idUser;
    vector<Skeleton> sequence;
     
private:

};

vector<vector< vector<SkeletonSequence> > >  getKFoldsVector(Connection *con, list<SkeletonSequence> *validationData);

list<SkeletonSequence> setTrainingAndTestKFold(vector< vector<SkeletonSequence> >  data, int kFold, 
        list<SkeletonSequence> *dataTrainingGlobal, list<SkeletonSequence> *dataTestGlobal);

string getIdsTraining(list<SkeletonSequence> *dataTraining);
  
#endif	/* SKELETON_H */

