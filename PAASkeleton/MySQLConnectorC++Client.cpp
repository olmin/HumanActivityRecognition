/* Standard C++ headers */
#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <stdexcept>

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

/*! CompressionRate for the PAA algorithm*/
#define CompressionRate 4

using namespace std;
using namespace sql;

class SimpleSkeleton{
public:
    SimpleSkeleton(){        
    }

     float height;
     int frame;
     float angles[Num_Angles];
};

void executePAA(Connection *con,int idBehaviour, SimpleSkeleton skelVector[CompressionRate],int count){

     PreparedStatement *prep_stmt;
    prep_stmt = con -> prepareStatement(
            "INSERT INTO `PAASkeleton` (`id_behaviour`, `frame`, `compression_rate`, `height`, `ang0`, `ang1`, `ang2`, `ang3`, `ang4`, `ang5`, `ang6`, `ang7`, `ang8`, `ang9`, `ang10`) "
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"
            );
    
    float height =0;
    for(int i=0;i< count;i++){
        height+=skelVector[i].height;
    }
    height= height/count;
    
    prep_stmt -> setInt(1, idBehaviour);
    prep_stmt -> setInt(2, skelVector[0].frame);
    prep_stmt -> setInt(3, CompressionRate);
    prep_stmt -> setDouble(4,height);
    
    float angle;
    for(int i=0;i<Num_Angles;i++){
        angle=0;
        for(int j=0;j<count;j++){
            angle+=skelVector[j].angles[i];            
        }
        angle=angle/count;
        prep_stmt -> setDouble(i+5,angle);
    }
 
        prep_stmt -> executeUpdate();

}

SimpleSkeleton fillSkeleton(ResultSet *resSkeletons){
    SimpleSkeleton skel;
    string pos3d=resSkeletons->getString("pos3D");
    int indexHeight = pos3d.find_last_of(',')+1;
    int lengthHeight= pos3d.length()-indexHeight-1;
   
    skel.height=  atof(pos3d.substr(indexHeight,lengthHeight).c_str());     
    skel.frame=resSkeletons->getInt("frame");
    skel.angles[0]=resSkeletons->getDouble("ang0");
    skel.angles[1]=resSkeletons->getDouble("ang1");
    skel.angles[2]=resSkeletons->getDouble("ang2");
    skel.angles[3]=resSkeletons->getDouble("ang3");
    skel.angles[4]=resSkeletons->getDouble("ang4");
    skel.angles[5]=resSkeletons->getDouble("ang5");
    skel.angles[6]=resSkeletons->getDouble("ang6");
    skel.angles[7]=resSkeletons->getDouble("ang7");
    skel.angles[8]=resSkeletons->getDouble("ang8");
    skel.angles[9]=resSkeletons->getDouble("ang9");
    skel.angles[10]=resSkeletons->getDouble("ang10");  
    
    return skel;
}


int main(int argc, const char *argv[]) {

	Driver *driver;
	Connection *con;
	Statement *stmt;
	ResultSet *resBehaviours,*resSkeletons;
	PreparedStatement *prep_stmt;
	Savepoint *savept;

	int updatecount = 0;

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

		cout << "\nDatabase connection\'s autocommit mode = " << con -> getAutoCommit() << endl;
                            
		/* select appropriate database schema */
		con -> setSchema(database);

                 cout << "\tCreating a save point \"SAVEPT1\" .." << endl;
		savept = con -> setSavepoint ("SAVEPT1");
                
		/* create a statement object */
		stmt = con -> createStatement();

                /* run a query which returns exactly one result set */
		resBehaviours = stmt -> executeQuery ("SELECT * FROM Behaviour");

		cout << "Retrieving the result set .." << endl;
                
                /* fetch the data : retrieve all the rows in the result set */
                //vars
                int idBehaviour;
                int numFrames;
                SimpleSkeleton skelVector[CompressionRate];
                int index=0;
                
                while (resBehaviours->next()) {
                    idBehaviour= resBehaviours -> getInt("id");
                    numFrames= resBehaviours -> getInt("frames");
                    prep_stmt= con->prepareStatement("SELECT * FROM Skeleton WHERE id_behaviour=?");
                    prep_stmt -> setInt (1, idBehaviour);                                  
                    resSkeletons = prep_stmt -> executeQuery();
                    
                    index=0;
                    while(resSkeletons->next()){                      
                        skelVector[index++]=fillSkeleton(resSkeletons);                        
                        if(index==CompressionRate){
                            executePAA(con,idBehaviour, skelVector,CompressionRate);
                            index=0;
                        }
                    }//end while Skeletons    
                    if(index!=0){//the vector is not complete so we save it
                         executePAA(con,idBehaviour, skelVector,index);
                    }
                } // while Behaviours
                
			
		con -> releaseSavepoint (savept);
		cout << "\tCommitting outstanding updates to the database .." << endl;
		con -> commit();
		

		/* Clean up */
		delete resBehaviours;
                delete resSkeletons;
		delete stmt;
		delete prep_stmt;
		con -> close();
		delete con;

	} catch (SQLException &e) {
		cout << "ERROR: SQLException in " << __FILE__;
		cout << " (" << __func__<< ") on line " << __LINE__ << endl;
		cout << "ERROR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << ")" << endl;

                cout << "\tRolling back until the last save point \"SAVEPT1\" .." << endl;
		con -> rollback (savept);
		
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
