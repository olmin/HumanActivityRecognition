/* 
 * File:   DTW.cpp
 * Author: olmin
 * 
 * Created on 4 de noviembre de 2013, 14:19
 */

#include "DTW.h"

/* Euclidea 1D */
double Euclidean1D(double x, double y) {

    return fabs(x - y);
}

double DistMax(vector<double> x, vector<double> y){
    double distance,actualdist;
     int min = (x.size() < y.size()) ? x.size() : y.size();
     
      for (int j = 0; j < min; j++) {
            actualdist = fabs(x[j] - y[j]);
            if (actualdist > distance)
                distance = actualdist;           
        }
      return distance;
}

/* Euclidea */
double Euclidean(double *x, double *y) {

    double d = 0;
    int min = 3;//(sizeof(x) < sizeof(y)) ? sizeof(x) : sizeof(y);

    for (int i = 0; i < min; i++)
        d += (x[i] - y[i])*(x[i] - y[i]);

//    if (x.size() < y.size()) {
//        for (int i = min; i < x.size(); i++)
//            d += x[i] * x[i];
//
//    } else if (y.size() > x.size()) {
//        for (int i = min; i < y.size(); i++)
//            d += y[i] * y[i];
//    }

    d = sqrt(d);
    return d;
}

double Max3D(double **x, double **y){
    double distance,actualdist;
    int min = 15;//(sizeof(x) < sizeof(y)) ? sizeof(x) : sizeof(y);
        
    distance= 0;
      for (int i = 0; i < min; i++) {         
           actualdist=0;
           for (int j = 0; j < 3; j++){
            actualdist += fabs(x[i][j] - y[i][j]);                       
           }
            if (actualdist > distance)
                distance = actualdist;   
        }

      return distance;
}

/* Euclidea */
double Euclidean3D(double **x, double **y) {

    double actualdist;
    double d = 0;
   int min = 15;//(sizeof(x) < sizeof(y)) ? sizeof(x) : sizeof(y);

    for (int i = 0; i < min; i++){
        actualdist=0;
//          for (int j = 0; j < 3; j++){
//            actualdist += fabs(x[i][j] - y[i][j]);                       
//           }
//        d += actualdist*actualdist;
        d += Euclidean(x[i],y[i])*Euclidean(x[i],y[i]);
           
    }

//    if (x.size() < y.size()) {
//        for (int i = min; i < x.size(); i++)
//            d += x[i] * x[i];
//
//    } else if (y.size() > x.size()) {
//        for (int i = min; i < y.size(); i++)
//            d += y[i] * y[i];
//    }

    d = sqrt(d);
    return d;
}

double CityBlock(vector<double> x, vector<double> y) {

    double d = 0;
    int min = (x.size() < y.size()) ? x.size() : y.size();

    for (int i = 0; i < min; i++)
        d += fabs(x[i] - y[i]);
   
    return d;
}

double CityBlock3D(double **x, double **y) {

    double d = 0;
   int min = 15;//(sizeof(x) < sizeof(y)) ? sizeof(x) : sizeof(y);
   
    for (int i = 0; i < min; i++)
        for (int j = 0; j < 3; j++){
            d += fabs(x[i][j] - y[i][j]);                       
           }       
   
    return d;
}
//double Dist(double a, double b) {
//    return Euclidean1D(a, b);
//};

double Dist(double **a, double **b) {
     if(DistanceMeasureUsed=="Max")
       return Max3D(a,b);
    if(DistanceMeasureUsed=="Euclidean")
         return Euclidean3D(a,b);   
    if(DistanceMeasureUsed=="CityBlock")
          return CityBlock3D(a,b); 
//    srand(time(NULL));   
//    timeval ttime;
//    gettimeofday(&ttime, NULL);
//    return rand();
    
};

