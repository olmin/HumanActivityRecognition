/* 
 * File:   DTW.h
 * Author: olmin
 *
 * Created on 4 de noviembre de 2013, 14:19
 */

#ifndef DTW_H
#define	DTW_H

#include "../Clustering/config.h"

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

    // Indices de la serie destino donde se emparejan los indices de la serie origen
    vector<IntPair> path;
};

//class DTW {
//public:
    static vector< vector<double> > cost;
    static vector< vector<double> > matrix;

    //    static Dist1D distance;

    /* Reduce el emparejamiento entre 2 series s[sIni..sEnd] y t[tIni...tEnd] para que un punto de s no se empareje
            con varios puntos de t, atendiendo al criterio de mínima distancia */
//    void MinimizeMatching(DTWMatching *matching, vector<int> s, vector<int> t);
//    DTWMatching DTWClassic(vector<int> s, int sIni, int sEnd, vector<int> t, int tIni, int tEnd);
//    DTWMatching DTWSubsequence(vector<int> s, int sIni, int sEnd, vector<int> t, int tIni, int tEnd);   
    
//    void MinimizeMatching(DTWMatching *matching, vector< vector <double> > s, vector< vector <double> > t);
//    DTWMatching DTWClassic(vector< vector <double> > s, int sIni, int sEnd, vector< vector <double> > t, int tIni, int tEnd);
//    DTWMatching DTWSubsequence(vector< vector <double> > s, int sIni, int sEnd, vector< vector <double> > t, int tIni, int tEnd);    
//    
     DTWMatching DTWClassic(double **s, int sIni, int sEnd, double **t, int tIni, int tEnd);
//};

#endif	/* DTW_H */

