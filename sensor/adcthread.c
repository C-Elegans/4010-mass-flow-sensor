#include "adcthread.h"
#include "analyzer.h"
#include "massflow.h"
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>

void *AdcSendData(void* arg){
    int *new_socket = (int*) arg;

    while(1){
	if(start){
	    float t1 = get_temperature(1);

	    float t2 = get_temperature(2);

	    float mass = get_mass_flow(t1, t2, 1.0);
	    char buf[1024];

	    snprintf(buf, sizeof(buf), "%f\n", mass);
	    send(*new_socket, buf, strlen(buf), 0);
	    printf("mass flow %f\n", mass);
	}
	usleep(delay);
    }
}
