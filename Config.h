#ifndef _CONFIG_H
#define _CONFIG_H

#include"common/macro.h"

class Config {
    GETSETVAR(int, socket_port);
    GETSETSTR(256, socket_addr);

public:
    int ReadConfig();
};

#endif