/* 
 * File:   Automata.cpp
 * Author: olmin
 * 
 * Created on 15 de noviembre de 2012, 11:52
 */

#include "Automata.h"
#include <stdlib.h>
#include <ext/hash_map>

using namespace std;

void Automata::createDotFile(string fileName) {
	stringstream str_ac, str_fileName ;        

        vector<int> normalStates(numStates,0);
        vector<int>::iterator it;
        vector<int>::iterator it2;
        vector<int>::iterator sti;    
        
        bool header_written=false;

        // head
        str_ac << "digraph finite_automaton {\n"
                "\tgraph[fontsize=8]\n"
                "\trankdir=LR;\n"
                "\tsize=8;\n\n";
     
        int count=1;
        for(it2 = normalStates.begin(); it2 != normalStates.end(); it2++){                       
              *it2=count++;                    
        }
        
        // final states
        for(it = finalStates.begin(); it != finalStates.end(); it++) {          
            if(!header_written) {
                    str_ac << "\tnode [shape=doublecircle, style=\"\", color=black];";
                    header_written = true;
            }
            str_ac << " q" << (*it);
            
            for(it2 = normalStates.begin(); it2 != normalStates.end(); it2++){                       
                if(*it == *it2){                
                  normalStates.erase(it2);                         
                  break;
                }
            }
        }
        
        if(header_written)
                str_ac << ";\n";

        // normal states
        if(finalStates.size() < numStates) {
                str_ac << "\tnode [shape=circle, style=\"\", color=black];";
                for(it2 = normalStates.begin(); it2 != normalStates.end(); ++it2)                       
                        str_ac << " q" << (*it2);
                str_ac << ";\n";
        }

        
        // non-visible states for arrows to initial states                 
        header_written = false;   
        for(sti = initStates.begin(); sti != initStates.end(); sti++) {
                if(!header_written) {
                        str_ac << "\tnode [shape=plaintext, label=\"\", style=\"\"];";
                        header_written = true;
                }
                str_ac << " iq" << *sti;
        }
        if(header_written)
                str_ac << ";\n";

        // and arrows to mark initial states
        for(sti = initStates.begin(); sti != initStates.end(); ++sti)
                str_ac << "\tiq" << *sti << " -> q" << *sti << " [color=blue];\n";


    for (int row = 0; row < transitionMatrix.size(); row++) {
        for (int col = 0; col < transitionMatrix[row].size(); col++) {

            if(!transitionMatrix[row][col]==0)
                str_ac << "\tq" << row+1 << " -> q" << transitionMatrix[row][col] << " [label=\"" << getIndexFromAlphabet(col) << "\"];\n";

        }
    }

        str_ac << "};\n";


      str_fileName << fileName << ".dot";         
      ofstream myfile(str_fileName.str().c_str());
      if (myfile.is_open()) {
        myfile << str_ac.str(); 
        myfile.close();
      }else{
          cout << "Unable to open file"; 
      }
      /**End added**/
      
    //  return str_ac.str();
}

int  Automata::getIndexFromAlphabet(int column){
    
    for(hash_map<int,int>::iterator it= alphabet.begin(); it!= alphabet.end();it++){
        if(it->second==column)
            return it->first;            
    }    
}

void Automata::startAutomata(){
        actualState=initStates[0];
}

bool Automata::isSequenceValid(vector<int> sequence){
    
    startAutomata();
    
    bool foundStep;
    
    for(int i=0; i< sequence.size(); i++){        
        foundStep=false;
        
                
        for(int col=0; col< alphabet.size(); col++){            
          
            if(alphabet.find(sequence[i])!= alphabet.end() && transitionMatrix[actualState-1][alphabet[sequence[i]]]!=0){
                foundStep=true;
                actualState=transitionMatrix[actualState-1][alphabet[sequence[i]]];
                break;
            }            
        }
        if(!foundStep)
            return false;
    }
        
    return isFinalState();
}

bool Automata::isFinalState(){
    for(int i=0;i< finalStates.size();i++){
        if(finalStates[i]==actualState)
            return true;
    }
    return false;
}

int compareAutomatas(vector<Automata> automs, vector<int> sequence){
    int selAction=-1;
    for(int i=0;i<automs.size();i++){
        if(automs[i].isSequenceValid(sequence))
//            if(selAction==NULL)
                return i;
//            else
//                selAction=NULL;
    }
    
    return selAction;
}