/* 
 * File:   MySQLConnector.cpp
 * Author: olmin
 * 
 * Created on 22 de septiembre de 2011, 14:51
 */

#include "MySQLConnector.h"

/* Standard C++ headers */
#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <stdexcept>
#include <sys/time.h>

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
#define DATABASE "behaviours_detection_db"

#define NUMOFFSET 100
#define COLNAME 200

using namespace std;
using namespace sql;

MySQLConnector::MySQLConnector() {

    try {
        driver = get_driver_instance();

        /* alternate syntax using auto_ptr to create the db connection */
        //auto_ptr  con (driver -> connect(url, user, password));

    } catch (SQLException &e) {
        cout << "ERROR: SQLException in " << __FILE__;
        cout << " (" << __func__ << ") on line " << __LINE__ << endl;
        cout << "ERROR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << ")" << endl;

        if (e.getErrorCode() == 1047) {
            /*
            Error: 1047 SQLSTATE: 08S01 (ER_UNKNOWN_COM_ERROR)
            Message: Unknown command
             */
            cout << "\nYour server does not seem to support Prepared Statements at all. ";
            cout << "Perhaps MYSQL < 4.1?" << endl;
        }

        //return EXIT_FAILURE;
    } catch (std::runtime_error &e) {

        cout << "ERROR: runtime_error in " << __FILE__;
        cout << " (" << __func__ << ") on line " << __LINE__ << endl;
        cout << "ERROR: " << e.what() << endl;

        //return EXIT_FAILURE;
    }
    //return EXIT_SUCCESS;
}

MySQLConnector::MySQLConnector(const MySQLConnector& orig) {
}

MySQLConnector::~MySQLConnector() {
}

/*!Returns the next unique id to be inserted in the db's row*/
static int getNextId(Connection *con){
    ResultSet *res;
    Statement *stmt;      
     
    /* create a statement object */
    stmt = con -> createStatement();
    /* run a query which returns exactly one result set */
    res = stmt -> executeQuery ("SELECT * FROM Behaviour");
    int lastId= res -> rowsCount();     
     
    //cleanup
    delete res;
    delete stmt;
        
    return lastId+1;        
}


