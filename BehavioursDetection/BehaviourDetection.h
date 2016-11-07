/* 
 * File:   BehaviourDetection.h
 * Author: olmin
 *
 * Created on 11 de enero de 2013, 12:24
 */

#ifndef BEHAVIOURDETECTION_H
#define	BEHAVIOURDETECTION_H

#include "../Clustering/config.h"
#include "../Clustering/Commons.h"
#include "../Clustering/BehaviourSequence.h"
#include "../Clustering/ClusterData.h"
#include "../Clustering/SimpleSkeleton.h"
#include "../Clustering/PAA.h"
#include "../BehaviourInferenceOlmoAutomata/OlmoAutomata.h"
#include "../Inference/HMM.h"

#include <boost/ptr_container/ptr_vector.hpp>

using namespace boost;
using namespace std;
using namespace sql;


class BehaviourDetection {
public:
    BehaviourDetection(int argc, char **argv);
     string addFrame(SimpleSkeleton skeleton);
    
private:
    //one automata for each action
//    vector<InferenceClassInterface*> models;
    boost::ptr_vector<InferenceClassInterface> models;
    vector<SimpleSkeleton> clusters;
    vector<SimpleSkeleton> toResume;
//    bool firstTime=true;
};

#endif	/* BEHAVIOURDETECTION_H */

