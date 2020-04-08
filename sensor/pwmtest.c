#include "pwm.h"
#include <unistd.h>

int main(int argc, char** argv){
    struct pwm_context *pwm = create_pwm_context();

    set_period(pwm, 1000);
    int duty = 0;
    while(1){
	for(duty = 0; duty < 1000; duty+= 10){
	    set_duty_cycle(pwm, duty);
	    usleep(1000 * 20);
	}
	for(; duty > 1; duty-= 10){
	    set_duty_cycle(pwm, duty);
	    usleep(1000 * 20);
	}
    }

}
