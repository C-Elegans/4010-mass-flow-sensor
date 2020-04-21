#include <stdio.h>
FILE * setADC(int pin);
float getVoltage(FILE * in);
float getAngle(FILE * in);
int closeADC(FILE * in);
