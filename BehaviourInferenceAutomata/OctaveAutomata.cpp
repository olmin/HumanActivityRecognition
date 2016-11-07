/* 
 * File:   OctaveAutomata.cpp
 * Author: olmin
 * 
 * Created on 15 de noviembre de 2012, 12:44
 */

#include "OctaveAutomata.h"



using namespace std;
using namespace __gnu_cxx;

OctaveAutomata::OctaveAutomata(vector<vector<int> > inData, int k){
    string data;
    char buffer [33];

    for(int i=0;i< inData.size();i++){
        for(int j=0;j< inData[i].size();j++){            
            sprintf(buffer, "%d", inData[i][j]); 
            data.append(buffer);
            if(j!= inData[i].size()-1)
                data.append(" ");
            else
                data.append("\n");
        }
    }
      
   initAutomata(data,k); 
}
 
void OctaveAutomata::initAutomata(string inData,int k){

    if(!octave_initialized){
         const char * argvv [] = {"" /* name of program, not relevant */, "--silent"};

        octave_main(2, (char **) argvv, true /* embedded */);
    }else{        
        cout<<"Octave already initialized"<<endl;
    }
    octave_value_list functionArguments;
    functionArguments(0) = k;
    functionArguments(1) = inData;
    
    const octave_value_list result = feval("K2dfa", functionArguments, 1);

    Octave_map a(result(0).map_value());

    finiteStates = a.contents(a.seek("FiniteSetOfStates"))(0).array_value();
    alphabets = a.contents(a.seek("Alphabets"))(0).list_value();
    transitionMatrix = a.contents(a.seek("TransitionMatrix"))(0).matrix_value();
    initState = a.contents(a.seek("InitialState"))(0).array_value();
    acceptedStates = a.contents(a.seek("FinalAcceptStates"))(0).array_value();

//cout<< "Alphabet= ";
//    for(int i=0; i< alphabets.length();i++){        
//        cout << alphabets(i).string_value()<< " ";
//    }
//   
//   cout << "\n FiniteSetOfStates = "<< finiteStates << endl <<
//
////           "  Alphabets = " << alphabets << endl <<
//           "  TransitionMatrix = " << transitionMatrix << endl <<
//           "  InitialState =  " << initState << endl <<
//           " FinalAcceptStates = " << acceptedStates << endl;
    
       do_octave_atexit();
}

OctaveAutomata::OctaveAutomata(string inData,int k) {

    initAutomata(inData,k);    
    
}

hash_map <int, int> OctaveAutomata::getAlphabet(){
    
    hash_map <int, int> alph (alphabets.length());
    
    for(int i=0; i< alphabets.length();i++){        
       alph[atoi(alphabets(i).string_value().c_str())]=i;
    }
    return alph;
}

int OctaveAutomata::getNumStates() {
    return finiteStates.length();
}

 vector<vector<int> >  OctaveAutomata::getTransitionMatrix() {
    
    vector<vector<int> > transMat(transitionMatrix.rows(), vector<int>(transitionMatrix.columns(),0));
        
    for(int i=0; i< transitionMatrix.rows();i++){        
         for(int j=0; j< transitionMatrix.columns();j++){  
             transMat[i][j]= transitionMatrix.elem(i,j);//.int_value();
         }      
    }
    return transMat;
}

vector<int> OctaveAutomata::getInitStates() {
    
    vector<int> inSt (initState.length());
    
    for(int i=0; i< initState.length();i++){        
       inSt[i]= initState(i);//.double_value();
    }
    return inSt;

}

vector<int> OctaveAutomata::getFinalStates() {
    
     vector<int> finSt (acceptedStates.length());
    
    for(int i=0; i< acceptedStates.length();i++){        
       finSt[i]= acceptedStates(i);//.double_value();
    }
    return finSt;

}
