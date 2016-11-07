
#include <libio.h>

#include "config.h"
#include "SimpleSkeleton.h"
#include "BehaviourSequence.h"
#include "Commons.h"

using namespace std;
using namespace sql;
static bool firstSQL=true;
void connectSQL(){
    if(firstSQL){        
         driver = get_driver_instance();
         firstSQL=false;
    }
    /* create a database connection using the Driver */
//        if(con==NULL || con -> isClosed())
        con = driver -> connect(DBHOST, USER, PASSWORD);

        /* alternate syntax using auto_ptr to create the db connection */
        //auto_ptr  con (driver -> connect(url, user, password));

        /* turn off the autocommit */
        con -> setAutoCommit(0);

        /* select appropriate database schema */
        con -> setSchema(DATABASE);
        savept = con -> setSavepoint("SAVEPT1");
    
}

void disconnectSQL(){
       con -> releaseSavepoint(savept);
//       cout << "\tCommitting outstanding updates to the database .." << endl;
       con -> commit();

       /* Clean up */
       con -> close();       
}

list<int> getListIdBehaviours(Connection *con) {

    list<int> listIds;

    ResultSet *resBehaviours;
    PreparedStatement *prep_stmt;

    /* create a statement object */
    prep_stmt = con -> prepareStatement("SELECT id FROM Behaviour");
    resBehaviours = prep_stmt -> executeQuery();

    /*For each action we get the ids of the behaviours*/
    while (resBehaviours->next()) {
        listIds.push_back(resBehaviours->getInt("id"));
    }

    delete resBehaviours;
    delete prep_stmt;

    return listIds;
}

int getRowsActionType(Connection *con, int idAction){
    
    ResultSet *resLimits;
    PreparedStatement *prep_stmtLimits;
      
    prep_stmtLimits = con -> prepareStatement("SELECT ROUND(COUNT(*)) FROM Behaviour WHERE name=?");
    prep_stmtLimits->setString(1, listActions[idAction]);

    resLimits = prep_stmtLimits -> executeQuery();
    int count;
    while (resLimits->next()) {
        count = resLimits->getInt(1);
    }
    delete resLimits;
    delete prep_stmtLimits;
    
    return count;
}

int getRate(int total, int rate){   
    return  total* rate/100;   
}

/*! Returns the kind of action from the listActions. E.g. 0 if walk; 1 if sit down, etc.*/
int getTypeAction(Connection *con, int idAction){
          
    ResultSet *resBehaviours;
    PreparedStatement *prep_stmt;
   /* create a statement object */
    prep_stmt = con -> prepareStatement("SELECT name FROM Behaviour WHERE id=?");
    //begin get the resultset
    prep_stmt->setInt(1, idAction);      
    resBehaviours = prep_stmt -> executeQuery();   

    string name;
    while (resBehaviours->next()) {
        name=resBehaviours->getString("name");
    }    
    int selAction=-1;
    for(int action=0;action<Num_Actions;action++){
        if(name==listActions[action])
            selAction=action;
    }
     delete resBehaviours;  
    delete prep_stmt;  
    
    return selAction;
}

vector<int> getListIdBehFromAction(Connection *con, int idAction, int type, list<int> *otherListIds ) {
    vector<int> vectorIds;

    ResultSet *resBehaviours;
    PreparedStatement *prep_stmt;
    
    int count= getRowsActionType(con,idAction);
    
    /* create a statement object */
    prep_stmt = con -> prepareStatement("SELECT id FROM Behaviour WHERE name=? LIMIT ?,?");
    //begin get the resultset
    prep_stmt->setString(1, listActions[idAction]);
   
   
    resBehaviours = prep_stmt -> executeQuery();

    /*For each action we get the ids of the behaviours*/
    while (resBehaviours->next()) {
        vectorIds.push_back(resBehaviours->getInt("id"));
    }
    int trainingRate=getRate(count,TrainingRate);    
     int validationRate=getRate(count,ValidationRate);
     switch (type) {
        case AllData:
            prep_stmt->setInt(2, 0);
            prep_stmt->setInt(3, count);
            break;
        case TestData:             
            prep_stmt->setInt(2, 0);
            prep_stmt->setInt(3, trainingRate);
            break;
        case TrainingData:             
            prep_stmt->setInt(2, 0);
            prep_stmt->setInt(3, trainingRate);
            break;
        case ValidationData:            
            prep_stmt->setInt(2, count-validationRate);
            prep_stmt->setInt(3, count);
            break;
        default:
            break;
    }     
     
    vector<int> id2sel;
    switch (type) {
        case AllData:           
            break;
        case TestData:            
            id2sel.resize(count* TestRate/100, 0);            
            break;
        case TrainingData:            
            id2sel.resize(count* TrainingRate/100, 0);
            break;
        case ValidationData:           
            break;
        default:
            break;
    }

    delete resBehaviours;  
    delete prep_stmt;
    
    
    if(type==TestData || type==TrainingData){
         srand(time(NULL));
         for(int i=0;i<id2sel.size();i++)
             id2sel[i]=rand()%vectorIds.size();
         vector<int> finalList;
         
         for(int i=0;i<id2sel.size();i++){
             int id= vectorIds[id2sel[i]];
             vectorIds[id2sel[i]]=-1;
             finalList.push_back(id);            
         }
         
         for(int i=0;i<vectorIds.size();i++){
             if(vectorIds[i]!=-1)
                 otherListIds->push_back(vectorIds[i]);
         }
         return finalList;         
    }
        
      return vectorIds;

}

