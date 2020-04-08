#include <pthread.h>


void pwm_thread(void* data);

extern pthread_mutex_t pwm_mutex;
extern volatile float mass_flow_rate;
extern volatile float heater_multiplier;
