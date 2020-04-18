#include "adcthread.h"
#include "analyzer.h"
#include "massflow.h"
#include "pwmthread.h"
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>

void *AdcSendData(void* arg){
    int *new_socket = (int*) arg;

    while(1){
	if(start){

	    char buf[1024];

	    snprintf(buf, sizeof(buf), "%f\n", mass_flow_rate);
	    send(*new_socket, buf, strlen(buf), 0);
	    printf("mass flow %f\n", mass_flow_rate);
	}
	usleep(delay);
    }
}
