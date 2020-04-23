#include "pwm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char *pwm_pin = "P9.14";
const char *pwm_path = "/sys/devices/platform/ocp/48302000.epwmss/48302200.pwm/pwm/pwmchip4/pwm-4:0";


struct pwm_context{
    const char* pin;
    const char* directory;
    FILE *duty_cycle;
    FILE *period;
};

// run the config-pin command to configure an IO pin as pwm
void config_pin(){
    char buf[128];
    snprintf(buf, sizeof(buf), "config-pin %s", pwm_pin);
    FILE *s = popen(buf, "r");
    pclose(s);
}

// setup an IO pin as PWM and create a pwm_context struct that can be
// used to control the PWM parameters
struct pwm_context* create_pwm_context(){
    FILE* enable = NULL;
    config_pin();

    // allocate the context
    struct pwm_context *ctx = malloc(sizeof(*ctx));
    ctx->pin = pwm_pin;
    ctx->directory = pwm_path;
    char buf[1024];
    // create the filename for duty_cycle and open the file
    snprintf(buf, sizeof(buf), "%s/duty_cycle", ctx->directory);
    ctx->duty_cycle = fopen(buf, "w");
    if(!ctx->duty_cycle) goto err; // goto is occasionally useful

    // create the filename for period and open the file
    snprintf(buf, sizeof(buf), "%s/period", ctx->directory);
    ctx->period = fopen(buf, "w");
    if(!ctx->period) goto err;

    // open the PWM pin's enable file
    snprintf(buf, sizeof(buf), "%s/enable", ctx->directory);
    enable = fopen(buf, "w");
    if(!enable) goto err;
    // write a 1 to the enable file to enable the pin
    
    fprintf(enable, "1");
    fclose(enable);


    return ctx;

 err:
    // If we encountered an error, clean up
    if(ctx->duty_cycle) fclose(ctx->duty_cycle);
    if(ctx->period) fclose(ctx->period);
    if(enable) fclose(enable);
    free(ctx);
    return NULL;
}

// set the PWM period from a given context
void set_period(struct pwm_context* ctx, int period){
    ftruncate(fileno(ctx->period), 0);
    if(ctx->period){
        fprintf(ctx->period, "%d", period * PWM_MULTIPLIER);
        rewind(ctx->period);
    }
}

// set the PWM duty cycle from a given context
void set_duty_cycle(struct pwm_context* ctx, int duty){
    ftruncate(fileno(ctx->duty_cycle), 0);
    if(ctx->duty_cycle){
        fprintf(ctx->duty_cycle, "%d", duty * PWM_MULTIPLIER);
        rewind(ctx->duty_cycle);
    }
}
