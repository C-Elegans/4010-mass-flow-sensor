#include "massflow.h"
#include "ADC.h"
#include <math.h>

#define HEATER_RESISTANCE 40.0
#define HEATER_VOLTAGE 13.8
#define HEATER_POWER ((HEATER_VOLTAGE) * (HEATER_VOLTAGE) / HEATER_RESISTANCE)
#define K 1

#define AngleRef 300.0

const float r_bias = 10e3;
const float vadc = 1.8;

const float r_alpha   = 0.09919;        // Ro * e^(-B/To)  in K, To= 298K
const float c2kelvin  = 273.15;         // Celsius to Kelvin offset
const float Bvalue    = 3435;           // from thermistor datasheet

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
    float cp = (-1.29142e-10*powf(tempk, 3.0)) +
               (3.2309586e-7*powf(tempk, 2.0)) +
               (-4.703434e-5*tempk) +
                0.9917484975;
    return cp;
}

/* Calculating the Mass flow 
This function calculates the Mass flow from a delta T and the heater power
Parameters are the temp1 and temp2 from the thermistors, and the heater duty cycle/period
*/

float get_mass_flow(float temp1, float temp2, float power_multiplier)
{
    float cp = get_heat_capacity(temp1);
    float dtemp = temp1 - temp2;
    
    float M=(K*HEATER_POWER * power_multiplier)/(cp*dtemp);

    if(M > 100) M = 100;
    if(M < -100) M = -100;

    return M;
}

// converts a temperature from celcius to kelvin
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

// Calculates the potentiometer angle from the potentiometer adc file
float get_potentiometer(FILE * potentiometer_file){
    float angle = getVoltage(potentiometer_file)*AngleRef/VRef;
    return angle;
}

// calculates the thermistor temperature from the thermistor's resistance
float get_thermistor_temp(float resistance){

    float t_c = (Bvalue/(logf(resistance/r_alpha)))-c2kelvin;
    return t_c;
}

// calculate the thermistor's resistance from the ADC voltage
float get_thermistor_resistance(float voltage){
    float r_therm = r_bias * voltage/(vadc - voltage);
    return r_therm;
}

// initialize the ADC files
void adc_init(void){
    temp1 = setADC(TEMP1_PIN);
    temp2 = setADC(TEMP2_PIN);
    angle = setADC(ANGLE_PIN);
}

// get the temperature of a thermistor from the ADC file
float get_temperature(FILE* thermistor_file){
    float voltage = getVoltage(thermistor_file);
    float resistance = get_thermistor_resistance(voltage);
    float temp = get_thermistor_temp(resistance);
    return temp;
}
