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

	    float t1 = get_temperature(temp1);
	    float t2 = get_temperature(temp2);
	    float dt = t2-t1;

	    snprintf(buf, sizeof(buf), "%f %f\n", dt, mass_flow_rate);
	    send(*new_socket, buf, strlen(buf), 0);
	    //printf("mass flow %s\n", buf);
	}
	usleep(delay);
    }
}
