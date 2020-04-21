#include <stdio.h>
FILE * setADC(int pin);
float getVoltage(FILE * in);
int closeADC(FILE * in);
#define VRef 1.8
