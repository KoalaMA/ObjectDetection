#include <datamanagement.h>

CMyDBAdd::CMyDBAdd()
{
    //初始化连接数据库变量
    connection = mysql_init(NULL);
    if(connection == NULL)
    {
        perror("mysql_init");
        exit(1);
    }
}

CMyDBAdd::~CMyDBAdd()
{
    //关闭初始化连接数据库变量
    if(connection != NULL)
    {
        mysql_close(connection);
//        delete connection;
    }
}
//初始化数据库 数据库连接
bool CMyDBAdd::initDB(string server_host , string user, string password , string db_name )
{
    //运用mysql_real_connect函数实现数据库的连接
    connection = mysql_real_connect(connection , server_host.c_str() , user.c_str() , password.c_str() , db_name.c_str() , 0 , NULL , 0);
    if(connection == NULL)//connect failed
      {
        perror("mysql_real_connect");
        exit(1);
      }
    else { //connect sucessfully,and then ,we can write all the matrix into table

      }
    return true;
}
//执行SQL语句
bool CMyDBAdd::executeSQL(string sql_str)
{
    // 查询编码设定
    if(mysql_query(connection, "set names utf8"))
    {
        fprintf(stderr, "%d: %s\n",mysql_errno(connection), mysql_error(connection));
    }
    int t = mysql_query(connection,  sql_str.c_str());
    if(t)
    {
        printf("Error making query: %s\n" , mysql_error(connection));
        exit(1);
    }
    else
    {
        //初始化逐行的结果集检索
        res = mysql_use_result(connection);
        if(res)
        {
            //mysql_field_count(connection)   返回作用在连接上的最近查询的列数
            for(int i = 0 ; i < mysql_field_count(connection) ; i++)//traversing all the record
            {
                //检索一个结果集合的下一行
                row = mysql_fetch_row(res);
                if(row <= 0)
                {
                    break;
                }
                //mysql_num_fields(res)  函数返回结果集中字段的数
                for(int r = 0 ; r  < mysql_num_fields(res) ; r ++)
                {
                    printf("%s\t" , row[r]);
                }
                printf("\n");
            }
        }
        //释放结果集使用的内存
        mysql_free_result(res);
    }
    return true;
}
//表的创建
bool CMyDBAdd::create_table(string table_str_sql)
{
    int t = mysql_query(connection , table_str_sql.c_str());
    if(t)
    {
        printf("Error making query: %s\n" , mysql_error(connection));
        exit(1);
    }
    return true;
}

CMyDBQuery::CMyDBQuery()
{
  driver = get_driver_instance();
  connection = driver->connect("localhost","root","shilei744534");
  statement = connection->createStatement();

  statement->execute("SET NAMES utf8");
  statement->execute("use convert_matrix");
}

CMyDBQuery::~CMyDBQuery()
{
  connection->close();
  delete statement;
  delete connection;

}

ResultSet *CMyDBQuery::QueryRecord(string sql)
{
  result_set = statement->executeQuery(sql.c_str());
}
