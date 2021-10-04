#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <iostream>
#include <mysql.h>
using namespace std;

int qstate;
MYSQL* conn;
MYSQL_ROW row;
MYSQL_RES* res;

class db_response
{
public:
    static void ConnectionFunction()
    {
        //connector initialization
        conn = mysql_init(0);
        if (conn)
            cout << "Database Connected" << endl;
        else
            cout << "Failed To Connect!" << endl;

        conn = mysql_real_connect(conn, "localhost", "root", "", "TourismDB", 3306, NULL, 0);
        if (conn)
            cout << "Database Connected To MySql" << endl;
        else
            cout << "Failed To Connect!" << endl;
    }
};

#endif
