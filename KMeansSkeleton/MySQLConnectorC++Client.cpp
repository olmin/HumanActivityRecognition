/* Standard C++ headers */
#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <stdexcept>
#include <list>
#include <math.h>

/* MySQL Connector/C++ specific headers */
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/metadata.h>
#include <cppconn/resultset_metadata.h>
#include <cppconn/exception.h>
#include <cppconn/warning.h>

#define DBHOST "tcp://127.0.0.1:3306"
#define USER "olmin"
#define PASSWORD "dover"
#define DATABASE "behavioursdb"


/*! Number of joints angles. */
#define Num_Angles 11
/*!Number of different actions*/
#define Num_Actions 7
/*! Number of total clusters*/
#define Num_Clusters 28
/*! CompressionRate for the PAA algorithm*/
#define CompressionRate 4
/*!Multistart max iterations*/
#define Max_Iterations 1000 

#define TrainingRate 70
#define TestRate 0
#define ValidationRate 30

/*!Values min and max of height and angles*/
#define Min_Height 0
#define Max_Height 3000
#define Min_Angle 0
#define Max_Angle 180

using namespace std;
using namespace sql;

class SimpleSkeleton {
public:

    SimpleSkeleton() {
    }
      
    int idCluster;
    float height;
    float angles[Num_Angles];
    
    /*!Calculates the distance of the two skeletons*/
    float distance (SimpleSkeleton other){
        float dist=0;
        dist+=fabs(this->height - other.height);
        for(int i=0;i<Num_Angles;i++){
            dist+=fabs(this->angles[i] - other.angles[i]);
        }        
        return dist;
    }

    /*!normalizes the skeleton*/
    void normalizeSkel() {
        this->height = (this->height - Min_Height) / (Max_Height - Min_Height);
        for (int i = 0; i < Num_Angles; i++)
            this->angles[i] = (this->angles[i] - Min_Angle) / (Max_Angle - Min_Angle);
    }
    
};
        

/*!List of actions that can be done*/
static const string listActions[Num_Actions] = {
    "walk",
    "sit down",
    "stand up from chair",
    "bend down",
    "bend up",
    "twist right",
    "twist left"
};

/*!Saves the skeleton cluster in the DB. Id of the cluster and type="initial" or "final" are the row's key*/
void saveSkelClusters(Connection *con, int idCluster, SimpleSkeleton skeleton, int type) {
    PreparedStatement *prep_stmt;
    prep_stmt = con -> prepareStatement(
            "INSERT INTO `KMeansClusters` (`id`,`type`,`height`, `ang0`, `ang1`, `ang2`, `ang3`, `ang4`, `ang5`, `ang6`, `ang7`, `ang8`, `ang9`, `ang10`) "
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"
            );

    prep_stmt -> setInt(1, idCluster);
    prep_stmt -> setInt(2, type);
    prep_stmt -> setDouble(3, skeleton.height);

    for (int i = 0; i < Num_Angles; i++) {
        prep_stmt -> setDouble(i + 4, skeleton.angles[i]);
    }

    prep_stmt -> executeUpdate();

    delete prep_stmt;
}

SimpleSkeleton fillSkeleton(ResultSet *resSkeletons) {
 
    SimpleSkeleton skel;  
    
    while (resSkeletons->next()){
        skel.height = resSkeletons->getDouble("height");        
        for (int i = 0; i < Num_Angles; i++) {
            skel.angles[i] = resSkeletons->getDouble(i+4);
        }   
        break;
    }
    skel.normalizeSkel();
    return skel;
}

SimpleSkeleton getRandomSkelton(Connection *con, int idBehaviour, int totalFrames, int samples4action, int count) {
    
    ResultSet *resSkeletons;
    PreparedStatement *prep_stmt;
    
    /* create a statement object */
    prep_stmt = con -> prepareStatement("SELECT * FROM PAASkeleton WHERE id_behaviour=? AND frame<=? AND frame>?");

    /*we create a random frame in between the range of the portion expressed by "count" respect "samples4action*/
    int frame = rand() % totalFrames/samples4action + (count*totalFrames/samples4action);

    prep_stmt->setInt(1, idBehaviour);
    prep_stmt->setInt(2, frame + CompressionRate/2);
    prep_stmt->setInt(3, frame - CompressionRate/2);

    resSkeletons = prep_stmt->executeQuery();  
    
    SimpleSkeleton result=fillSkeleton(resSkeletons); 
    
    delete prep_stmt;
    delete resSkeletons;
    return result;
}

