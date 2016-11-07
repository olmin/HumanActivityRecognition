#include "config.h"
#include "SimpleSkeleton.h"
#include "Commons.h"
#include "BehaviourSequence.h"

using namespace std;
using namespace sql;

void saveClusteredBehaviour(Connection *con, int idBehaviour, string sequence){
        
    PreparedStatement *prep_stmt;
    prep_stmt = con -> prepareStatement(
            "INSERT INTO `BehaviourClustered`(`id`, `compression_rate`,`distance_measure`, `num_clusters`, `sequence`) "
            "VALUES (?,?,?,?,?);"
            );

    prep_stmt -> setInt(1, idBehaviour);
    prep_stmt -> setInt(2, compressionRate);
    prep_stmt -> setString(3, DistanceMeasureUsed);
    prep_stmt -> setInt(4, numClusters);
    prep_stmt -> setString(5, sequence);
    
    prep_stmt->executeUpdate();
    
    delete prep_stmt;
}

void startClusteringData(Connection *con) {

    cout<< "Starting clustering data"<<endl;
    
    //First we get the skeletons from the PAA table  
    list<int> listIdBehaviours = getListIdBehaviours(con);
    list<int>::iterator it;
 
    //we get the clusters from DB
    SimpleSkeleton clusters[numClusters];
    getClustersFromDB(con,clusters);
   
       
    for ( it=listIdBehaviours.begin() ; it != listIdBehaviours.end(); it++ ){
        //vars init 
        list<SimpleSkeleton> dataSkeletons;
        list<SimpleSkeleton>::iterator itSkel;
        std::ostringstream sequenceOS;

        addSkeletonsFromIdBehaviour(con, *it, &dataSkeletons);

        //we assign each frame skeleton to a cluster
        assignSkeletons2Clusters(clusters, &dataSkeletons);

        for (itSkel = dataSkeletons.begin(); itSkel != dataSkeletons.end(); itSkel++) {
            sequenceOS << itSkel->idCluster << ",";
        }
        string sequence = sequenceOS.str();
        saveClusteredBehaviour(con, *it, sequence.substr(0, sequence.size() - 1));
        
        sequenceOS.clear();
    }  
    
    cout<< "Ending and saved clustered data"<<endl;

}

