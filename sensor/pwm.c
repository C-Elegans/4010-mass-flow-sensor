#include "pwm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *pwm_pin = "P9.14";
const char *pwm_path = "/sys/devices/platform/ocp/48302000.epwmss/48302200.pwm/pwm/pwmchip4/pwm-4:0";


struct pwm_context{
    const char* pin;
    const char* directory;
    FILE *duty_cycle;
    FILE *period;
};

struct pwm_context* init_pwm(){
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
void set_period(struct pwm_context* ctx, int period){}
void set_duty_cycle(struct pwm_context* ctx, int duty){}
