/* 
 * File:   DTW.h
 * Author: olmin
 *
 * Created on 4 de noviembre de 2013, 14:19
 */

#ifndef DTW_H
#define	DTW_H

#include "../Clustering/config.h"
//#include "../Clustering/vec3.h"

/* Par de enteros */
class IntPair {
public:
    int X, Y;

    IntPair(int x, int y) {

        X = x;
        Y = y;
    }
};

/* Emparejamiento de DTW */
class DTWMatching {
public:
    // Valor del emparejamiento
    double value;

    vector< vector<double> > cost;
    vector< vector<double> > matrix;
    // Indices de la serie destino donde se emparejan los indices de la serie origen
    vector<IntPair> path;
    
    void DTWClassic(double ***s, int sIni, int sEnd, double ***t, int tIni, int tEnd);
    
};

//class DTW {
//public:
   

    //    static Dist1D distance;

    /* Reduce el emparejamiento entre 2 series s[sIni..sEnd] y t[tIni...tEnd] para que un punto de s no se empareje
            con varios puntos de t, atendiendo al criterio de mínima distancia */
//    void MinimizeMatching(DTWMatching *matching, vector<int> s, vector<int> t);
//    DTWMatching DTWClassic(vector<int> s, int sIni, int sEnd, vector<int> t, int tIni, int tEnd);
//    DTWMatching DTWSubsequence(vector<int> s, int sIni, int sEnd, vector<int> t, int tIni, int tEnd);   
    
//    void MinimizeMatching(DTWMatching *matching, vector< vector <vec3> > s, vector< vector <vec3> > t);
//    DTWMatching DTWClassic(double ***s, int sIni, int sEnd, double ***t, int tIni, int tEnd);
//    DTWMatching DTWSubsequence(vector< vector <vec3> > s, int sIni, int sEnd, vector< vector <vec3> > t, int tIni, int tEnd);    
//};

#endif	/* DTW_H */

