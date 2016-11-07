/* 
 * File:   Skeleton.cpp
 * Author: olmin
 * 
 * Created on 29 de noviembre de 2013, 12:49
 */

#include "Skeleton.h"
#include "../Clustering/config.h"
//#include "../Clustering/SimpleSkeleton.h"



//vec3 string2Point3D(string stringPoint){
//    vec3 point;
//    
//    stringstream stream(stringstream::in | stringstream::out);
//    stream<<stringPoint;
//    
//    stream>> point.x;    
//    stream>> point.y ;   
//    stream>> point.z ;
//    
//    return point;
//}

double* string2Vector3D(string stringPoint){
    double *point=new double[3];
    
    stringstream stream(stringstream::in | stringstream::out);
    stream<<stringPoint;
    
    stream>> point[0];    
    stream>> point[1] ;   
    stream>> point[2] ;
    
    return point;
}

void addSkeletons(Connection *con, SkeletonSequence *skelSeq){
    ResultSet *resSkeletons;
    PreparedStatement *prep_stmt;
    
    /* create a statement object */
    prep_stmt = con -> prepareStatement("SELECT * FROM Skeleton WHERE id_behaviour=?");
    prep_stmt->setInt(1, skelSeq->idBehaviour);   

    resSkeletons = prep_stmt->executeQuery();

    while (resSkeletons->next()) {
        Skeleton skel;
        skel.idBehaviour = resSkeletons->getInt(1);       
        skel.frame=resSkeletons->getInt(2);
        skel.joints= new double*[15];
        for (int i = 0; i < 15; i++) {
            skel.joints[i]=new double[3];
            skel.joints[i]= string2Vector3D(resSkeletons->getString(i + 3));        
        }   
        skelSeq->sequence.push_back(skel);
    }
    delete resSkeletons;
    delete prep_stmt;    
 }

vector<vector< vector<SkeletonSequence> > >  getKFoldsVector(Connection *con,list<SkeletonSequence> *validationData){
    
    vector<vector< vector<SkeletonSequence> > > kFoldsVector(Num_Actions, 
                                                        vector< vector<SkeletonSequence> >( K_Fold));
            
    ResultSet *resBehaviours;
    PreparedStatement *prep_stmt;

    for(int idAction=0;idAction<Num_Actions;idAction++){        

        vector<SkeletonSequence> result;
       
        prep_stmt = con -> prepareStatement("SELECT id, user FROM `Behaviour` "
                "WHERE name=? ");
        //begin get the resultset
//        prep_stmt->setInt(1, compressionRate);
//        prep_stmt->setInt(2, numClusters);
        prep_stmt->setString(1, listActions[idAction]);

        resBehaviours = prep_stmt -> executeQuery();

        /*For each action we get the ids of the behaviours*/
        while (resBehaviours->next()) {
            SkeletonSequence beh;
            beh.idAction=idAction;
            beh.idBehaviour=resBehaviours->getInt("id");
            beh.idUser=resBehaviours->getInt("user");          
                       
            addSkeletons(con, &beh);        
          
            result.push_back(beh);       
        }   
        
        //split train and validation           
        float trainRate=result.size()*((float)TrainingRate/100);
        int validRate=result.size()- (int)(trainRate);      
        for(int j=0;j<validRate;j++){
            int k = rand()%result.size();
            vector<SkeletonSequence>::iterator it=result.begin();
            it+=k;           
            validationData->push_back(result[k]);
            result.erase(it);
        }       
        
        //shuffle array
        int n = result.size();
	while (n > 1) {
		// 0 <= k < n.
		int k = rand()%n;		
		
		// n is now the last pertinent index;
		n--;		
		// swap array[n] with array[k]
		SkeletonSequence temp = result[n];	
		result[n] = result[k];
		result[k] = temp;
	}
        int k=0;
         for(int i=0;i<result.size();i++){             
             kFoldsVector[idAction][k%K_Fold].push_back(result[i]);
             k++;
         }
   
        resBehaviours->close();
        prep_stmt->clearParameters();
    }
    delete resBehaviours;
    delete prep_stmt;
    
    return kFoldsVector;
}
 

list<SkeletonSequence> setTrainingAndTestKFold(vector< vector<SkeletonSequence> >  data, int kFold, 
        list<SkeletonSequence> *dataTrainingGlobal, list<SkeletonSequence> *dataTestGlobal){
    
    
//    BehaviourSequence first= dataAction->front();
//    first.compressionRate=2;
    
    
    list<SkeletonSequence> trainingData;
    
       
    for (int i = 0; i < K_Fold; i++){
        if(i==kFold){
            for (int j = 0; j < data[i].size(); j++)
                dataTestGlobal->push_back(data[i][j]);
        }else{
            for (int j = 0; j < data[i].size(); j++){
                dataTrainingGlobal->push_back(data[i][j]);
                trainingData.push_back(data[i][j]);
            }
        }       
    }   
       return trainingData;
}

  string getIdsTraining(list<SkeletonSequence> *dataTraining){
     std::ostringstream s1;
 
     list<SkeletonSequence>::iterator it;
    for(it=dataTraining->begin();it!=dataTraining->end();it++)
        s1<<it->idBehaviour <<",";
     
    string idsTraining= s1.str();
    
    return idsTraining.substr(0, idsTraining.size()-1);
 }