void calcInitialClusters(Connection *con, SimpleSkeleton clusters[Num_Clusters]) {
 //   cout << "Calculating initial Clusters" << endl;


    ResultSet *resBehaviours,*resLimits;
    PreparedStatement *prep_stmt,*prep_stmtLimits;

    int countClusters = 0;
    //SimpleSkeleton skelVector[Num_Clusters];
               
    prep_stmtLimits = con -> prepareStatement("SELECT ROUND(COUNT(*) * ?/100) FROM Behaviour WHERE name=?");
       
    /* create a statement object */
    prep_stmt = con -> prepareStatement("SELECT id,frames FROM Behaviour WHERE name=? LIMIT 0,?");
        
    int idBehaviour,totalFrames,index;    
    int samples4action = Num_Clusters / Num_Actions;
    srand(time(0));
    
    /*For each action we get some samples*/
    for (int i = 0; i < Num_Actions; i++) {
        //begin get the number of instance for the training
         prep_stmtLimits->setInt(1,TrainingRate);
         prep_stmtLimits->setString(2,listActions[i]);
         resLimits = prep_stmtLimits -> executeQuery();
         int limit;
         while(resLimits->next()){
             limit=resLimits->getInt(1);
         }
         //end get number of instance, the limit
         
         //begin get the resultset
         prep_stmt->setString(1, listActions[i]);
         prep_stmt->setInt(2,limit);
         resBehaviours = prep_stmt -> executeQuery();
        
        /*For each action we get some samples*/
        for (int j = 0; j < samples4action; j++) {
            resBehaviours->first();
            //randomly we choose one behaviour and its skeleton            
            index = rand() % resBehaviours->rowsCount()+ 1; //index of the behaviour to get

            resBehaviours->absolute(index);
            idBehaviour = resBehaviours->getInt("id");
            totalFrames = resBehaviours->getInt("frames");

            clusters[countClusters] = getRandomSkelton(con, idBehaviour, totalFrames, samples4action, j);
            clusters[countClusters].idCluster=countClusters++ +1;
        }//end samples per action

    }//end for actions   

    
//    std::ostringstream type;
//    type<<"initial"<<Num_Clusters;
//    //now we save the clusters in the DB
//    for (int i = 0; i < Num_Clusters; i++) {
//        saveSkelClusters(con, i + 1, clusters[i], Num_Clusters);
//    }
    
    
    delete resBehaviours;
    delete resLimits;
    delete prep_stmt;
    delete prep_stmtLimits;
}

void getInitialClusters(Connection *con, SimpleSkeleton clusters[Num_Clusters]){
    
    int count=0;
//    
//    SimpleSkeleton clusters[Num_Clusters];
//    SimpleSkeleton* pointer=clusters;
//    
    ResultSet *resClusters;
    PreparedStatement *prepstmt;

    /* create a statement object */
    prepstmt = con -> prepareStatement("SELECT * FROM KMeansClusters WHERE type=? ORDER BY id ASC");
    
    prepstmt->setInt(1,Num_Clusters);
    
    resClusters= prepstmt->executeQuery();
    
    while(resClusters->next()){
        clusters[count].idCluster=resClusters->getInt("id");
        clusters[count].height=resClusters->getDouble("height");
        for(int i=0;i<Num_Angles;i++){
            clusters[count].angles[i]=resClusters->getDouble(i+4);
        }
        count++;
    }
    
//    return pointer;
} 

