#ifndef DATAMANAGEMENT_H

#include <iostream>
#include <stdlib.h>
using namespace std;

#include <mysql/mysql.h>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <mysql_error.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace sql;

#define DATAMANAGEMENT_H


class CMyDBAdd;
class CMyDBQuery;



class CMyDBAdd
{
public:
    CMyDBAdd();
    ~CMyDBAdd();
    bool initDB(string server_host , string user, string password, string db_name);
    bool executeSQL(string sql_str);
    bool create_table(string table_str_sql);

public:
    MYSQL *connection;
private:
    MYSQL_RES *res;
    MYSQL_ROW row;
};

class CMyDBQuery
{
public:
  CMyDBQuery();
  ~CMyDBQuery();

  ResultSet *QueryRecord(string sql);

public:
  Driver *driver;
  Connection *connection;
  Statement *statement;

  ResultSet *result_set;
};

#endif // DATAMANAGEMENT_H
