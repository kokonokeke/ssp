#ifndef _DBMANAGER_H
#define _DBMANAGER_H
#include <string>
#include <iostream>
#include "mysql/mysql.h"
#include "UserInfo.h"

class DbManager {
private:
    GETSETVAR(int, transection)

private:
    MYSQL *conn;
    MYSQL_RES *result;
    MYSQL_ROW row;

public:
    int Init();
    int execSql(std::string sql);
    int GetUsersBegin();
    int GetUsersOneByOne(UserInfo *user);
    int GetUsersEnd();
    int GetCurUserId();
    int InsertUser(UserInfo *user);
};

#endif

// CREATE TABLE tb_var (
//     id int(11),
//     key_str VARCHAR(256),
//     value int(11)
// );

// CREATE TABLE tb_user (
//     user_id int(11),
//     user_info varchar(10240)
// );