void addSkeletonsFromIdBehaviour(Connection *con, int idBehaviour, list<SimpleSkeleton> *listSkel) {
    ResultSet *resSkeletons;
    PreparedStatement *prep_stmt;

    /* create a statement object */
    prep_stmt = con -> prepareStatement("SELECT * FROM PAASkeleton WHERE id_behaviour=? AND compression_rate=?");
//    prep_stmt = con -> prepareStatement("SELECT * FROM PAASkeleton s, PAAHandObjectsData o "
//            "WHERE id_behaviour =? AND id_behaviour = activity_id AND o.compression_rate =? "
//            "AND o.compression_rate = s.compression_rate AND o.frame = s.frame");
//            
            prep_stmt->setInt(1, idBehaviour);
    prep_stmt->setInt(2, compressionRate);

    resSkeletons = prep_stmt->executeQuery();

    while (resSkeletons->next()) {
        SimpleSkeleton skel;
        skel.idBehaviour = idBehaviour;
        skel.height = resSkeletons->getDouble("height");
        for (int i = 0; i < Num_Angles; i++) {
            skel.angles[i] = resSkeletons->getDouble(listAngles[i]);
        }
        skel.normalizeSkel();
        listSkel->push_back(skel);
    }
//     while (resSkeletons->next()) {
//        SimpleSkeleton skel;
//        skel.idBehaviour = idBehaviour;
//        skel.height = 0;//resSkeletons->getDouble("height");
////        for (int i = 0; i < Num_Angles; i++) {
////            if(i==1 || i==2 || i==5 || i==6)        
//        skel.angles[0] = (resSkeletons->getDouble("ang1") - Min_Angle) / (Max_Angle - Min_Angle);
//        skel.angles[1] = (resSkeletons->getDouble("ang2") - Min_Angle) / (Max_Angle - Min_Angle);
//        skel.angles[2] = (resSkeletons->getDouble("ang5") - Min_Angle) / (Max_Angle - Min_Angle);
//        skel.angles[3] = (resSkeletons->getDouble("ang6") - Min_Angle) / (Max_Angle - Min_Angle);
//         
//        //objects
//       for(int i=0;i<Max_Objects_Per_Action;i++){
//            Object obj;
//            obj.id= i;
//            obj.distance2hand=resSkeletons->getDouble(i+19);                        
//            if(obj.distance2hand!=-1){
//                obj.distance2hand=(Max_Object_Dist-obj.distance2hand)/Max_Object_Dist; 
//                obj.object_type= getObjectType(idBehaviour, i+1);
//                skel.angles[3+i]=obj.distance2hand;
//                if(obj.object_type!=0 && obj.object_type!=1 && obj.object_type!=5)
//                    skel.angles[3+Max_Objects_Per_Action+obj.object_type]=obj.distance2hand;
//            }
//        }
//        }
//        skel.normalizeSkel();
//        listSkel->push_back(skel);
//    }
    delete resSkeletons;
    delete prep_stmt;
}

/*!Gets the skeleton data of the PAASkeleton table in DB. type 0=ALL, 1=Training, 2=Test, 3=Validation*/
list<SimpleSkeleton> getDataSkeletons(Connection *con) {

    list<SimpleSkeleton> listSkel;

    list<int> listIdBehaviours = getListIdBehaviours(con);
    list<int>::iterator it;

    for (it = listIdBehaviours.begin(); it != listIdBehaviours.end(); it++)
        addSkeletonsFromIdBehaviour(con, *it, &listSkel);

    return listSkel;
}

/*!Gets the skeleton data of the PAASkeleton table in DB from selected action. type= ALL, Training, Test, Validation*/
list<SimpleSkeleton> getDataSkeletonsFromAction(Connection *con, int idAction, int type) {
    list<SimpleSkeleton> listSkel;

    vector<int> listIdBehaviours = getListIdBehFromAction(con, idAction, type, NULL);
    vector<int>::iterator it;

    for (it = listIdBehaviours.begin(); it != listIdBehaviours.end(); it++)
        addSkeletonsFromIdBehaviour(con, *it, &listSkel);

    return listSkel;
}

