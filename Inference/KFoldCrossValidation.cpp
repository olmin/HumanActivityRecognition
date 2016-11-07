/* 
 * File:   KFoldCrossValidation.cpp
 * Author: olmin
 * 
 * Created on 9 de mayo de 2013, 12:56
 */

#include "KFoldCrossValidation.h"

vector<vector< vector<BehaviourSequence> > >  getKFoldsVector(Connection *con, list<BehaviourSequence> *validationData){
    
    vector<vector< vector<BehaviourSequence> > > kFoldsVector(Num_Actions, 
                                                        vector< vector<BehaviourSequence> >( K_Fold));
            
    ResultSet *resBehaviours;
    PreparedStatement *prep_stmt;

    for(int idAction=0;idAction<Num_Actions;idAction++){        

        vector<BehaviourSequence> result;
       
        prep_stmt = con -> prepareStatement("SELECT b.id, b.user, bc.sequence FROM `Behaviour` b, `BehaviourClustered` bc "
                "WHERE `compression_rate`=? AND `num_clusters`=? AND b.id=bc.id AND  b.name=? ");
        //begin get the resultset
        prep_stmt->setInt(1, compressionRate);
        prep_stmt->setInt(2, numClusters);
        prep_stmt->setString(3, listActions[idAction]);

        resBehaviours = prep_stmt -> executeQuery();

        /*For each action we get the ids of the behaviours*/
        while (resBehaviours->next()) {
            BehaviourSequence beh;
            beh.idAction=idAction;
            beh.idBehaviour=resBehaviours->getInt("id");
            beh.idUser=resBehaviours->getInt("user");
            beh.compressionRate=compressionRate;
    //        beh.numClusters=numClusters;
            beh.sequence=getSequence(resBehaviours->getString("sequence"));
            addSkeletonsFromIdBehaviour(con,beh.idBehaviour, &(beh.skeletons));        

            setIdCluster2Skeletons(&beh);
            //we add it to the result list
            result.push_back(beh);       
        }   
        
        //split train and validation       
        float trainRate=result.size()*((float)TrainingRate/100);
        int validRate=result.size()-abs(trainRate);      
        for(int j=0;j<validRate;j++){
            int k = rand()%result.size();
            vector<BehaviourSequence>::iterator it=result.begin();
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
		BehaviourSequence temp = result[n];	
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

list<BehaviourSequence> setTrainingAndTestKFold(vector< vector<BehaviourSequence> >  data, int kFold, 
        list<BehaviourSequence> *dataTrainingGlobal, list<BehaviourSequence> *dataTestGlobal){
    
    
//    BehaviourSequence first= dataAction->front();
//    first.compressionRate=2;
    
    
    list<BehaviourSequence> trainingData;
    
       
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

