#include "massflow.h"
#include "ADC.h"
#include <math.h>

#define HEATER_RESISTANCE 40.0
#define HEATER_VOLTAGE 13.8
#define HEATER_POWER ((HEATER_VOLTAGE) * (HEATER_VOLTAGE) / HEATER_RESISTANCE)
#define K 1

/* Calculating the Heat capacities of air 
This function calculates the Heat Capacities of Air (Cp).
Made a statistical plot, based on the heat capacities or air table at
https://www.ohio.edu/mechanical/thermo/property_tables/air/air_Cp_Cv.html.
Found the best fitted cubic function according to it's data.
Temperature needs to be in Celcius
*/

float get_heat_capacity(float temp)
{
    float tempk = tokelvin(temp);
    float cp = ((-1.29142e-10)*powf(tempk, 3.0)) +
	       ((3.2309586e-7)*powf(tempk, 2.0)) +
	       (-4.703434e-5*tempk) +
	        0.9917484975;
    return cp;
}

/* Calculating the Mass flow 
This function calculates the Mass flow.
Parameters are the temp1 and temp2 from the thermistors, and the heat capacity of the air.
*/

float get_mass_flow(float temp1, float temp2)
{
    float cp = get_heat_capacity(temp1);
    float dtemp = temp1 - temp2;
    
    float M=(K*HEATER_POWER)/(cp*dtemp);

    return M;
}

float tokelvin(float tempc){
    return tempc + 273.15;
}

/* Spencer, you want to do these? you should be able to adapt the code you have from your 4020 lab for these. IDK how you connected yours, but the ones on our tube are connected like so:
   1.8V
     <
     > - 10k resistor
     <
     O--- to BBB ADC
     <
     > - NTC thermistor
     <
     GND
 */

FILE* temp1 = NULL;
FILE* temp2 = NULL;
FILE* angle = NULL;

__attribute__((weak))
float get_thermistor_temp(float resistance){

    return 0.0;
}

__attribute__((weak))
float get_thermistor_resistance(float voltage){
    return 0.0;
}

void adc_init(void){
    temp1 = setADC(TEMP1_PIN);
    temp2 = setADC(TEMP2_PIN);
    angle = setADC(ANGLE_PIN);
}

float get_temperature(FILE* thermistor_file){
    float voltage = getVoltage(thermistor_file);
    float resistance = get_thermistor_resistance(voltage);
    float temp = get_thermistor_temp(resistance);
    return temp;
}
