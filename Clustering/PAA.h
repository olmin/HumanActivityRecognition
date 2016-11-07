/* 
 * File:   PAA.h
 * Author: olmin
 *
 * Created on 27 de octubre de 2011, 14:03
 */

#ifndef PAA_H
#define	PAA_H

#include "config.h"
#include "SimpleSkeleton.h"

using namespace std;
using namespace sql;

void startPAA(Connection *con) ;

SimpleSkeleton getPAASkeletons(vector<SimpleSkeleton> vectorSkel);

#endif	/* PAA_H */

