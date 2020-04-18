#include "pwmthread.h"
#include "pwm.h"
#include "massflow.h"
#include <unistd.h>
#include <math.h>
#define MILLISECONDS 1000
#define SECONDS (1000 * MILLISECONDS)
#define PWM_THREAD_LOOP_RATE (100 * MILLISECONDS)
#define AMBIENT_TEMP_TIMEOUT (5 * SECONDS)

#define PWM_STEP 10
#define PWM_PERIOD 1000

float mass_flow_rate = 0;


float ambient_temp;
int pwm_duty_cycle = 0;
void pwm_thread(void* data){
    struct pwm_context *ctx =  create_pwm_context();
    set_period(ctx, PWM_PERIOD);
    set_duty_cycle(ctx, 0);

    usleep(AMBIENT_TEMP_TIMEOUT);
    ambient_temp = get_temperature(temp1);

    while(1){
	float t1 = get_temperature(temp1);
	float t2 = get_temperature(temp2);
	float absdiff = fabsf(t1-t2);

	pwm_duty_cycle += (2 - absdiff) * PWM_STEP;

	if(pwm_duty_cycle < 0)
	    pwm_duty_cycle = 0;
	if(pwm_duty_cycle > PWM_PERIOD)
	    pwm_duty_cycle = PWM_PERIOD;
				
	mass_flow_rate = get_mass_flow(t1, t2,
				       (float)pwm_duty_cycle/PWM_PERIOD);
	
	set_duty_cycle(ctx, pwm_duty_cycle);
	
	usleep(PWM_THREAD_LOOP_RATE);
    }

}
