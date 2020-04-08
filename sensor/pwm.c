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

void config_pin(){
    char buf[128];
    snprintf(buf, sizeof(buf), "config-pin %s", pwm_pin);
    FILE *s = popen(buf, "r");
    pclose(s);
}

struct pwm_context* create_pwm_context(){
    config_pin();
    
    struct pwm_context *ctx = malloc(sizeof(*ctx));
    ctx->pin = pwm_pin;
    ctx->directory = pwm_path;
    char buf[1024];
    snprintf(buf, sizeof(buf), "%s/duty_cycle", ctx->directory);
    ctx->duty_cycle = fopen(buf, "w");
    if(!ctx->duty_cycle) goto err;

    snprintf(buf, sizeof(buf), "%s/period", ctx->directory);
    ctx->period = fopen(buf, "w");
    if(!ctx->period) goto err;

    return ctx;

 err:
    if(ctx->duty_cycle) fclose(ctx->duty_cycle);
    if(ctx->period) fclose(ctx->period);
    free(ctx);
    return NULL;
}
void set_period(struct pwm_context* ctx, int period){
    ftruncate(fileno(ctx->period), 0);
    printf("set period cycle: %d\n", period * PWM_MULTIPLIER);
    if(ctx->period){
	fprintf(ctx->period, "%d", period * PWM_MULTIPLIER);
	rewind(ctx->period);
    }
}
void set_duty_cycle(struct pwm_context* ctx, int duty){
    ftruncate(fileno(ctx->duty_cycle), 0);
    printf("set duty cycle: %d\n", duty * PWM_MULTIPLIER);
    if(ctx->duty_cycle){
	fprintf(ctx->duty_cycle, "%d", duty * PWM_MULTIPLIER);
	rewind(ctx->duty_cycle);
    }
}
