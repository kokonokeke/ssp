#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BusManager.h"

int BusManager::Init() {
    //shm Init
    printf("---- Init ----\n");
    shm_id = shmget((key_t)8998, 10240 * 8, IPC_CREAT | 0666);
    if (shm_id < 0) {
        printf("shmget failed: %d\n", shm_id);
        return -1;
    }
    printf("---- shm_id:%d ----\n", shm_id);
    shmp = (char *)shmat(shm_id, NULL, 0);
    if (*shmp < 0) {
        printf("shmat failed: %d\n", *shmp);
        return -2;
    }
    printf("---- shmat_addr:%p ----\n", shmp);
    if(*shmp == 'Y') {
        BusInit();
    } else {
        *shmp = 'Y';
        BusInit();
        ChannelInit();
    }
    return 0;
}

int BusManager::ShmDetach() {
    int ret = shmdt(shmp);
    return 0;
}

int BusManager::ShmRemove() {
    int ret = shmctl(shm_id, IPC_RMID, 0);
    return 0;
}

int BusManager::BusInit() {
    char *curp = shmp + 1;
    int size_channel = sizeof(Channel);
    int size_block = sizeof(BusBlock);
    printf("channel(%d) block(%d)\n", size_channel, size_block);

    for (int i = 0; i < 4; i++) {
        channel_[i] = (Channel *)curp;
        curp += size_channel;
    }

    // channel_[0] = (Channel *)curp;
    // curp += size_channel;
    // channel_[1] = (Channel *)curp;
    // curp += size_channel;
    // channel_[2] = (Channel *)curp;
    // curp += size_channel;
    // channel_[3] = (Channel *)curp;
    // curp += size_channel;

    // channel_[0]->name = curp;
    // curp += 256;

    // channel_[0]->from = (int *)curp;
    // curp += sizeof(int);

    // channel_[0]->to = (int *)curp;
    // curp += sizeof(int);

    // channel_[0]->block = (BusBlock *)curp;
    // curp += sizeof(BusBlock);//not star

    // channel_[0]->block->used = curp;
    // curp++;

    // channel_[0]->block->next = (BusBlock *)curp;
    // curp += sizeof(BusBlock *);
    
    // channel_[0]->block->size = (int *)curp;
    // curp += sizeof(int);

    // channel_[0]->block->data = curp;
    // curp += 10240;

    for (int i = 0; i < 4; i++) {
        channel_[i]->name = curp;
        curp += 256;

        channel_[i]->from = (int *)curp;
        curp += sizeof(int);

        channel_[i]->to = (int *)curp;
        curp += sizeof(int);

        channel_[i]->block = (BusBlock *)curp;
        curp += sizeof(BusBlock);//not star

        channel_[i]->block->used = curp;
        ++curp;

        channel_[i]->block->next = (BusBlock *)curp;
        curp += sizeof(BusBlock *);
        
        channel_[i]->block->size = (int *)curp;
        curp += sizeof(int);

        channel_[i]->block->data = curp;
        curp += 10240;
    }

    return 0;
}

int BusManager::ChannelInit() {
    strncpy(channel_[0]->name, "User1 -> Server", 15);
    *(channel_[0]->from) = 11;
    *(channel_[0]->to) = 21;
    *(channel_[0]->block->used) = 'N';
    *(channel_[0]->block->size) = 0;
    memset(channel_[0]->block->data, 0, 10240);

    strncpy(channel_[1]->name, "User2 -> Server", 15);
    *(channel_[1]->from) = 12;
    *(channel_[1]->to) = 21;
    *(channel_[1]->block->used) = 'N';
    *(channel_[1]->block->size) = 0;
    memset(channel_[1]->block->data, 0, 10240);

    strncpy(channel_[2]->name, "Server -> User1", 15);
    *(channel_[2]->from) = 21;
    *(channel_[2]->to) = 11;
    *(channel_[2]->block->used) = 'N';
    *(channel_[2]->block->size) = 0;
    memset(channel_[2]->block->data, 0, 10240);

    strncpy(channel_[3]->name, "Server -> User2", 15);
    *(channel_[3]->from) = 21;
    *(channel_[3]->to) = 12;
    *(channel_[3]->block->used) = 'N';
    *(channel_[3]->block->size) = 0;
    memset(channel_[3]->block->data, 0, 10240);
    return 0;
}

int BusManager::ChannelShow() {
    for (int i = 0; i < 4; i++) {
        printf("Channel[%d].name:%s\n", i, channel_[i]->name);
        printf("Channel[%d].from:%d\n", i, *(channel_[i]->from));
        printf("Channel[%d].to  :%d\n", i, *(channel_[i]->to));
        printf("Channel[%d].used:%c\n", i, *(channel_[i]->block->used));
        printf("Channel[%d].size:%d\n", i, *(channel_[i]->block->size));
        printf("Channel[%d].data:%s\n", i, channel_[i]->block->data);
    }
    return 0;
}

int BusManager::CheckRecv(int target_id) {
    for (int i = 0; i < 4; i++) {
        if (*(channel_[i]->to) == target_id) {
            if (*(channel_[i]->block->used) == 'Y') {
                return *(channel_[i]->from);
            }
        }
    }
    return -1;
}

Channel *BusManager::GetChannel(int from, int to) {
    for (int i = 0; i < 4; i++) {
        if (*(channel_[i]->from) == from && *(channel_[i]->to) == to) {
            return channel_[i];
        }
    }
    return NULL;
}

char *BusManager::Recv(int from, int to) {
    Channel *pchannel = GetChannel(from, to);
    if (pchannel != NULL) {
        if (*(pchannel->block->used) == 'Y') {
            *(pchannel->block->used) = 'N';
            return pchannel->block->data;
        }
    }
    return NULL;
}

int BusManager::RecvSize(int from, int to) {
    Channel *pchannel = GetChannel(from, to);
    if (pchannel != NULL) {
        return *(pchannel->block->size);
    }
    return 0;
}

int BusManager::Clear(int from, int to) {
    Channel *pchannel = GetChannel(from, to);
    if (pchannel != NULL) {
        if (*(pchannel->block->used) == 'N') {
            *(pchannel->block->size) = 0;
            memset(pchannel->block->data, 0, *(pchannel->block->size));
            return 0;
        }
    }
    return -1;
}

int BusManager::Send(int from, int to, char *data, int size) {
    Channel *pchannel = GetChannel(from, to);
    if (pchannel != NULL && *(pchannel->block->used) == 'N') {
        *(pchannel->block->size) = size;
        strncpy(pchannel->block->data, data, size);
        *(pchannel->block->used) = 'Y';
        return 0;
    }
    return -1;
}