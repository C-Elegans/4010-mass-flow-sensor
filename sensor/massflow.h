#include <stdio.h>
float tokelvin(float);

float get_mass_flow(float temp1, float temp2);

float get_temperature(FILE* thermistor_file);

void adc_init(void);

extern FILE* temp1;
extern FILE* temp2;
extern FILE* angle;
