

#include "config.h"
#include "SimpleSkeleton.h"
#include "Commons.h"

using namespace std;
using namespace sql;
        


/*!Saves the skeleton cluster in the DB. Id of the cluster and type="initial" or "final" are the row's key*/
void saveSkelClusters(Connection *con, int idCluster, SimpleSkeleton skeleton, int type) {
    PreparedStatement *prep_stmt;
    prep_stmt = con -> prepareStatement(
            "INSERT INTO `KMeansClusters` (`id`,`num_clusters`,`compression_rate`, `distance_measure`,`height`, `ang0`, `ang1`, `ang2`, `ang3`, `ang4`, `ang5`, `ang6`, `ang7`, `ang8`, `ang9`, `ang10`) "
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"
            );

    prep_stmt -> setInt(1, idCluster);
    prep_stmt -> setInt(2, type);
    prep_stmt -> setInt(3, compressionRate);
    prep_stmt -> setString(4, DistanceMeasureUsed);
    prep_stmt -> setDouble(5, skeleton.height);

    for (int i = 0; i < Num_Angles; i++) {
        prep_stmt -> setDouble(i + 6, skeleton.angles[i]);
    }

    prep_stmt -> executeUpdate();

    delete prep_stmt;
}

SimpleSkeleton fillSkeleton(ResultSet *resSkeletons) {
 
    SimpleSkeleton skel;  
    
    while (resSkeletons->next()){
        skel.height = resSkeletons->getDouble("height");        
        for (int i = 0; i < Num_Angles; i++) {
            skel.angles[i] = resSkeletons->getDouble(listAngles[i]);
        }   
        break;
    }
    skel.normalizeSkel();
    return skel;
}

SimpleSkeleton getRandomSkeleton(Connection *con, int idBehaviour, int totalFrames, int samples4action, int count) {
    
    ResultSet *resSkeletons;
    PreparedStatement *prep_stmt;
    
    /* create a statement object */
    prep_stmt = con -> prepareStatement("SELECT * FROM PAASkeleton WHERE id_behaviour=? AND compression_rate=? AND frame<=? AND frame>?");

    /*we create a random frame in between the range of the portion expressed by "count" respect "samples4action*/
    int frame = rand() % totalFrames/samples4action + (count*totalFrames/samples4action);

    prep_stmt->setInt(1, idBehaviour);
    prep_stmt->setInt(2, compressionRate);
    prep_stmt->setInt(3, frame + compressionRate/2 + compressionRate%2);
    prep_stmt->setInt(4, frame - compressionRate/2);

    resSkeletons = prep_stmt->executeQuery();  
    
    SimpleSkeleton result=fillSkeleton(resSkeletons); 
    
    delete prep_stmt;
    delete resSkeletons;
    return result;
}

SimpleSkeleton* calcInitialClusters(Connection *con) {
 //   cout << "Calculating initial Clusters" << endl;

    SimpleSkeleton* clusters= new SimpleSkeleton[numClusters];

    ResultSet *resBehaviours,*resLimits;
    PreparedStatement *prep_stmt;//,*prep_stmtLimits;

    int countClusters = 0;
    //SimpleSkeleton skelVector[numClusters];
//               
//     prep_stmtLimits = con -> prepareStatement("SELECT ROUND(COUNT(*) * ?/100) FROM Behaviour WHERE name=?");
//      prep_stmtLimits = con -> prepareStatement("SELECT COUNT(*) FROM Behaviour WHERE name=?");
     
    /* create a statement object */
    prep_stmt = con -> prepareStatement("SELECT id,frames FROM Behaviour WHERE name=?");// LIMIT 0,?");
        
    int idBehaviour,totalFrames,index;    
    int samples4action = numClusters / Num_Actions;
        
    /*For each action we get some samples*/
    for (int i = 0; i < Num_Actions; i++) {
//        //begin get the number of instance for the training
//         prep_stmtLimits->setInt(1,TrainingRate+TestRate);
//         prep_stmtLimits->setString(1,listActions[i]);
//         resLimits = prep_stmtLimits -> executeQuery();
//         int limit;
//         while(resLimits->next()){
//             limit=resLimits->getInt(1);
//         }
         //end get number of instance, the limit
         
         //begin get the resultset
         prep_stmt->setString(1, listActions[i]);
//         prep_stmt->setInt(2,limit);
         resBehaviours = prep_stmt -> executeQuery();
        
        /*For each action we get some samples*/
        for (int j = 0; j < samples4action; j++) {
            resBehaviours->first();
            //randomly we choose one behaviour and its skeleton            
            index = rand() % resBehaviours->rowsCount()+ 1; //index of the behaviour to get

            resBehaviours->absolute(index);
            idBehaviour = resBehaviours->getInt("id");
            totalFrames = resBehaviours->getInt("frames");

            clusters[countClusters] = getRandomSkeleton(con, idBehaviour, totalFrames, samples4action, j);
            clusters[countClusters].idCluster=countClusters++ +1;
        }//end samples per action

    }//end for actions   

    
//    std::ostringstream type;
//    type<<"initial"<<numClusters;
//    //now we save the clusters in the DB
//    for (int i = 0; i < numClusters; i++) {
//        saveSkelClusters(con, i + 1, clusters[i], numClusters);
//    }
    
    
    delete resBehaviours;
 //   delete resLimits;
    delete prep_stmt;
  //  delete prep_stmtLimits;
    return clusters;
}


