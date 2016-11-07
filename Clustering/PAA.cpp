/* 
 * File:   PAA.cpp
 * Author: olmin
 * 
 * Created on 4 de noviembre de 2012, 20:49
 */

#include "PAA.h"


using namespace std;
using namespace sql;


//SimpleSkeleton resumeSkeleton(SimpleSkeleton skelVector[],int count){
//    
//    SimpleSkeleton finalSkel;
//    
//    finalSkel.idBehaviour=skelVector[0].idBehaviour;    
//    
//    
//    float height =0;
//    for(int i=0;i< count;i++){
//        height+=skelVector[i].height;
//    }
//    finalSkel.height= height/count;
//    finalSkel.frame= skelVector[0].frame;    
//    
//    float angle;
//    for(int i=0;i<Num_Angles;i++){
//        angle=0;
//        for(int j=0;j<count;j++){
//            angle+=skelVector[j].angles[i];            
//        }
//        angle=angle/count;
//        finalSkel.angles[i]=angle;        
//    }
// 
//    return finalSkel;
//}

SimpleSkeleton getPAASkeletons(vector<SimpleSkeleton> vectorSkel){
    SimpleSkeleton finalSkel;
    
    finalSkel.idBehaviour=vectorSkel[0].idBehaviour;    
    
    
    float height =0;
    for(int i=0;i< vectorSkel.size();i++){
        height+=vectorSkel[i].height;
    }
    finalSkel.height= height/vectorSkel.size();
    finalSkel.frame= vectorSkel[0].frame;    
    
    float angle;
    for(int i=0;i<Num_Angles;i++){
        angle=0;
        for(int j=0;j<vectorSkel.size();j++){
            angle+=vectorSkel[j].angles[i];            
        }       
        finalSkel.angles[i]=angle/vectorSkel.size();        
    } 
    
    return finalSkel;
//    int index=0;
//    list<SimpleSkeleton> paaList;
//    list<SimpleSkeleton>::iterator it;
//    
//    SimpleSkeleton skelVector[paaRatio];    
//    
//    for (it = listSkel.begin(); it != listSkel.end(); it++) {
//        skelVector[index++] = *it;
//        if (index == paaRatio) {
//            paaList.push_back(resumeSkeleton(skelVector, paaRatio));
//            index = 0;
//        }
//    }//end for list
//    if (index != 0) {//the vector is not complete so we save it
//        paaList.push_back(resumeSkeleton(skelVector, index));
//    }    
//    return paaList;
}


void executePAA(Connection *con,int idBehaviour, SimpleSkeleton skelVector[],int count) {

    PreparedStatement *prep_stmt;
    prep_stmt = con -> prepareStatement(
            "INSERT INTO `PAASkeleton` (`id_behaviour`, `frame`, `compression_rate`, `height`, `ang0`, `ang1`, `ang2`, `ang3`, `ang4`, `ang5`, `ang6`, `ang7`, `ang8`, `ang9`, `ang10`) "
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"
            );

    float height = 0;
    for (int i = 0; i < count; i++) {
        height += skelVector[i].height;
    }
    height = height / count;

    prep_stmt -> setInt(1, idBehaviour);
    prep_stmt -> setInt(2, skelVector[0].frame);
    prep_stmt -> setInt(3, compressionRate);
    prep_stmt -> setDouble(4,height);
//    prep_stmt -> setDouble(4, 0.0);

    float angle;
    for (int i = 0; i < Num_Angles; i++) {
        angle = 0;
        for (int j = 0; j < count; j++) {
            angle += skelVector[j].angles[i];
        }
        angle = angle / count;
        prep_stmt -> setDouble(i + 5, angle);
    }

    prep_stmt -> executeUpdate();
    delete prep_stmt;

}


bool PAAnotExists(Connection *con){ 
     PreparedStatement *prep_stmt;
     ResultSet *resSet;
     prep_stmt= con->prepareStatement("SELECT * FROM `PAASkeleton` WHERE `compression_rate`=?");
     prep_stmt -> setInt(1, compressionRate);
     
     resSet=prep_stmt->executeQuery();
    
     bool result=resSet->rowsCount()>0;
     
     delete prep_stmt;
     delete resSet;
     return result;
}


void startPAA(Connection *con) {
    Statement *stmt;
    ResultSet *resBehaviours, *resSkeletons;
    PreparedStatement *prep_stmt;
  
    if (PAAnotExists(con))
        return;
  
    cout << "Executing PAA with compression rate: "<< compressionRate << endl;
      
    /* create a statement object */
    stmt = con -> createStatement();

    /* run a query which returns exactly one result set */
    resBehaviours = stmt -> executeQuery("SELECT * FROM Behaviour");

    /* fetch the data : retrieve all the rows in the result set */
    //vars
    int idBehaviour, idUser;
    double userHeight;
    int numFrames;
    SimpleSkeleton skelVector[compressionRate];
    int index = 0;

    while (resBehaviours->next()) {
        idBehaviour = resBehaviours -> getInt("id");
        numFrames = resBehaviours -> getInt("frames");
        idUser = resBehaviours -> getInt("user");
        userHeight = resBehaviours -> getDouble("user_height");
        
        prep_stmt = con->prepareStatement("SELECT * FROM Skeleton WHERE id_behaviour=?");
        prep_stmt -> setInt(1, idBehaviour);
        resSkeletons = prep_stmt -> executeQuery();
        int countFrame=1;
        index = 0;
        while (resSkeletons->next()) {
            skelVector[index] = fillSkeletonFromDB(idBehaviour,resSkeletons, userHeight);
            skelVector[index++].frame =countFrame++;           
//            skelVector[index++] = fillSkeletonFromDB(idBehaviour,resSkeletons);
            if (index == compressionRate) {
                executePAA(con, idBehaviour, skelVector, compressionRate);
                index = 0;
            }
        }//end while Skeletons    
        if (index != 0) {//the vector is not complete so we save it
            executePAA(con, idBehaviour, skelVector, index);
        }
        prep_stmt->clearParameters();
        prep_stmt->close();
    } // while Behaviours

    /* Clean up */
    delete resBehaviours;
    delete resSkeletons;
    delete stmt;
    delete prep_stmt;
	
} // main()
