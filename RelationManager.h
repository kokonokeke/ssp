#ifndef _RELATIONMANAGER_H
#define _RELATIONMANAGER_H

#include"RelationInfo.h"

class RelationManager {
private:
    RelationInfo relations_[10240];
    GETSETVAR(int, relation_count)

public:
    int Start();
    int Proc();
    int Restart();
    int Shutdown();

public:
    int UserRelationInit(int user_id);
    RelationInfo *GetRelation(int user_id);
    int AddFriend(int user_id, int other_id);
    int DelFriend(int user_id, int other_id);
    //AddBlack
    //DelBlack
};

#endif