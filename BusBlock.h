#ifndef _BUSBLOCK_H
#define _BUSBLOCK_h

class BusBlock {
public:
    char *used;
    BusBlock *next;
    int *size;
    char *data;
};


#endif 