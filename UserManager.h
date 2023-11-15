#ifndef _USERMANAGER_H
#define _USERMANAGER_H

#include "common/macro.h"
#include "UserInfo.h"
#include "DbManager.h"

class UserManager {
private:
    UserInfo users_[10240];
    GETSETVAR(int, user_count)
    GETSETVAR(int, cur_user_id)

private:    
    DbManager *db_svr_;

public:
    int Start(DbManager *db_svr);
    int Proc();
    int Restart();
    int Shutdown();

public:
    UserInfo *GetUser(int user_id);
    int CheckExist(int user_id);
    int CreateUser(const char *user_name, const char *password, int from, int reg_time);
    int DeleteUser(int user_id);
    int SaveUser(int user_id);
    int SaveUser();//todo
    int LoginCheck(const char *user_name, const char *password);
    int UserLogout(int user_id, int time_now);
    int GetUserIdByUserName(const char *user_name);
    int UpdateLoginTime(int user_id, int time_now);
    // int UpdateUserLogoutime(int user_id, int time_now);
    // int UpdateUserFreshTime(int usr_id,int time_now);
}; 

#endif