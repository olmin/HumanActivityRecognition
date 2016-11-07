/* 
 * File:   Object.h
 * Author: olmin
 *
 * Created on 24 de febrero de 2014, 16:01
 */

#ifndef OBJECT_H
#define	OBJECT_H

#include "vec3.h"
#include "config.h"

class Object {
public:
    Object(){};
    int id;
    int object_type;
    double distance2hand;
    vec3 position;    
    
    double getMove(Object prev);
private:

};

double distanceOfObjects(vector<Object> objects);

double getObjectsMove(vector<Object> objects1, vector<Object> objects2);

#endif	/* OBJECT_H */

