#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>


int main() {
    int shm_id = shmget((key_t)8998, 10240, IPC_CREAT | 0666);
    if (shm_id < 0) {
        printf("shmget failed: %d\n", shm_id);
        return 0;
    }
    printf("shm_id: %d\n", shm_id);
    char *shmp = (char *)shmat(shm_id, NULL, 0);
    if (*shmp < 0) {
        printf("shmat failed: %d\n", *shmp);
        return 0;
    } 
    printf("shmp_addr=%p\n", shmp);
    while (1) {
        if (strcmp(shmp, "quit") == 0) {
            break;
        }
        if (*shmp == 0) {
            continue;
        }
        printf("get %s\n", shmp);
        memset(shmp, 0, 10240);
        sleep(1);
    }
    return 0;
}