void MySQLConnector::save(Behaviour *behaviour, std::istream * snapshot) {

    Connection *con=NULL;   
    PreparedStatement *prep_stmt=NULL;
    Savepoint *savept=NULL;

    int updatecount = 0;
    char dateBuffer [22];
    
    try {
        /* create a database connection using the Driver */
        con = driver -> connect(DBHOST, USER, PASSWORD);
        
        /* select appropriate database schema */
	con -> setSchema(DATABASE);
        
        int idNewBehaviour= getNextId(con);

        /* turn off the autocommit */
        con -> setAutoCommit(false);          
        cout << "\t Autocommit false .." << endl;             
        
        cout << "\tCreating a save point \"SAVEPT1\" .." << endl;
        savept = con -> setSavepoint("SAVEPT1");
               
        prep_stmt = con -> prepareStatement(
                "INSERT INTO `Behaviour` (`id`, `name`, `date_ini`, `date_end`, `frames`,`snapshot`,`comments`,`user`) "
                "VALUES (?, ?, ?, ?, ?, ?, ?, ?);");
        
        prep_stmt -> setInt(1,idNewBehaviour); 
        prep_stmt -> setString(2, behaviour->name);
        
        strftime (dateBuffer,22,"%Y-%m-%d %H:%M:%S",localtime (& behaviour->dateIni)); //YYYY-MM-DD HH:MM:SS.microssec        
        prep_stmt -> setDateTime(3, dateBuffer);
              
        strftime (dateBuffer,22,"%Y-%m-%d %H:%M:%S",localtime (& behaviour->dateEnd)); //YYYY-MM-DD HH:MM:SS.microssec       
        prep_stmt -> setDateTime(4, dateBuffer);
        prep_stmt -> setInt(5, behaviour->count);
        prep_stmt -> setBlob(6, snapshot);
        prep_stmt -> setString(7, behaviour->comments);
        prep_stmt -> setInt(8, behaviour->user);
        
        cout << "\t Execute update.." << endl;           
        updatecount = prep_stmt -> executeUpdate();
 
        prep_stmt = con -> prepareStatement(
                //"INSERT INTO `Skeleton` (`id_behaviour`, `frame`, `pos3D`, `ang0`, `ang1`, `ang2`, `ang3`, `ang4`, `ang5`, `ang6`, `ang7`, `ang8`, `ang9`, `ang10`) "
                "INSERT INTO `Skeleton`(`id_behaviour`, `frame`, `head`, `neck`, `torso`, `left_shoulder`, `left_elbow`, `left_hand`, `left_knee`, `left_foot`, `left_hip`, `right_shoulder`, `right_elbow`, `right_hand`, `right_knee`, `right_foot`, `right_hip`) "
                "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"
                );
        
        
         //inserting the skelton for every frame        
        for (int i = 0; i < behaviour->count;i++) {        
                    
            prep_stmt -> setInt(1, idNewBehaviour);
            prep_stmt -> setInt(2, i+1);            
            prep_stmt -> setString(3, point3D2String(behaviour->positions[i].joints[0]));
            prep_stmt -> setString(4, point3D2String(behaviour->positions[i].joints[1]));
            prep_stmt -> setString(5, point3D2String(behaviour->positions[i].joints[2]));
            prep_stmt -> setString(6, point3D2String(behaviour->positions[i].joints[3]));
            prep_stmt -> setString(7, point3D2String(behaviour->positions[i].joints[4]));
            prep_stmt -> setString(8, point3D2String(behaviour->positions[i].joints[5]));
            prep_stmt -> setString(9, point3D2String(behaviour->positions[i].joints[6]));
            prep_stmt -> setString(10,point3D2String( behaviour->positions[i].joints[7]));
            prep_stmt -> setString(11,point3D2String( behaviour->positions[i].joints[8]));
            prep_stmt -> setString(12,point3D2String( behaviour->positions[i].joints[9]));
            prep_stmt -> setString(13,point3D2String( behaviour->positions[i].joints[10]));            
            prep_stmt -> setString(14,point3D2String( behaviour->positions[i].joints[11]));
            prep_stmt -> setString(15,point3D2String( behaviour->positions[i].joints[12]));
            prep_stmt -> setString(16,point3D2String( behaviour->positions[i].joints[13]));
            prep_stmt -> setString(17,point3D2String( behaviour->positions[i].joints[14]));
               
           updatecount = prep_stmt -> executeUpdate();
           
        }      
        
        cout << "\tCommitting outstanding updates to the database .." << endl;
        con -> commit();

        /* Clean up */      
        delete prep_stmt;
        con -> close();
        delete con;

    } catch (SQLException &e) {
        if (con != NULL && savept!= NULL) {
            cout << "\tRolling back until the last save point \"SAVEPT1\" .." << endl;
            con -> rollback(savept);
            con -> releaseSavepoint(savept);              
            cout << "\t Rollback done.." << endl;
        }

        cout << "ERROR: SQLException in " << __FILE__;
        cout << " (" << __func__ << ") on line " << __LINE__ << endl;
        cout << "ERROR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << ")" << endl;

        if (e.getErrorCode() == 1047) {
            /*
            Error: 1047 SQLSTATE: 08S01 (ER_UNKNOWN_COM_ERROR)
            Message: Unknown command
             */
            cout << "\nYour server does not seem to support Prepared Statements at all. ";
            cout << "Perhaps MYSQL < 4.1?" << endl;
        }

        //return EXIT_FAILURE;
    } catch (std::runtime_error &e) {

        cout << "ERROR: runtime_error in " << __FILE__;
        cout << " (" << __func__ << ") on line " << __LINE__ << endl;
        cout << "ERROR: " << e.what() << endl;

        //return EXIT_FAILURE;
    }
    // return EXIT_SUCCESS;
}

 
void MySQLConnector::closeConnection() {

}