///* Reduce el emparejamiento entre 2 series s[sIni..sEnd] y t[tIni...tEnd] para que un punto de s no se empareje
//        con varios puntos de t, atendiendo al criterio de mínima distancia */
//void MinimizeMatching(DTWMatching *matching, vector<int> s, vector<int> t) {
//
//    vector<IntPair> path; // = new list<IntPair>();
//    int bestPairing;
//    double bestDist, aux;
//
//
//    bestPairing = 0;
//    bestDist = Dist(s[matching->path[0].X], t[matching->path[0].Y]);
//    for (int i = 0; i < matching->path.size() - 1; i++) {
//
//        if (matching->path[i].X == matching->path[i + 1].X) {
//
//            aux = Dist(s[matching->path[i + 1].X], t[matching->path[i + 1].Y]);
//            if (aux <= bestDist) {
//                bestDist = aux;
//                bestPairing = i + 1;
//            }
//
//        } else {
//            path.push_back(IntPair(matching->path[bestPairing].X, matching->path[bestPairing].Y));
//            bestDist = Dist(s[matching->path[i + 1].X], t[matching->path[i + 1].Y]);
//            bestPairing = i + 1;
//        }
//    }
//
//    if (matching->path[matching->path.size() - 2].X != matching->path[matching->path.size() - 1].X) {
//        path.push_back(IntPair(matching->path[bestPairing].X, matching->path[bestPairing].Y));
//    }
//
//    // Copiamos el path
//    matching->path.clear();
//    //        matching->path.resize(path.size(), new IntPair(0,0));
//    aux = 0;
//    for (int i = 0; i < path.size(); i++) {
//        matching->path.push_back(IntPair(path[i].X, path[i].Y));
//        aux += Dist(s[matching->path[i].X], t[matching->path[i].Y]);
//    }
//    matching->value = aux;
//}
//
///* DTW clásico, que empareja una serie {s[sIni]...s[sEnd]} con otra {t[tIni]...t[tEnd]}
//        Para medir distancias, se utiliza la función Dist entre 2 double */
//DTWMatching DTWClassic(vector<int> s, int sIni, int sEnd, vector<int> t, int tIni, int tEnd) {
//
//    DTWMatching matching; // = new DTWMatching();
//    //        distance = Dist;
//    int sLength = (sEnd - sIni + 1);
//    int tLength = (tEnd - tIni + 1);
//    double min;
//
//
//
//    if ((cost.size() < sLength) || (cost[0].size() < tLength)) {
//
//        //cost = new vector(sLength, vector(tLength));
//        cost.resize(sLength, vector<double>(tLength, 0));
//        //matrix = new vector(sLength, vector(tLength));
//        matrix.resize(sLength, vector<double>(tLength, 0));
//    }
//
//
//
//    // Calculo de costes iniciales e inicializacion
//    for (int i = sIni; i <= sEnd; i++)
//        for (int j = tIni; j <= tEnd; j++)
//            cost[i - sIni][j - tIni] = Dist(s[i], t[j]);
//
//
//
//    // Aplicamos DTW
//    for (int i = 0; i < matrix.size(); i++) {
//
//        for (int j = 0; j < matrix[0].size(); j++) {
//
//            min = DBL_MAX;
//            if (i > 0)
//                min = matrix[i - 1][j];
//            if (j > 0 && matrix[i][j - 1] < min)
//                min = matrix[i][j - 1];
//            if (i > 0 && j > 0 && matrix[i - 1][j - 1] < min)
//                min = matrix[i - 1][j - 1];
//            if (i == 0 && j == 0)
//                min = 0;
//
//            matrix[i][j] = cost[i][j] + min;
//        }
//    }
//    matching.value = matrix[matrix.size() - 1][matrix[0].size() - 1];
//
//
//
//    // Encontramos el emparejamiento
//    list<IntPair> path; // = new list<IntPair>();
//    int idxi, idxj, restai, restaj;
//
//    idxi = matrix.size() - 1;
//    idxj = matrix[0].size() - 1;
//    path.push_front(IntPair(idxi, idxj));
//    while (idxi != 0 || idxj != 0) {
//
//        if (idxi == 0)
//            idxj--;
//        else if (idxj == 0)
//            idxi--;
//        else {
//
//            min = matrix[idxi - 1][idxj - 1];
//            restai = 1;
//            restaj = 1;
//
//            if (min >= matrix[idxi - 1][idxj]) {
//                min = matrix[idxi - 1][idxj];
//                restai = 1;
//                restaj = 0;
//            }
//
//            if (min >= matrix[idxi][idxj - 1]) {
//                min = matrix[idxi][idxj - 1];
//                restai = 0;
//                restaj = 1;
//            }
//
//            idxi -= restai;
//            idxj -= restaj;
//        }
//
//        path.push_front(IntPair(idxi, idxj));
//    }
//
//    // Copiamos el path
//    matching.path.clear(); // = new IntPair[path.size()];      
//    for (list<IntPair>::iterator it = path.begin(); it != path.end(); it++)
//        matching.path.push_back(IntPair(it->X, it->Y));
//
//
//    return matching;
//}
//
///* DTW modificado. que empareja una serie {s[sIni]...s[sEnd]} con una subsecuencia de {t[tIni]...t[tEnd]}
//        Para medir distancias, se utiliza la función Dist entre 2 double */
// DTWMatching DTWSubsequence(vector<int> s, int sIni, int sEnd, vector<int> t, int tIni, int tEnd) {
//
//    DTWMatching matching; // = new DTWMatching();
//    //        distance = Dist;
//    int sLength = (sEnd - sIni + 1);
//    int tLength = (tEnd - tIni + 1);
//    double min;
//
//
//
//    if ((cost.size() < sLength) || (cost[0].size() < tLength)) {
//
//        //cost = new vector(sLength, vector(tLength));
//        cost.resize(sLength, vector<double>(tLength, 0));
//        //matrix = new vector(sLength, vector(tLength));
//        matrix.resize(sLength, vector<double>(tLength, 0));
//
//    }
//
//    // Calculo de costes iniciales e inicializacion
//    for (int i = sIni; i <= sEnd; i++)
//        for (int j = tIni; j <= tEnd; j++)
//            cost[i - sIni][j - tIni] = Dist(s[i], t[j]);
//
//
//
//    // Aplicamos DTW
//    for (int i = 0; i < matrix.size(); i++) {
//
//        for (int j = 0; j < matrix[0].size(); j++) {
//
//            min = DBL_MAX;
//            if (i > 0)
//                min = matrix[i - 1][j];
//            if (j > 0 && matrix[i][j - 1] < min)
//                min = matrix[i][j - 1];
//            if (i > 0 && j > 0 && matrix[i - 1][j - 1] < min)
//                min = matrix[i - 1][j - 1];
//            if (i == 0 && j == 0)
//                min = 0;
//
//            if (i > 0)
//                matrix[i][j] = cost[i][j] + min;
//            else
//                matrix[i][j] = cost[i][j];
//        }
//    }
//
//
//    // Encontramos el emparejamiento
//    int idxi, idxj, restai, restaj;
//    list<IntPair> path; // = new list<IntPair>();
//
//    idxi = matrix.size() - 1;
//    idxj = 0;
//    for (int i = 0; i < matrix[0].size(); i++)
//        if (matrix[idxi][i] <= matrix[idxi][idxj])
//            idxj = i;
//    matching.value = matrix[idxi][idxj];
//    path.push_front(IntPair(idxi, idxj));
//    while (idxi != 0) {
//
//        if (idxj == 0)
//            idxi--;
//        else {
//
//            min = matrix[idxi - 1][idxj - 1];
//            restai = 1;
//            restaj = 1;
//
//            if (min >= matrix[idxi - 1][idxj]) {
//                min = matrix[idxi - 1][idxj];
//                restai = 1;
//                restaj = 0;
//            }
//
//            if (min >= matrix[idxi][idxj - 1]) {
//                min = matrix[idxi][idxj - 1];
//                restai = 0;
//                restaj = 1;
//            }
//
//            idxi -= restai;
//            idxj -= restaj;
//        }
//
//        path.push_front(IntPair(idxi, idxj));
//    }
//
//
//    // Copiamos el path
//    matching.path.clear(); // = new IntPair[path.Count];
//    for (list<IntPair>::iterator it = path.begin(); it != path.end(); it++)
//        matching.path.push_back(IntPair(it->X, it->Y));
//
//
//    return matching;
//}


 
 /* Reduce el emparejamiento entre 2 series s[sIni..sEnd] y t[tIni...tEnd] para que un punto de s no se empareje
        con varios puntos de t, atendiendo al criterio de mínima distancia */
