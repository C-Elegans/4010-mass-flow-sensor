#include "pwmthread.h"
#include "pwm.h"
#include "massflow.h"
#include <unistd.h>
#include <math.h>
#include <stdbool.h>
#define MILLISECONDS 1000
#define SECONDS (1000 * MILLISECONDS)
#define PWM_THREAD_LOOP_RATE (500 * MILLISECONDS)
#define AMBIENT_TEMP_TIMEOUT (5 * SECONDS)
#define DT_TARGET 0.75

#define PWM_STEP 4
#define PWM_PERIOD 1000

float mass_flow_rate = 0;

float ambient_temp;
int pwm_duty_cycle = 0;


float kp = 500.0;
float ki = 0.0;
float kd = 0.0;

static float integral_error = 0;
static float past_error = 0;
void get_pwm_duty_cycle(float absdiff){

    float error = DT_TARGET - absdiff;

    pwm_duty_cycle = kp * error + ki * integral_error
	- kd * past_error; 

    if(pwm_duty_cycle < 0) pwm_duty_cycle = 0;
    if(pwm_duty_cycle > PWM_PERIOD) pwm_duty_cycle = PWM_PERIOD;
    
    integral_error += absdiff;
    past_error = absdiff;
}


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
	//float mintempdiff = fabsf(fminf(t1, t2) - ambient_temp);
	bool no_flow = false;

	get_pwm_duty_cycle(absdiff);

	if(no_flow){
	    mass_flow_rate = 0;
	}
	else{
	    mass_flow_rate = get_mass_flow(t1, t2,
					   (float)pwm_duty_cycle/PWM_PERIOD);
	}

	printf("t1: %f, t2: %f, dt: %f, duty: %d, flow: %f\n",
	       t1, t2, absdiff, pwm_duty_cycle, mass_flow_rate);
	
	set_duty_cycle(ctx, pwm_duty_cycle);
	
	usleep(PWM_THREAD_LOOP_RATE);
    }

}
