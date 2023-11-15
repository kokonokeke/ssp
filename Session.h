#ifndef _SESSION_H
#define _SESSION_H

class Session {
public:
    char user_name[256];     
    char password[256];
    int from; 
    int cur_user_id;
    int collect_count;
};

#endif