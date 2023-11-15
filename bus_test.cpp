#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BusManager.h"

BusManager bus, bus2;

int main() {
    bus.Init();
    bus2.Init();
    int test_on = 1;
    int from, to;
    char message[256];
    do {
        scanf("%d %d %s", &from, &to, message);
        if (strcmp(message, "quit") == 0) {
            test_on = 0;
        }
        bus.Send(from, to, message, strlen(message));
        int ret11 = bus2.CheckRecv(11);
        int ret12 = bus2.CheckRecv(12);
        int ret21 = bus2.CheckRecv(21);
        bus2.ChannelShow();
        if (ret11) {
            printf("[user1 recv]: %s\n", bus2.Recv(ret11, 11));
            bus2.Clear(ret11, 11);
        }
        
        if (ret12) {
            printf("[user2 recv]: %s\n", bus2.Recv(ret12, 12));
            bus2.Clear(ret12, 12);
        }

        if (ret21) {
            printf("[server recv]: %s\n", bus2.Recv(ret21, 21));
            bus2.Clear(ret21, 21);
        }

    } while(test_on);
    bus2.ShmDetach();
    bus.ShmDetach();
    bus2.ShmRemove();
    return 0;
}