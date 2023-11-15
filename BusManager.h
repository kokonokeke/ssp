#ifndef _BUSMANAGER_H
#define _BUSMANAGER_H
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "Channel.h"
// #include "BusBlock.h"

class BusManager {
private:
    int shm_id;
    char *shmp;
    Channel *channel_[4];

public:
    int Init();
    int BusInit();
    int ChannelInit();
    int ShmDetach();
    int ShmRemove();
    Channel *GetChannel(int from, int to);
    int ChannelShow();
    int CheckRecv(int target_id);
    int RecvSize(int from, int to);
    char *Recv(int from, int to);
    int Clear(int from, int to);
    int Send(int from, int to, char *data, int size);
};

#endif