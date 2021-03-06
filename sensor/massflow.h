#include <stdio.h>
float tokelvin(float);

float get_mass_flow(float temp1, float temp2, float power_multiplier);

float get_potentiometer(FILE * potentiometer_file);

float get_temperature(FILE* thermistor_file);

void adc_init(void);

extern FILE* temp1;
extern FILE* temp2;
extern FILE* angle;

#define TEMP1_PIN 5
#define TEMP2_PIN 3
#define ANGLE_PIN 1
