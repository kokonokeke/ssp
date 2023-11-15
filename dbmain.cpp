#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DbManager.h"

DbManager db_svr;


int main() {
    db_svr.Init();
    db_svr.execSql("truncate tb_user;");
    db_svr.execSql("insert into tb_user values('10001', 'hank1')");
    db_svr.execSql("insert into tb_user values('10002', 'hank2')");
    db_svr.execSql("insert into tb_user values('10003', 'hank3')");
    db_svr.execSql("select * from tb_user;");
    return 0;
}

// CREATE TABLE tb_user (
//     user_id INT,
//     user_info VARCHAR(10240)
// );