void assignSkeletons2Clusters(SimpleSkeleton* clusters, list<SimpleSkeleton> *dataSkeletons) {

    list<SimpleSkeleton>::iterator it;
    float minDistance;
    int idCluster;

    for (it = dataSkeletons->begin(); it != dataSkeletons->end(); it++) {
        minDistance = FLT_MAX; //init
        for (int i = 0; i < numClusters; i++) {
            float distance = clusters[i].distance(*it);
            if (distance < minDistance) {
                minDistance = distance;
                idCluster = clusters[i].idCluster;
            }
        }
        it->idCluster = idCluster;
    }
}
void assignSkeletons2Clusters(vector<SimpleSkeleton> clusters, list<SimpleSkeleton> *dataSkeletons) {

    list<SimpleSkeleton>::iterator it;
    float minDistance;
    int idCluster;

    for (it = dataSkeletons->begin(); it != dataSkeletons->end(); it++) {
        minDistance = FLT_MAX; //init
        for (int i = 0; i < numClusters; i++) {
            float distance = clusters[i].distance(*it);
            if (distance < minDistance) {
                minDistance = distance;
                idCluster = clusters[i].idCluster;
            }
        }
        it->idCluster = idCluster;
    }
}
void assignSkeleton2Clusters(SimpleSkeleton clusters[], SimpleSkeleton *dataSkeletons) {
   
    float minDistance;
    int idCluster;
    minDistance = FLT_MAX; //init
    for (int i = 0; i < numClusters; i++) {
        float distance = clusters[i].distance(*dataSkeletons);
        if (distance < minDistance) {
            minDistance = distance;
            idCluster = clusters[i].idCluster;
        }
    }
    dataSkeletons->idCluster = idCluster;    
}

void assignSkeleton2Clusters(vector<SimpleSkeleton> clusters, SimpleSkeleton *dataSkeleton) {
   
    float minDistance;
    int idCluster;
    minDistance = FLT_MAX; //init
    for (int i = 0; i < numClusters; i++) {
        float distance = clusters[i].distance(*dataSkeleton);
        if (distance < minDistance) {
            minDistance = distance;
            idCluster = clusters[i].idCluster;            
        }
    }
    dataSkeleton->idCluster = idCluster;    
}

void getClustersFromDB(Connection *con, vector<SimpleSkeleton> *clusters) {

    int count = 0;
    //    
    //    SimpleSkeleton clusters[numClusters];
    //    SimpleSkeleton* pointer=clusters;
    //    
    ResultSet *resClusters;
    PreparedStatement *prepstmt;

    /* create a statement object */
    prepstmt = con -> prepareStatement("SELECT * FROM KMeansClusters WHERE num_clusters=?  AND compression_rate=? AND distance_measure=? ORDER BY id ASC");
    prepstmt->setInt(1, numClusters);
    prepstmt->setInt(2, compressionRate);
    prepstmt->setString(3, DistanceMeasureUsed);
    
    resClusters = prepstmt->executeQuery();

    while (resClusters->next()) {        
        (*clusters)[count].idCluster = resClusters->getInt("id");               
        (*clusters)[count].height = resClusters->getDouble("height");
        for (int i = 0; i < Num_Angles; i++) {
            (*clusters)[count].angles[i] = resClusters->getDouble(listAngles[i]);
        }
        count++;
    }
    delete resClusters;
    delete prepstmt;    
}

void getClustersFromDB(Connection *con, SimpleSkeleton clusters[]) {

    int count = 0;
    //    
    //    SimpleSkeleton clusters[numClusters];
    //    SimpleSkeleton* pointer=clusters;
    //    
    ResultSet *resClusters;
    PreparedStatement *prepstmt;

    /* create a statement object */
    prepstmt = con -> prepareStatement("SELECT * FROM KMeansClusters WHERE num_clusters=?  AND compression_rate=? AND distance_measure=? ORDER BY id ASC");
    prepstmt->setInt(1, numClusters);
    prepstmt->setInt(2, compressionRate);
    prepstmt->setString(3, DistanceMeasureUsed);
 
    resClusters = prepstmt->executeQuery();

    while (resClusters->next()) {
        clusters[count].idCluster = resClusters->getInt("id");
        clusters[count].height = resClusters->getDouble("height");
        for (int i = 0; i < Num_Angles; i++) {
            clusters[count].angles[i] = resClusters->getDouble(listAngles[i]);
        }
        count++;
    }
    delete resClusters;
    delete prepstmt;    
}

float calcClustersDistance(vector<SimpleSkeleton> clusters) {
    float distance, actualdist;

    distance = FLT_MAX;

    //DistanciaActual= Encontrar los centroides de Solucion, i y j, tales que la distancia entre i y j sea mínima
    for (int i = 0; i < numClusters; i++) {
        for (int j = 0; j < numClusters; j++) {
            if (i < j) {
                actualdist = clusters[i].distance(clusters[j]);
                if (actualdist < distance)
                    distance = actualdist;
            }
        }
    }

    return distance;
}

float calcClustersDistance(SimpleSkeleton clusters[]) {
    float distance, actualdist;

    distance = FLT_MAX;

    //DistanciaActual= Encontrar los centroides de Solucion, i y j, tales que la distancia entre i y j sea mínima
    for (int i = 0; i < numClusters; i++) {
        for (int j = 0; j < numClusters; j++) {
            if (i < j) {
                actualdist = clusters[i].distance(clusters[j]);
                if (actualdist < distance)
                    distance = actualdist;
            }
        }
    }

    return distance;
}

bool equalClusters(vector<SimpleSkeleton> clust1, vector<SimpleSkeleton> clust2) {

    for (int i = 0; i < numClusters; i++) {
        if (clust1[i].distance(clust2[i]) != 0)
            return false;
    }
    return true;
}

bool equalClusters(SimpleSkeleton* clust1, SimpleSkeleton* clust2) {

    for (int i = 0; i < numClusters; i++) {
        if (clust1[i].distance(clust2[i]) != 0)
            return false;
    }
    return true;
}

