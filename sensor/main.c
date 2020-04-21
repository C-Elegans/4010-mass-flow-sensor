#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#include "analyzer.h"
#include "adcthread.h"
#include "massflow.h"
#include "pwmthread.h"

#define PORT 8081

pthread_t analyzer_recv, adc_send, pwm_thread_var;

// I (Michael) can fill this in with the networking code to send it to
// the server
int main(int argc, char** argv){

    adc_init();

    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    int server_fd;
    int new_socket;

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("Socket failed");
        exit(-1);
    }

    int opt = 1;

    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
        exit(-1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd, (const struct sockaddr*) &address,
            sizeof(address)) < 0){
        perror("bind");
        exit(-1);
    }

    if(listen(server_fd, 3) < 0){
        perror("listen");
        exit(-1);
    }

    if((new_socket = accept(server_fd, (struct sockaddr*)&address,
                            &addrlen)) < 0){
        perror("accept");
        exit(-1);
    }

    if(pthread_create(&analyzer_recv, NULL, &AnalyzerReceive, &new_socket) != 0){
        perror("error creating thread");
        exit(-1);
    }

    if(pthread_create(&adc_send, NULL, &AdcSendData, &new_socket) != 0){
        perror("error creating thread");
        exit(-1);
    }

    if(pthread_create(&pwm_thread_var, NULL, &pwm_thread, NULL) != 0){
        perror("error creating thread");
        exit(-1);
    }

    while(1){}

    return 1;
}