//void MinimizeMatching(DTWMatching *matching, vector< vector <vec3> > s, vector< vector <vec3> > t) {
//
//    vector<IntPair> path; // = new list<IntPair>();
//    int bestPairing;
//    double bestDist, aux;
//
//
//    bestPairing = 0;
//    bestDist = Dist(s[matching->path[0].X], t[matching->path[0].Y]);
//    for (int i = 0; i < matching->path.size() - 1; i++) {
//
//        if (matching->path[i].X == matching->path[i + 1].X) {
//
//            aux = Dist(s[matching->path[i + 1].X], t[matching->path[i + 1].Y]);
//            if (aux <= bestDist) {
//                bestDist = aux;
//                bestPairing = i + 1;
//            }
//
//        } else {
//            path.push_back(IntPair(matching->path[bestPairing].X, matching->path[bestPairing].Y));
//            bestDist = Dist(s[matching->path[i + 1].X], t[matching->path[i + 1].Y]);
//            bestPairing = i + 1;
//        }
//    }
//
//    if (matching->path[matching->path.size() - 2].X != matching->path[matching->path.size() - 1].X) {
//        path.push_back(IntPair(matching->path[bestPairing].X, matching->path[bestPairing].Y));
//    }
//
//    // Copiamos el path
//    matching->path.clear();
//    //        matching->path.resize(path.size(), new IntPair(0,0));
//    aux = 0;
//    for (int i = 0; i < path.size(); i++) {
//        matching->path.push_back(IntPair(path[i].X, path[i].Y));
//        aux += Dist(s[matching->path[i].X], t[matching->path[i].Y]);
//    }
//    matching->value = aux;
//}

