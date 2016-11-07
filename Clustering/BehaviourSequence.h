/* 
 * File:   BehaviourSequence.h
 * Author: olmin
 *
 * Created on 4 de noviembre de 2011, 20:49
 */

#ifndef BEHAVIOURSEQUENCE_H
#define	BEHAVIOURSEQUENCE_H
#include "config.h"
#include "SimpleSkeleton.h"
#include "Object.h"
        
class BehaviourSequence {
public:
//    BehaviourSequence();
//    BehaviourSequence(const BehaviourSequence& orig);
//    virtual ~BehaviourSequence();
//    
    int idBehaviour;
    int idUser;
    int idAction;
    int compressionRate;
    //int numClusters;
    list<SimpleSkeleton> skeletons;
    vector<int> sequence;
    vector<vector<Object> > objectsData;
private:

};

#endif	/* BEHAVIOURSEQUENCE_H */

