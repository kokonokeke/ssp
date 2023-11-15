#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Config.h"

int Config::ReadConfig() {
    FILE *f = fopen("config.ini", "r");
    if (f == NULL) {
        return -1;
    }
    while (1) {
        char config_line[256];
        if (fscanf(f, "%s", config_line) == EOF) {
            break;
        }
        if (strstr(config_line, "socket_port") != NULL) {
            int port;
            sscanf(config_line, "socket_port=%d", &port);
            printf("port:%d\n", port);
            //set_socket_port(port);
            socket_port_ = port;
        }
        if (strstr(config_line, "socket_addr") != NULL) {
            char scaddr[256];
            sscanf(config_line, "socket_addr=%s", scaddr);
            //set_socket_addr(scaddr);
            strncpy(socket_addr_, scaddr, 256);
            printf("addr:%s\n", scaddr);
        }
    }
    fclose(f);
    return 0;
}