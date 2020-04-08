
struct pwm_context;

struct pwm_context* create_pwm_context();
void set_period(struct pwm_context* ctx, int period);
void set_duty_cycle(struct pwm_context* ctx, int duty);
void free_pwm_context(struct pwm_context*);
