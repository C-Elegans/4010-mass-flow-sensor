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

            pthread_mutex_lock(&pwm_mutex);
            float t1 = get_temperature(temp1);
            float t2 = get_temperature(temp2);
            float dt = t2-t1;
            float mf = mass_flow_rate;
            float hm = heater_multiplier;
            pthread_mutex_unlock(&pwm_mutex);
            float a1 = get_potentiometer(angle); //Not sure how to integrate this properly

            snprintf(buf, sizeof(buf), "{\"dt\": %f, \"massflow\": %f, "
                     "\"heater_mult\": %f}\n",
                     dt, mf, hm);
            send(*new_socket, buf, strlen(buf), 0);
            //printf("mass flow %s\n", buf);
        }
        usleep(delay);
    }
}
