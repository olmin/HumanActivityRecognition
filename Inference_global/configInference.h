/* 
 * File:   configInference.h
 * Author: olmin
 *
 * Created on 5 de noviembre de 2013, 12:26
 */

#ifndef CONFIGINFERENCE_H
#define	CONFIGINFERENCE_H

#include "InferenceClassInterface.h"

//add your inference class
//#include "../Inference_HMM/HMM.h"
//#include "../Inference_DTW/DTWInference.h"
#include "../MSRDailyAct3D/Inference_OlmoAutomata_MSR/OlmoAutomata.h"

//#include "../datasetCAD120/Inference_DTW_Objects/DTWInference.h"
//#include "../datasetCAD120/Inference_OlmoAutomata_Objects/OlmoAutomata.h"


InferenceClassInterface *getInferenceModel() {

    InferenceClassInterface *inferenceObj;
//  
//    if (InferenceClassUsed == "DTW") {
//        inferenceObj = new DTWInference;
//    }

//    if (InferenceClassUsed == "HMM") {
//        inferenceObj = new HMM;
//    }
    
    if (InferenceClassUsed == "OlmoAutomata") {
        inferenceObj = new OlmoAutomata;
    }

    return inferenceObj;
}


#endif	/* CONFIGINFERENCE_H */

