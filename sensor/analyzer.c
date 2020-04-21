#include "analyzer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

long start = 0;
useconds_t delay = 100000;

void *AnalyzerReceive(void *arg){
    int *new_socket = arg;
    char buf[1024];
    long bytes;
    while(((bytes = read(*new_socket, buf, sizeof(buf)))) > 0){
        printf("%s\n", buf);
        if(strncmp(buf, "start", strlen("start")) == 0){
            printf("Starting!\n");
            start = 1;
        }
        else if(strncmp(buf, "delay", strlen("delay")) == 0){
            printf("Setting Delay!\n");
            char *ptr = buf + strlen("delay ");
            delay = (useconds_t)(atoi(ptr) * 1000);
            printf("Delay: %d\n", delay);
        }
    }
    return NULL;
}


