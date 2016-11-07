/* 
 * File:   MySQLConnector.h
 * Author: olmin
 *
 * Created on 22 de septiembre de 2011, 14:51
 */

#ifndef MYSQLCONNECTOR_H
#define	MYSQLCONNECTOR_H

#include "behaviour.h"
/* MySQL Connector/C++ specific headers */
#include <cppconn/driver.h>

class MySQLConnector {
public:
    MySQLConnector();
    MySQLConnector(const MySQLConnector& orig);
    virtual ~MySQLConnector();
    void save(Behaviour *behaviour,std::istream * snapshot);   
    void closeConnection();
private:
      sql::Driver *driver;	
};

#endif	/* MYSQLCONNECTOR_H */

