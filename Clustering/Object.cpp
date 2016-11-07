/* 
 * File:   Object.cpp
 * Author: olmin
 * 
 * Created on 24 de febrero de 2014, 16:01
 */

#include "Object.h"



double distanceOfObjects(vector<Object> objects){
    double dist=0; 
    for(int i=0;i< objects.size();i++)
        for(int j=0;j< objects.size();j++)
            if(i<j){
                dist+=vec3(objects[i].position-objects[j].position).Length();                
//                dist+=fabs(objects[i].position.x-objects[j].position.x);
//                dist+=fabs(objects[i].position.y-objects[j].position.y);
//                dist+=fabs(objects[i].position.z-objects[j].position.z);
            }
    
//    return (Max_Object_Dist*3 -dist)/(double)Max_Object_Dist*3 ;
    return dist/objects.size();
}

double getObjectsMove(vector<Object> objects1, vector<Object> objects2){
     double dist=0;
    for(int i=0;i< objects1.size();i++)
        dist+=vec3(objects1[i].position-objects2[i].position).Length();
     
     return dist;
}

double Object::getMove(Object prev){
    return vec3(position-prev.position).Length();
}