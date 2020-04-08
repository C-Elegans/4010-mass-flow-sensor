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
#define DT_TARGET 1.5

#define PWM_STEP 4
#define PWM_PERIOD 1000

volatile float mass_flow_rate = 0;
volatile float heater_multiplier = 0;
pthread_mutex_t pwm_mutex = PTHREAD_MUTEX_INITIALIZER;

float ambient_temp;
int pwm_duty_cycle = 0;

#define KU 200.0
#define PU 18.0

#define T0 ((float)PWM_THREAD_LOOP_RATE/SECONDS)

float kp = 150;
float ki = 5;
float kd = 0;

static float integral_error = 0;
static float past_error = 0;
void get_pwm_duty_cycle(float absdiff){

    float error = DT_TARGET - absdiff;
    float de = error - past_error;

    pwm_duty_cycle = kp * error + ki * integral_error
	+ kd * de; 

    printf("P: %f, I: %f, D: %f\n", kp * error,
	   ki * integral_error,
	   kd * de);

    if(pwm_duty_cycle < 0) pwm_duty_cycle = 0;
    if(pwm_duty_cycle > PWM_PERIOD) pwm_duty_cycle = PWM_PERIOD;
    
    integral_error += error;
    past_error = error;
}


void pwm_thread(void* data){

    struct pwm_context *ctx =  create_pwm_context();
    set_period(ctx, PWM_PERIOD);
    set_duty_cycle(ctx, 0);

    usleep(AMBIENT_TEMP_TIMEOUT);
    ambient_temp = get_temperature(temp1);

    while(1){
	pthread_mutex_lock(&pwm_mutex);
	float t1 = get_temperature(temp1);
	float t2 = get_temperature(temp2);
	float absdiff = fabsf(t1-t2);
	//float mintempdiff = fabsf(fminf(t1, t2) - ambient_temp);
	bool no_flow = false;

	get_pwm_duty_cycle(absdiff);
	heater_multiplier = (float)pwm_duty_cycle/PWM_PERIOD;

	mass_flow_rate = get_mass_flow(t1, t2, heater_multiplier);
	pthread_mutex_unlock(&pwm_mutex);
				       

	printf("t1: %f, t2: %f, dt: %f, duty: %d, flow: %f\n",
	       t1, t2, absdiff, pwm_duty_cycle, mass_flow_rate);
	
	set_duty_cycle(ctx, pwm_duty_cycle);
	
	usleep(PWM_THREAD_LOOP_RATE);
    }

}
