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
// The target temperature the PID controller tries to obtain
#define DT_TARGET 1.5

#define PWM_STEP 4
#define PWM_PERIOD 1000

volatile float mass_flow_rate = 0;
volatile float heater_multiplier = 0;
// mutex used for locking heater_multiplier and mass_flow_rate
pthread_mutex_t pwm_mutex = PTHREAD_MUTEX_INITIALIZER;

float ambient_temp;
int pwm_duty_cycle = 0;

float kp = 150;
float ki = 5;
float kd = 0;

// variables for storing the history of error for the pid controller
static float integral_error = 0;
static float past_error = 0;

// sets the PWM duty cycle from a given temperature difference
void get_pwm_duty_cycle(float absdiff){

    // calculate the error and change in error
    float error = DT_TARGET - absdiff;
    float de = error - past_error;

    // calculate the duty cycle from the proportional, integral, and
    // derivative components
    pwm_duty_cycle = kp * error + ki * integral_error
        + kd * de;

    // print some debugging info about the PID parameters
    printf("P: %f, I: %f, D: %f\n", kp * error,
           ki * integral_error,
           kd * de);

    // Scale the duty cycle to the valid range
    if(pwm_duty_cycle < 0) pwm_duty_cycle = 0;
    if(pwm_duty_cycle > PWM_PERIOD) pwm_duty_cycle = PWM_PERIOD;

    // set the history variables
    integral_error += error;
    past_error = error;
}


// This thread is in charge of controlling the heater PWM and updating
// the mass_flow_rate and heater_period variables
void pwm_thread(void* data){

    // initialize the pwm
    struct pwm_context *ctx =  create_pwm_context();
    set_period(ctx, PWM_PERIOD);
    set_duty_cycle(ctx, 0);

    // wait for the sensor to come to room temperature
    usleep(AMBIENT_TEMP_TIMEOUT);

    while(1){
	// adcthread looks at these variables so they need to be
	// updated in a critical section
        pthread_mutex_lock(&pwm_mutex);
	// calculate the temperature difference
        float t1 = get_temperature(temp1);
        float t2 = get_temperature(temp2);
        float absdiff = fabsf(t1-t2);

	// run the PID algorithm
        get_pwm_duty_cycle(absdiff);
	// set the heater_multiplier variable
        heater_multiplier = (float)pwm_duty_cycle/PWM_PERIOD;

        mass_flow_rate = get_mass_flow(t1, t2, heater_multiplier);
        pthread_mutex_unlock(&pwm_mutex);


	// print some debug info
        printf("t1: %f, t2: %f, dt: %f, duty: %d, flow: %f\n",
               t1, t2, absdiff, pwm_duty_cycle, mass_flow_rate);

	// update the PWM duty cycle
        set_duty_cycle(ctx, pwm_duty_cycle);

        usleep(PWM_THREAD_LOOP_RATE);
    }

}
