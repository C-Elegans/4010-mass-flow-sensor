#include "massflow.h"

#define HEATER_RESISTANCE 40.0
#define HEATER_VOLTAGE 13.8
#define HEATER_POWER ((HEATER_VOLTAGE) * (HEATER_VOLTAGE) / HEATER_RESISTANCE)

float Kq = 1;
float Cp = 1;
/* Paul, you want to take this one? This should return the mass flow
rate given the temperature of the two sensors in the tube */
float get_heat_capacity(float temp)
{
    return 1.0;
}
    
float get_mass_flow(float temp1, float temp2, float cp)
{
    float dtemp = temp1 - temp2;
    
    float M=Kq/(Cp*dtemp);

    return M;
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


float get_thermistor_temp(float resistance){

    return 0.0;
}

float get_thermistor_resistance(float voltage){
    return 0.0;
}

float get_thermistor_voltage(int thermistor_number){
    return 0.0;
}

float get_temperature(int thermistor_number){
    float voltage = get_thermistor_voltage(thermistor_number);
    float resistance = get_thermistor_resistance(voltage);
    float temp = get_thermistor_temp(resistance);
    return temp;
}
