#include "adcthread.h"
#include "analyzer.h"
#include "massflow.h"
#include "pwmthread.h"
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>

/* This function collects the data sent to the network application and sends it at a rate determined by analyzer.c */
void *AdcSendData(void* arg){
    int *new_socket = (int*) arg;

    while(1){
        if(start){

            char buf[1024];

	    /* Get data. Since the PWM thread may change these we need
	    to acquire a lock before reading the variables */
            pthread_mutex_lock(&pwm_mutex);
	    // get temperatures and calculate delta T
            float t1 = get_temperature(temp1);
            float t2 = get_temperature(temp2);
            float dt = t2-t1;
	    // Store the mass flow and heater mult in temporary
	    // variables so the printf doesn't need to be part of the
	    // critical section
            float mf = mass_flow_rate;
            float hm = heater_multiplier;
            pthread_mutex_unlock(&pwm_mutex);

	    //get the potentiometer angle
            float a1 = get_potentiometer(angle); 

	    // format the data as JSON and send it to the network
            snprintf(buf, sizeof(buf), "{\"dt\": %f, \"massflow\": %f, "
                     "\"heater_mult\": %f, \"angle\": %f}\n",
                     dt, mf, hm, a1);
            send(*new_socket, buf, strlen(buf), 0);
            //printf("mass flow %s\n", buf);
        }
        usleep(delay);
    }
}