SimpleSkeleton* getCopyClusters(SimpleSkeleton* copy, SimpleSkeleton* clusters) {
    //    SimpleSkeleton copy[numClusters];    
    //    SimpleSkeleton* pointer=copy;

    for (int i = 0; i < numClusters; i++) {
        copy[i].idCluster = clusters[i].idCluster;
        copy[i].height = clusters[i].height;
        for (int j = 0; j < Num_Angles; j++)
            copy[i].angles[j] = clusters[i].angles[j];
    }

    //    return pointer;
}


//saves in DB the selectedAction for the Behaviour with that id

void saveSelectedAction(Connection *con, int selectedAction, int idBehaviour) {


}

vector<int> getSequence(string sequence){
    vector<int> result;
    
    while(true){
        int pos=sequence.find(",");  
        if(pos== -1){
            result.push_back(atoi(sequence.c_str())-1);
            return result;        
        }
        
        result.push_back(atoi(sequence.substr(0,pos).c_str())-1);        
        sequence.erase(0,pos+1);        
    }    
}

int getObjectType(int idBehaviour, int object_id){
     ResultSet *resObjects;
    PreparedStatement *prepstmt;
    if(con== NULL)
        connectSQL();
    /* create a statement object */
    prepstmt = con -> prepareStatement("SELECT DISTINCT o.id FROM  `HandObjectsData` h, Objects o "
            "WHERE  `activity_id` =? AND  `object_id` =? AND o.name = h.object_name");
    prepstmt->setInt(1, idBehaviour);
    prepstmt->setInt(2, object_id);   
 
    resObjects = prepstmt->executeQuery();
    int obj_type;
    while (resObjects->next()) {
        obj_type= resObjects->getInt(1);
    }
    
    
    delete resObjects;
    delete prepstmt;
    
//    if(obj_type==2 || obj_type==6)
//        obj_type=1;
    
    return obj_type-1;//starting in 0 for indexing vectors
}

 void includeNullActions(list<BehaviourSequence>& dataTestGlobal){
      ResultSet *resBehaviours;
    PreparedStatement *prep_stmt;

        connectSQL();
        prep_stmt = con -> prepareStatement("SELECT b.id, b.user, bc.sequence FROM `Behaviour` b, `BehaviourClustered` bc "
                "WHERE `compression_rate`=? AND `num_clusters`=? AND b.id=bc.id AND  b.name=? AND `distance_measure`=? AND b.frames>10 ");
        //begin get the resultset
        prep_stmt->setInt(1, compressionRate);
        prep_stmt->setInt(2, numClusters);
        prep_stmt->setString(3, "null");
         prep_stmt->setString(4, DistanceMeasureUsed);

        resBehaviours = prep_stmt -> executeQuery();

        /*For each action we get the ids of the behaviours*/
        while (resBehaviours->next()) {
            BehaviourSequence beh;
            beh.idAction=Num_Actions;
            beh.idBehaviour=resBehaviours->getInt("id");
            beh.idUser=resBehaviours->getInt("user");
            beh.compressionRate=compressionRate;
    //        beh.numClusters=numClusters;
            beh.sequence=getSequence(resBehaviours->getString("sequence"));
            addSkeletonsFromIdBehaviour(con,beh.idBehaviour, &(beh.skeletons));                    
            setIdCluster2Skeletons(&beh);
            addObjects2Behaviour(&beh);
             //we add it to the result list
            dataTestGlobal.push_back(beh);                        
        }   
        delete resBehaviours;
    delete prep_stmt;
    disconnectSQL();
 }

  
void  addObjects2Behaviour(BehaviourSequence *beh){
    
     ResultSet *resObjects;
    PreparedStatement *prepstmt;

    /* create a statement object */
    prepstmt = con -> prepareStatement("SELECT * FROM `PAAHandObjectsData` WHERE `activity_id`=? and `compression_rate`=? ORDER BY frame");
    prepstmt->setInt(1, beh->idBehaviour);
    prepstmt->setInt(2, compressionRate);   
 
    resObjects = prepstmt->executeQuery();

    while (resObjects->next()) {
        vector<Object> objectsFrame;
        for(int i=0;i<Max_Objects_Per_Action;i++){
            Object obj;
            obj.id= i;
            obj.distance2hand=resObjects->getDouble(i+4);            
            obj.position=string2Point3D(resObjects->getString(i+9));
            if(obj.distance2hand!=-1){
                obj.object_type= getObjectType(beh->idBehaviour, i+1);
//            else
//                obj.object_type=-1;
            
                objectsFrame.push_back(obj);
            }
        }
        beh->objectsData.push_back(objectsFrame);       
    }
     delete resObjects;
    delete prepstmt;
}
  

void setIdCluster2Skeletons(BehaviourSequence *beh){
     
     list<SimpleSkeleton>::iterator itSkel;
     int posSeq=0;
     for(itSkel=beh->skeletons.begin();itSkel!=beh->skeletons.end();itSkel++){
         itSkel->idCluster=beh->sequence[posSeq++];
     }
 }