void calcKmeans(Connection *con, SimpleSkeleton* clusters, list<SimpleSkeleton> *dataSkeletons){  
    
//    SimpleSkeleton* copyClusters= new SimpleSkeleton[numClusters];
//    getCopyClusters(copyClusters,clusters);    
    // copyClusters=clusters;
     
    list<SimpleSkeleton>::iterator it;
    int count[numClusters];   
    int idCluster;
    int totalCount=0;     
    bool some_point_is_moving=true;
    float max=FLT_MIN;
    float dis;    
    while(some_point_is_moving){//we loop until it converges        
        some_point_is_moving=false;
        //Now we start Kmeans by placing each skeleton on one cluster       
        assignSkeletons2Clusters(clusters,dataSkeletons);
        
        //initializing the count
        for (int i = 0; i < numClusters; i++) {
            count[i] = 0;
        }

        //now we calculate the new clusters doing the centroide
        for (it = dataSkeletons->begin(); it != dataSkeletons->end(); it++) {
            idCluster = it->idCluster - 1;
            if (count[idCluster] == 0) {
                //initialize
                clusters[idCluster].height = it->height;
                for (int j = 0; j < Num_Angles; j++)
                    clusters[idCluster].angles[j] = it->angles[j];

            } else {//not the first
                clusters[idCluster].height += it->height;
                for (int j = 0; j < Num_Angles; j++)
                    clusters[idCluster].angles[j] += it->angles[j];
            }
            count[idCluster]++; //inserted in that cluster
        }

        //now the mean for each cluster
        for (int i = 0; i < numClusters; i++) {
            if (count[i] != 0) {
                clusters[i].height /= count[i];
                for (int j = 0; j < Num_Angles; j++)
                    clusters[i].angles[j] /= count[i];
            }
        }
//        if (equalClusters(copyClusters, clusters) ){//|| totalCount==150){           
//            break;//then we got the convergence and we stop;
//        }
       
        if(totalCount<Max_KMeans_Loop){                               
                some_point_is_moving=true;
//                getCopyClusters(copyClusters,clusters);
//                dis= calcClustersDistance(clusters);
//                if(dis>max){                     
//                   getCopyClusters(copyClusters,clusters);
//                    max=dis;
//                    cout << "\tnew dist=" <<dis<< endl;	
//                }                    
        }
//         copyClusters=clusters;
         totalCount++;
    }       
//    getCopyClusters(clusters, copyClusters);
//    delete[] copyClusters;
//    copyClusters = NULL ;
  //  return clusters;
    
}


void startKmeans(Connection *con) {

    /***********************START ALGORITHM***************************************************/

    cout << "Starting KMeans" << endl;
     
    SimpleSkeleton* clusters= new SimpleSkeleton[numClusters];
            
    SimpleSkeleton* bestClusters= new SimpleSkeleton[numClusters];
    
    float clusterDistance = FLT_MIN;
    float actualDistance;

    //we get the data we are going to use for the whole training
    list<SimpleSkeleton> dataSkeletons = getDataSkeletons(con);

    for (int iterations = 0; iterations < Max_Iterations; iterations++) { //multistart  
        clusters=calcInitialClusters(con); //for initial execution            
        calcKmeans(con, clusters, &dataSkeletons);
        actualDistance = calcClustersDistance(clusters);
       
        if (actualDistance > clusterDistance) {
            getCopyClusters(bestClusters, clusters);
            //bestClusters=clusters;
            clusterDistance = actualDistance;
            cout << "Better clusters found, distance: " << clusterDistance << endl;
        }
    }//end iterations MultiStart


    cout << "Saving the best solution in the db" << endl;
    //now we save the final clusters in DB
    for (int i = 0; i < numClusters; i++) {
        saveSkelClusters(con, bestClusters[i].idCluster, bestClusters[i], numClusters);
    }
    /***********************END ALGORITHM******************************************************/
    
} // end start