list<int> getListIdBehaviours(Connection *con){
    
    list<int> listIds;
    
    ResultSet *resBehaviours,*resLimits;
    PreparedStatement *prep_stmt,*prep_stmtLimits;
    
     prep_stmtLimits = con -> prepareStatement("SELECT ROUND(COUNT(*) * ?/100) FROM Behaviour WHERE name=?");
       
    /* create a statement object */
    prep_stmt = con -> prepareStatement("SELECT id FROM Behaviour WHERE name=? LIMIT 0,?");
        
    /*For each action we get some samples*/
    for (int i = 0; i < Num_Actions; i++) {
        //begin get the number of instance for the training
         prep_stmtLimits->setInt(1,TrainingRate);
         prep_stmtLimits->setString(2,listActions[i]);
         resLimits = prep_stmtLimits -> executeQuery();
         int limit;
         while(resLimits->next()){
             limit=resLimits->getInt(1);
         }
         //end get number of instance, the limit
         
         //begin get the resultset
         prep_stmt->setString(1, listActions[i]);
         prep_stmt->setInt(2,limit);
         resBehaviours = prep_stmt -> executeQuery();
        
        /*For each action we get the ids of the behaviours*/
        while (resBehaviours->next()) {            
            listIds.push_back(resBehaviours->getInt("id"));
        }
    }//end list of actions
    
    return listIds;
}

void addSkeletonsFromIdBehaviour(Connection *con, int idBehaviour,list<SimpleSkeleton> *listSkel ){
    ResultSet *resSkeletons;
    PreparedStatement *prep_stmt;
    
    /* create a statement object */
    prep_stmt = con -> prepareStatement("SELECT * FROM PAASkeleton WHERE id_behaviour=?");
    prep_stmt->setInt(1, idBehaviour);

    resSkeletons = prep_stmt->executeQuery();    
    
     while (resSkeletons->next()){
        SimpleSkeleton skel;
        skel.height = resSkeletons->getDouble("height");        
        for (int i = 0; i < Num_Angles; i++) {
            skel.angles[i] = resSkeletons->getDouble(i+4);
        }   
        skel.normalizeSkel();
        listSkel->push_back(skel);
    }    
}


list<SimpleSkeleton> getDataSkeletons(Connection *con){
    
    list<SimpleSkeleton> listSkel; 
                 
    
    list<int> listIdBehaviours= getListIdBehaviours(con);   
    list<int>::iterator it;
 
    for ( it=listIdBehaviours.begin() ; it != listIdBehaviours.end(); it++ )            
        addSkeletonsFromIdBehaviour(con,*it,&listSkel);       
  
    return listSkel;
} 


SimpleSkeleton* getCopyClusters(SimpleSkeleton copy[Num_Clusters], SimpleSkeleton clusters[Num_Clusters]){
//    SimpleSkeleton copy[Num_Clusters];    
//    SimpleSkeleton* pointer=copy;
    
    for(int i=0;i<Num_Clusters;i++){
        copy[i].idCluster=clusters[i].idCluster;
         copy[i].height=clusters[i].height;
            for (int j = 0; j < Num_Angles; j++)
               copy[i].angles[j]= clusters[i].angles[j];
    }
    
//    return pointer;
}

float calcClustersDistance(SimpleSkeleton clusters[Num_Clusters]){
    float distance,actualdist;
    
    distance=999999999999999999;
    
     //DistanciaActual= Encontrar los centroides de Solucion, i y j, tales que la distancia entre i y j sea mínima
    for(int i=0;i<Num_Clusters;i++){
        for(int j=0;j<Num_Clusters;j++){
            if(i<j){
                actualdist= clusters[i].distance(clusters[j]);                
                if(actualdist<distance)
                    distance=actualdist;
            }
        }
    }    
    
    return distance;
}


bool equalClusters(SimpleSkeleton clust1[Num_Clusters],SimpleSkeleton clust2[Num_Clusters]){
    
    for(int i=0;i<Num_Clusters;i++){
        if(clust1[i].distance(clust2[i])!=0)
            return false;
    }
    return true;
}