list<BehaviourSequence> getBehavioursSequences(Connection *con, int idAction, int type){
    
    list<BehaviourSequence> result;
    
    ResultSet *resBehaviours;
    PreparedStatement *prep_stmt;

    int count = getRowsActionType(con, idAction);
    int trainingRate = getRate(count, TrainingRate + TestRate);
    int validationRate = getRate(count, ValidationRate);
    /* create a statement object */
//    prep_stmt = con -> prepareStatement("SELECT b.id, bc.sequence FROM `Behaviour` b, `BehaviourClustered` bc "
//            "WHERE `compression_rate`=? AND `num_clusters`=? AND b.id=bc.id AND  b.name=?   "
//            "LIMIT ?,?");
      prep_stmt = con -> prepareStatement("SELECT b.id, b.user, bc.sequence FROM `Behaviour` b, `BehaviourClustered` bc "
            "WHERE `compression_rate`=? AND `num_clusters`=? AND distance_measure=? AND b.id=bc.id AND  b.name=?   ");
    //begin get the resultset
    prep_stmt->setInt(1, compressionRate);
    prep_stmt->setInt(2, numClusters);
    prep_stmt->setString(3, DistanceMeasureUsed);
    prep_stmt->setString(4, listActions[idAction]);

//    switch (type) {
//        case AllData:
//            prep_stmt->setInt(4, 0);
//            prep_stmt->setInt(5, count);
//            break;
//        case TestData:
//            prep_stmt->setInt(4, 0);
//            prep_stmt->setInt(5, trainingRate);
//            break;
//        case TrainingData:
//            prep_stmt->setInt(4, 0);
//            prep_stmt->setInt(5, trainingRate);
//            break;
//        case ValidationData:
//            prep_stmt->setInt(4, count - validationRate);
//            prep_stmt->setInt(5, count);
//            break;
//        default:
//            break;
//    }     
   
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
   
    delete resBehaviours;
    delete prep_stmt;
    
    return result;
}

list<BehaviourSequence> getBehavioursSequencesWithoutSkel(Connection *con, int idAction, int type){
    
    list<BehaviourSequence> result;
    
    ResultSet *resBehaviours;
    PreparedStatement *prep_stmt;

    int count = getRowsActionType(con, idAction);
    int trainingRate = getRate(count, TrainingRate + TestRate);
    int validationRate = getRate(count, ValidationRate);
    /* create a statement object */
//    prep_stmt = con -> prepareStatement("SELECT b.id, bc.sequence FROM `Behaviour` b, `BehaviourClustered` bc "
//            "WHERE `compression_rate`=? AND `num_clusters`=? AND b.id=bc.id AND  b.name=?   "
//            "LIMIT ?,?");
      prep_stmt = con -> prepareStatement("SELECT b.id, b.user, bc.sequence FROM `Behaviour` b, `BehaviourClustered` bc "
            "WHERE `compression_rate`=? AND `num_clusters`=? AND distance_measure=? AND b.id=bc.id AND  b.name=?   ");
    //begin get the resultset
    prep_stmt->setInt(1, compressionRate);
    prep_stmt->setInt(2, numClusters);
    prep_stmt->setString(3, DistanceMeasureUsed);
    prep_stmt->setString(4, listActions[idAction]);

//    switch (type) {
//        case AllData:
//            prep_stmt->setInt(4, 0);
//            prep_stmt->setInt(5, count);
//            break;
//        case TestData:
//            prep_stmt->setInt(4, 0);
//            prep_stmt->setInt(5, trainingRate);
//            break;
//        case TrainingData:
//            prep_stmt->setInt(4, 0);
//            prep_stmt->setInt(5, trainingRate);
//            break;
//        case ValidationData:
//            prep_stmt->setInt(4, count - validationRate);
//            prep_stmt->setInt(5, count);
//            break;
//        default:
//            break;
//    }     
   
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
//        addSkeletonsFromIdBehaviour(con,beh.idBehaviour, &(beh.skeletons));        
        
        setIdCluster2Skeletons(&beh);
        //we add it to the result list
        result.push_back(beh);       
    }    
   
    delete resBehaviours;
    delete prep_stmt;
    
    return result;
}

int getActionId(string actionName){
    
    for(int i=0;i<Num_Actions;i++){
        if(actionName==listActions[i])
            return i;
    }
}

 BehaviourSequence getBehaviour(Connection *con, int idBehaviour){
    
    BehaviourSequence result;
    
    ResultSet *resBehaviours;
    PreparedStatement *prep_stmt;

    /* create a statement object */
    prep_stmt = con -> prepareStatement("SELECT b.name, bc.sequence FROM `Behaviour` b, `BehaviourClustered` bc "
            "WHERE `compression_rate`=? AND `num_clusters`=? AND distance_measure=? AND b.id=bc.id AND  b.id=?   "
            );
    //begin get the resultset
    prep_stmt->setInt(1, compressionRate);
    prep_stmt->setInt(2, numClusters);
    prep_stmt->setString(3, DistanceMeasureUsed);
    prep_stmt->setInt(4, idBehaviour);
   
    resBehaviours = prep_stmt -> executeQuery();

    /*For each action we get the ids of the behaviours*/
    while (resBehaviours->next()) {       
        result.idAction=getActionId(resBehaviours->getString("name"));
        result.idBehaviour=idBehaviour;
        result.compressionRate=compressionRate;
//        beh.numClusters=numClusters;
        result.sequence=getSequence(resBehaviours->getString("sequence"));
        addSkeletonsFromIdBehaviour(con,result.idBehaviour, &(result.skeletons));        
        
        setIdCluster2Skeletons(&result);            
    }    
   
    delete resBehaviours;
    delete prep_stmt;
    
    return result;
}