/* DTW clásico, que empareja una serie {s[sIni]...s[sEnd]} con otra {t[tIni]...t[tEnd]}
        Para medir distancias, se utiliza la función Dist entre 2 double */
 void DTWMatching::DTWClassic( double  ***s, int sIni, int sEnd, double ***t, int tIni, int tEnd) {

//    DTWMatching matching; // = new DTWMatching();
    //        distance = Dist;
    int sLength = (sEnd - sIni + 1);
    int tLength = (tEnd - tIni + 1);
    double min;

    if ((cost.size() < sLength) || (cost[0].size() < tLength)) {        
        //cost = new vector(sLength, vector(tLength));
        cost.resize(sLength, vector<double>(tLength, 0));
        //matrix = new vector(sLength, vector(tLength));
        matrix.resize(sLength, vector<double>(tLength, 0));
    }
 
    // Calculo de costes iniciales e inicializacion
    for (int i = sIni; i <= sEnd; i++)
        for (int j = tIni; j <= tEnd; j++)
            cost[i - sIni][j - tIni] = Dist(s[i], t[j]);            
          
    // Aplicamos DTW
    for (int i = 0; i < sLength; i++) { //matrix.size()

        for (int j = 0; j < tLength; j++) { //matrix[0].size()

            min = DBL_MAX;
            if (i > 0)
                min = matrix[i - 1][j];
            if (j > 0 && matrix[i][j - 1] < min)
                min = matrix[i][j - 1];
            if (i > 0 && j > 0 && matrix[i - 1][j - 1] < min)
                min = matrix[i - 1][j - 1];
            if (i == 0 && j == 0)
                min = 0;

            matrix[i][j] = cost[i][j] + min;
        }
    }
    value = matrix[sLength - 1][tLength - 1];//matrix.size() matrix[0].size()
 
    // Encontramos el emparejamiento
    list<IntPair> path; // = new list<IntPair>();
    int idxi, idxj, restai, restaj;

    idxi = matrix.size() - 1;
    idxj = matrix[0].size() - 1;
    path.push_front(IntPair(idxi, idxj));
    while (idxi != 0 || idxj != 0) {

        if (idxi == 0)
            idxj--;
        else if (idxj == 0)
            idxi--;
        else {

            min = matrix[idxi - 1][idxj - 1];
            restai = 1;
            restaj = 1;

            if (min >= matrix[idxi - 1][idxj]) {
                min = matrix[idxi - 1][idxj];
                restai = 1;
                restaj = 0;
            }

            if (min >= matrix[idxi][idxj - 1]) {
                min = matrix[idxi][idxj - 1];
                restai = 0;
                restaj = 1;
            }

            idxi -= restai;
            idxj -= restaj;
        }

        path.push_front(IntPair(idxi, idxj));
    }

    // Copiamos el path
    path.clear(); // = new IntPair[path.size()];      
    for (list<IntPair>::iterator it = path.begin(); it != path.end(); it++)
       path.push_back(IntPair(it->X, it->Y));


//    return matching;
}