void calcKmeans(Connection *con, SimpleSkeleton clusters[Num_Clusters], list<SimpleSkeleton> *dataSkeletons){  
    
    SimpleSkeleton copyClusters[Num_Clusters];
    getCopyClusters(copyClusters,clusters);    
    
    int count[Num_Clusters];

    list<SimpleSkeleton>::iterator it;
    float minDistance;
    int idCluster;
         
    while(true){//we loop until it converges        
        //Now we start Kmeans by placing each skeleton on one cluster    
        for (it = dataSkeletons->begin(); it != dataSkeletons->end(); it++) {
             minDistance = 999999999999999999;//init
            for (int i = 0; i < Num_Clusters; i++) {
                float distance = clusters[i].distance(*it);
                if (distance < minDistance) {
                    minDistance = distance;
                    idCluster = clusters[i].idCluster;
                }
            }
            it->idCluster = idCluster;           
        }

        //initializing the count
        for (int i = 0; i < Num_Clusters; i++) {
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
        for (int i = 0; i < Num_Clusters; i++) {
            if (count[i] != 0) {
                clusters[i].height /= count[i];
                for (int j = 0; j < Num_Angles; j++)
                    clusters[i].angles[j] /= count[i];
            }
        }
        if (equalClusters(copyClusters, clusters))
            break;//then we got the convergence and we stop;
        
         getCopyClusters(copyClusters,clusters);
    }       
    
}


int main(int argc, const char *argv[]) {

    Driver *driver;
    Connection *con;
    Savepoint *savept;     
    
    /* initiate url, user, password and database variables */
    string url(argc >= 2 ? argv[1] : DBHOST);
    const string user(argc >= 3 ? argv[2] : USER);
    const string password(argc >= 4 ? argv[3] : PASSWORD);
    const string database(argc >= 5 ? argv[4] : DATABASE);

    try {
        driver = get_driver_instance();

        /* create a database connection using the Driver */
        con = driver -> connect(url, user, password);

        /* alternate syntax using auto_ptr to create the db connection */
        //auto_ptr  con (driver -> connect(url, user, password));

        /* turn off the autocommit */
        con -> setAutoCommit(0);

        /* select appropriate database schema */
        con -> setSchema(database);
        savept = con -> setSavepoint("SAVEPT1");


        /***********************START ALGORITHM***************************************************/
        
       SimpleSkeleton clusters[Num_Clusters];       
       SimpleSkeleton bestClusters[Num_Clusters];       
       float clusterDistance=-99999999999999999;
       float actualDistance;
       
       //we get the data we are going to use for the whole training
       list<SimpleSkeleton> dataSkeletons = getDataSkeletons(con);
       
         for(int iterations=0;iterations<Max_Iterations;iterations++){ //multistart
            calcInitialClusters(con,clusters);//for initial execution            
            calcKmeans(con,clusters,&dataSkeletons);
            actualDistance=calcClustersDistance(clusters);
            
            if(actualDistance>clusterDistance){
                getCopyClusters(bestClusters,clusters);            
                clusterDistance=actualDistance;
                cout<<"Better clusters found, distance: "<<clusterDistance<<endl;
            }
          }//end iterations MultiStart

              
        cout<<"Saving the best solution in the db"<<endl;
        //now we save the final clusters in DB
        for (int i = 0; i < Num_Clusters; i++) {
            saveSkelClusters(con, bestClusters[i].idCluster, bestClusters[i], Num_Clusters);
        }
        /***********************END ALGORITHM******************************************************/

        con -> releaseSavepoint(savept);
        cout << "\tCommitting outstanding updates to the database .." << endl;
        con -> commit();

        /* Clean up */
        con -> close();
        delete con;

    } catch (SQLException &e) {
        cout << "ERROR: SQLException in " << __FILE__;
        cout << " (" << __func__ << ") on line " << __LINE__ << endl;
        cout << "ERROR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << ")" << endl;

        cout << "\tRolling back until the last save point \"SAVEPT1\" .." << endl;
        con -> rollback(savept);

        if (e.getErrorCode() == 1047) {
            /*
            Error: 1047 SQLSTATE: 08S01 (ER_UNKNOWN_COM_ERROR)
            Message: Unknown command
             */
            cout << "\nYour server does not seem to support Prepared Statements at all. ";
            cout << "Perhaps MYSQL < 4.1?" << endl;
        }

        return EXIT_FAILURE;
    } catch (std::runtime_error &e) {

        cout << "ERROR: runtime_error in " << __FILE__;
        cout << " (" << __func__ << ") on line " << __LINE__ << endl;
        cout << "ERROR: " << e.what() << endl;

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
} // main()