list<BehaviourSequence> setTrainingAndTest(list<BehaviourSequence> *dataAction,
        list<BehaviourSequence> *dataTrainingGlobal, list<BehaviourSequence> *dataTestGlobal){
    
    
//    BehaviourSequence first= dataAction->front();
//    first.compressionRate=2;
    
    
    list<BehaviourSequence> trainingData;
    
    list<BehaviourSequence>::iterator it=dataAction->begin();
    
    int numTraining=getRate(dataAction->size(),TrainingRate);//*100/(TrainingRate+TestRate));
    int actualCount=dataAction->size();
        
    for (int i = 0; i < numTraining; i++){
        int sel=rand() % actualCount--;
        for(int j=0;j<sel;j++)
            it++;
        trainingData.push_back(*it);  
        dataTrainingGlobal->push_back(*it); 
        dataAction->erase(it);
        it=dataAction->begin();
    }
     
    for(it;it!=dataAction->end();it++)
        dataTestGlobal->push_back(*it);
    
       return trainingData;
}
 

vector<int> getIdsVector(string ids){
     vector<int> result;
          
     while(true){
        int pos=ids.find(",");  
        if(pos== -1){
            result.push_back(atoi(ids.c_str()));
            return result;        
        }        
        result.push_back(atoi(ids.substr(0,pos).c_str()));        
        ids.erase(0,pos+1);        
    }  
    
 }
 
list<BehaviourSequence> getTrainedData(Connection *con,int idAction){
     
     list<BehaviourSequence> result;
     
     stringstream ss;     
     ss<<"SELECT `ids_training` FROM `InferenceResults` "
             "WHERE `inference_class_used`=? AND `num_clusters` =? AND `compression_rate` =? AND distance_measure=? AND `training_rate`=? AND `test_rate` =?";
     
    ResultSet *resBehaviours;
    PreparedStatement *prep_stmt;     
      /* create a statement object */
    prep_stmt = con -> prepareStatement(ss.str());
    //begin get the resultset    
    prep_stmt->setString(1, InferenceClassUsed);
    prep_stmt->setInt(2, numClusters);
    prep_stmt->setInt(3, compressionRate);
    prep_stmt->setString(4, DistanceMeasureUsed);
    prep_stmt->setInt(5, TrainingRate);
    prep_stmt->setInt(6, TestRate);        
    
    resBehaviours = prep_stmt -> executeQuery();
     
    string ids;    
      /*For each action we get the ids of the behaviours*/
    while (resBehaviours->next()) {        
        ids = resBehaviours->getString(1);      
    }    
        
    vector<int> idsVector= getIdsVector(ids);   
    for(int i=0;i < idsVector.size();i++){           
        BehaviourSequence beh= getBehaviour(con,idsVector[i]);
        
        if(beh.idAction==idAction)         
            result.push_back(beh);
    }
    
    delete resBehaviours;
    delete prep_stmt;     
     
    return result;
     
 }

list<BehaviourSequence> getTrainedData(Connection *con,int idAction, string dataset){
     
     list<BehaviourSequence> result;
     
     stringstream ss;
     ss<<"SELECT  `ids_training` FROM "<<dataset<<" WHERE  `num_clusters` =? AND  `compression_rate` =? AND distance_measure=? ";
     
    ResultSet *resBehaviours;
    PreparedStatement *prep_stmt;     
      /* create a statement object */
    prep_stmt = con -> prepareStatement(ss.str());
    //begin get the resultset    
    prep_stmt->setInt(1, numClusters);
    prep_stmt->setInt(2, compressionRate);
    prep_stmt->setString(3, DistanceMeasureUsed);
        
    resBehaviours = prep_stmt -> executeQuery();
     
    string ids;    
      /*For each action we get the ids of the behaviours*/
    while (resBehaviours->next()) {        
        ids = resBehaviours->getString(1);      
    }    
        
    vector<int> idsVector= getIdsVector(ids);   
    for(int i=0;i < idsVector.size();i++){           
        BehaviourSequence beh= getBehaviour(con,idsVector[i]);
        
        if(beh.idAction==idAction)         
            result.push_back(beh);
    }
    
    delete resBehaviours;
    delete prep_stmt;     
     
    return result;
     
 }

