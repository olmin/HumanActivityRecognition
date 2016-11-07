/* 
 * File:   InferenceClassInterface.h
 * Author: olmin
 *
 * Created on 29 de enero de 2013, 13:11
 */

#ifndef INFERENCECLASSINTERFACE_H
#define	INFERENCECLASSINTERFACE_H

#include "config.h"
#include "SimpleDataSet.h"

class InferenceClassInterface {//: boost::noncopyable{
public:
    virtual void startInference(ClusterData clusters, HarusDataSet trainDataSet)=0;
    
    virtual void save()=0;
    
    virtual void read(int idAction)=0;
    
    virtual void checkNewSequence(vector<int> sequence)=0;
    
    virtual double getActualProbability()=0;
    
    virtual bool isFinalState()=0;

    virtual void addNewState(int newState)=0;
    
    virtual int getIdAction()=0;    
};

#endif	/* INFERENCECLASSINTERFACE_H */

