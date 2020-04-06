//Adapted from:
// Logic Supply Inc               
// Workshop and Tutorials       visit   inspire.logicsupply.com
// Temperature Sensor Demo Program for the BeagleBone Black ARM Cortex A8
// Author      : Richard St-Pierre
// Sensor Type : Semitec 103AT-2
// Version     : 0.0   May 22, 2014  Initial creation
//
#include <math.h>
#include <stdio.h>
#include <float.h>

	float adcValue;//  = 0.00;           // Analog input  (0-100)   
	float r_bias    = 10000;          // Bias resistor (to GND)
	float r_therm   = 0;              // Calculated thermistor resistance (to Vref)
	float t_c       = 0;              // Temperature (Celsius)
	float r_alpha   = 0.09919;        // Ro * e^(-B/To)  in K, To= 298K
	float c2kelvin  = 273.15;         // Celsius to Kelvin offset
	float Bvalue    = 3435;           // from thermistor datasheet


int main(){

	while(1){
	printf("Type in the number\n");
	scanf("%f", &adcValue);
	printf("adcValue %f\n", adcValue);
	r_therm = abs((((1/adcValue)-1)*r_bias));
	printf("r_therm %f\n", r_therm);
	t_c = ((10*((Bvalue/(log(r_therm/r_alpha)))-c2kelvin))/10);
	printf("adcValue = %f and TempC = %f\n\n", adcValue, t_c);
	}

}