void saveValidationDataKTest(Connection *con, int errors,       
                 vector<int> * errorsByAction, vector<int> *errorsByActionSel){
     
     PreparedStatement *prep_stmt;
             
    std::ostringstream s1,s2;
    
     for(int i=0;i<Num_Actions;i++){
        s1 << (*errorsByAction)[i] << ",";
        s2 << (*errorsByActionSel)[i] << ",";       
    }
    string errorsByAct, errorsByActSel;
    
    errorsByAct= s1.str();
    errorsByActSel=s2.str();
       
    prep_stmt = con -> prepareStatement(
            "INSERT INTO `KTestResults`"
            "(`num_clusters`, `compression_rate`,`validation_rate`,`k_rate`,`errors`,"
            " `errors_by_action`, `errors_by_action_selected`) "
            "VALUES (?,?,?,?,?,?,?)"            
            );

    prep_stmt -> setInt(1, numClusters);
    prep_stmt -> setInt(2, compressionRate);
    prep_stmt -> setInt(3, ValidationRate);  
    prep_stmt -> setInt(4, K_Test_Factor);  
    prep_stmt -> setInt(5, errors);  
    prep_stmt -> setString(6, errorsByAct.substr(0, errorsByAct.size() - 1));
    prep_stmt -> setString(7, errorsByActSel.substr(0, errorsByActSel.size() - 1));
    
    prep_stmt->executeUpdate();
    
    delete prep_stmt;
 }


 void saveKTestTrainingData(Connection *con, string idsTraining, 
         int bestErrorTrain, int bestErrorTest,
         int worstErrorTrain, int worstErrorTest,
         int meanErrorTrain, int meanErrorTest,
         vector<int> *errorsTrainByAction, vector<int> *errorsTrainByActionSel,
         vector<int> *errorsTestByAction, vector<int> *errorsTestByActionSel){
     
    PreparedStatement *prep_stmt;
    std::ostringstream s1,s2,s3,s4;
    
    for(int i=0;i<Num_Actions;i++){
        s1 << (*errorsTrainByAction)[i] << ",";
        s2 << (*errorsTrainByActionSel)[i] << ",";
        s3 << (*errorsTestByAction)[i] << ",";
        s4 << (*errorsTestByActionSel)[i] << ",";
    }
    string errorsTrainByAct, errorsTrainByActSel, errorsTestByAct, errorsTestByActSel;
    
    errorsTrainByAct= s1.str();
    errorsTrainByActSel=s2.str();
    errorsTestByAct= s3.str();
    errorsTestByActSel=s4.str();
       
    prep_stmt = con -> prepareStatement(
            "INSERT INTO `KTestTrainingSet`(`num_clusters`, `compression_rate`, "
            "`training_rate`, `test_rate`, `k_rate`,`best_errors_training`, `best_errors_test`,"
            " `worst_errors_training`, `worst_errors_test`, `mean_errors_training`, "
            "`mean_errors_test`, `errors_training_by_action`, `errors_training_by_action_selected`, "
            "`errors_test_by_action`, `errors_test_by_action_selected`, `ids_training`) "
            "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)"             
            );

    prep_stmt -> setInt(1, numClusters);
    prep_stmt -> setInt(2, compressionRate);
    prep_stmt -> setInt(3, TrainingRate);
    prep_stmt -> setInt(4, TestRate);
    prep_stmt -> setInt(5, kRate);
    prep_stmt -> setInt(6, bestErrorTrain);
    prep_stmt -> setInt(7, bestErrorTest);
    prep_stmt -> setInt(8, worstErrorTrain);
    prep_stmt -> setInt(9, worstErrorTest);
    prep_stmt -> setInt(10, meanErrorTrain);
    prep_stmt -> setInt(11, meanErrorTest);
    prep_stmt -> setString(12, errorsTrainByAct.substr(0, errorsTrainByAct.size() - 1));
    prep_stmt -> setString(13, errorsTrainByActSel.substr(0, errorsTrainByActSel.size() - 1));
    prep_stmt -> setString(14, errorsTestByAct.substr(0, errorsTestByAct.size() - 1));
    prep_stmt -> setString(15, errorsTestByActSel.substr(0, errorsTestByActSel.size() - 1));
    prep_stmt -> setString(16, idsTraining);
    
    prep_stmt->executeUpdate();
    
    delete prep_stmt;
 
 }
 
 string getTimes(vector<double>& times, vector<int>& counts){
 
     std::ostringstream s1; 
    for(int i=0;i<times.size();i++)
        s1<<times[i]/(double)counts[i]<<",";
     
    string timesFinal= s1.str();
    
    return timesFinal.substr(0, timesFinal.size()-1);
     
 }
  
 string getIdsTraining(list<BehaviourSequence> *dataTraining){
     std::ostringstream s1;
 
     list<BehaviourSequence>::iterator it;
    for(it=dataTraining->begin();it!=dataTraining->end();it++)
        s1<<it->idBehaviour <<",";
     
    string idsTraining= s1.str();
    
    return idsTraining.substr(0, idsTraining.size()-1);
 }
 
  void saveValidationData(Connection *con, double total, double errors,       
                 vector<int> * errorsByAction, vector<int> *errorsByActionSel, double timeLearning, double timeDetection){
     
     PreparedStatement *prep_stmt;
             
    std::ostringstream s1,s2;
    
     for(int i=0;i<Num_Actions;i++){
        s1 << (*errorsByAction)[i] << ",";
        s2 << (*errorsByActionSel)[i] << ",";       
    }
    string errorsByAct, errorsByActSel;
    
    errorsByAct= s1.str();
    errorsByActSel=s2.str();
       
    prep_stmt = con -> prepareStatement(
            "INSERT INTO `ValidationResults`"
            "(`num_clusters`, `compression_rate`,`distance_measure`, `validation_rate`,`typeInference`, `total`,`errors`,"
            " `errors_by_action`, `errors_by_action_selected`, `time_learning`, `time_detection`) "
            "VALUES (?,?,?,?,?,?,?,?,?,?,?)"            
            );

    prep_stmt -> setInt(1, numClusters);
    prep_stmt -> setInt(2, compressionRate);
    prep_stmt->setString(3, DistanceMeasureUsed);    
    prep_stmt -> setInt(4, ValidationRate);  
    prep_stmt -> setString(5, InferenceClassUsed);  
    prep_stmt -> setDouble(6, total);  
    prep_stmt -> setDouble(7, errors);  
    prep_stmt -> setString(8, errorsByAct.substr(0, errorsByAct.size() - 1));
    prep_stmt -> setString(9, errorsByActSel.substr(0, errorsByActSel.size() - 1));
     prep_stmt -> setDouble(10, timeLearning);  
    prep_stmt -> setDouble(11, timeDetection);  
    
    prep_stmt->executeUpdate();
    
    delete prep_stmt;
 }
  
  void saveTrainingData(Connection *con, string idsTraining, 
         double bestErrorTrain, double bestErrorTest,
         double worstErrorTrain, double worstErrorTest,
         double meanErrorTrain, double meanErrorTest,
          vector<int> *errorsTrainByAction, vector<vector<int> >*errorsTrainByActionSel,
         vector<int> *errorsTestByAction, vector<vector<int> > *errorsTestByActionSel,
        string timeLearn, string timeDetect){
     
    PreparedStatement *prep_stmt;
    std::ostringstream s1,s2,s3,s4;    
   
     for(int i=0;i<Num_Actions;i++){
        s1 << (*errorsTrainByAction)[i] << ",";               
        s3 << (*errorsTestByAction)[i] << ",";
        for(int j=0;j<Num_Actions;j++){
            s2 << (*errorsTrainByActionSel)[i][j] << ",";
            s4 << (*errorsTestByActionSel)[i][j] << ",";
        }
        s2<<endl;
        s4<<endl;
    }
    string errorsTrainByAct, errorsTrainByActSel, errorsTestByAct, errorsTestByActSel;
    
    errorsTrainByAct= s1.str();
    errorsTrainByActSel=s2.str();
    errorsTestByAct= s3.str();
    errorsTestByActSel=s4.str();
       
    prep_stmt = con -> prepareStatement(
            "INSERT INTO `InferenceResults`(`inference_class_used`,`num_clusters`, `compression_rate`, "
            "`training_rate`, `test_rate`, `best_errors_training`, `best_errors_test`,"
            " `worst_errors_training`, `worst_errors_test`, `mean_errors_training`, "
            "`mean_errors_test`, `errors_training_by_action`, `errors_training_by_action_selected`, "
            "`errors_test_by_action`, `errors_test_by_action_selected`, `ids_training`, `distance_measure`, `times_learning`, `times_detecting`) "
            "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)"             
            );

    prep_stmt -> setString(1, InferenceClassUsed);    
    prep_stmt -> setInt(2, numClusters);
    prep_stmt -> setInt(3, compressionRate);
    prep_stmt -> setInt(4, TrainingRate);
    prep_stmt -> setInt(5, TestRate);
    prep_stmt -> setDouble(6, bestErrorTrain);
    prep_stmt -> setDouble(7, bestErrorTest);
    prep_stmt -> setDouble(8, worstErrorTrain);
    prep_stmt -> setDouble(9, worstErrorTest);
    prep_stmt -> setDouble(10, meanErrorTrain);
    prep_stmt -> setDouble(11, meanErrorTest);
    prep_stmt -> setString(12, errorsTrainByAct.substr(0, errorsTrainByAct.size() - 1));
    prep_stmt -> setString(13, errorsTrainByActSel.substr(0, errorsTrainByActSel.size() - 1));
    prep_stmt -> setString(14, errorsTestByAct.substr(0, errorsTestByAct.size() - 1));
    prep_stmt -> setString(15, errorsTestByActSel.substr(0, errorsTestByActSel.size() - 1));
    prep_stmt -> setString(16, idsTraining);
    prep_stmt -> setString(17, DistanceMeasureUsed);
    prep_stmt -> setString(18, timeLearn);
    prep_stmt -> setString(19, timeDetect);
    
    prep_stmt->executeUpdate();
    
    delete prep_stmt;
 
 }
  
  int getMaxLength(list<BehaviourSequence>& dataTraining){
    int maxLength=0; 
     for (list<BehaviourSequence>::iterator it = dataTraining.begin(); it != dataTraining.end(); it++) {
        if(it->sequence.size()>maxLength)
            maxLength=it->sequence.size();
     }
    return maxLength;
}
int getMinLength(list<BehaviourSequence>& dataTraining){
    int minLength=1000; 
     for (list<BehaviourSequence>::iterator it = dataTraining.begin(); it != dataTraining.end(); it++) {
        if(it->sequence.size()<minLength)
            minLength=it->sequence.size();
     }
    return minLength;
}

 