/* DTW modificado. que empareja una serie {s[sIni]...s[sEnd]} con una subsecuencia de {t[tIni]...t[tEnd]}
        Para medir distancias, se utiliza la función Dist entre 2 double */
// DTWMatching DTWSubsequence(vector< vector <vec3> > s, int sIni, int sEnd, vector< vector <vec3> > t, int tIni, int tEnd) {
//
//    DTWMatching matching; // = new DTWMatching();
//    //        distance = Dist;
//    int sLength = (sEnd - sIni + 1);
//    int tLength = (tEnd - tIni + 1);
//    double min;
//
//
//
//    if ((cost.size() < sLength) || (cost[0].size() < tLength)) {
//
//        //cost = new vector(sLength, vector(tLength));
//        cost.resize(sLength, vector<double>(tLength, 0));
//        //matrix = new vector(sLength, vector(tLength));
//        matrix.resize(sLength, vector<double>(tLength, 0));
//
//    }
//
//    // Calculo de costes iniciales e inicializacion
//    for (int i = sIni; i <= sEnd; i++)
//        for (int j = tIni; j <= tEnd; j++)
//            cost[i - sIni][j - tIni] = Dist(s[i], t[j]);
//
//
//
//    // Aplicamos DTW
//    for (int i = 0; i < matrix.size(); i++) {
//
//        for (int j = 0; j < matrix[0].size(); j++) {
//
//            min = DBL_MAX;
//            if (i > 0)
//                min = matrix[i - 1][j];
//            if (j > 0 && matrix[i][j - 1] < min)
//                min = matrix[i][j - 1];
//            if (i > 0 && j > 0 && matrix[i - 1][j - 1] < min)
//                min = matrix[i - 1][j - 1];
//            if (i == 0 && j == 0)
//                min = 0;
//
//            if (i > 0)
//                matrix[i][j] = cost[i][j] + min;
//            else
//                matrix[i][j] = cost[i][j];
//        }
//    }
//
//
//    // Encontramos el emparejamiento
//    int idxi, idxj, restai, restaj;
//    list<IntPair> path; // = new list<IntPair>();
//
//    idxi = matrix.size() - 1;
//    idxj = 0;
//    for (int i = 0; i < matrix[0].size(); i++)
//        if (matrix[idxi][i] <= matrix[idxi][idxj])
//            idxj = i;
//    matching.value = matrix[idxi][idxj];
//    path.push_front(IntPair(idxi, idxj));
//    while (idxi != 0) {
//
//        if (idxj == 0)
//            idxi--;
//        else {
//
//            min = matrix[idxi - 1][idxj - 1];
//            restai = 1;
//            restaj = 1;
//
//            if (min >= matrix[idxi - 1][idxj]) {
//                min = matrix[idxi - 1][idxj];
//                restai = 1;
//                restaj = 0;
//            }
//
//            if (min >= matrix[idxi][idxj - 1]) {
//                min = matrix[idxi][idxj - 1];
//                restai = 0;
//                restaj = 1;
//            }
//
//            idxi -= restai;
//            idxj -= restaj;
//        }
//
//        path.push_front(IntPair(idxi, idxj));
//    }
//
//
//    // Copiamos el path
//    matching.path.clear(); // = new IntPair[path.Count];
//    for (list<IntPair>::iterator it = path.begin(); it != path.end(); it++)
//        matching.path.push_back(IntPair(it->X, it->Y));
//
//
//    return matching;
//}
