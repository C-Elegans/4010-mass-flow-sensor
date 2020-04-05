#include "massflow.h"

#define HEATER_RESISTANCE 40.0
#define HEATER_VOLTAGE 13.8
#define HEATER_POWER ((HEATER_VOLTAGE) * (HEATER_VOLTAGE) / HEATER_RESISTANCE)

float Kq = 1;
float Cp = 1;

/* Calculating the Heat capacities of air 
This function calculates the Heat Capacities of Air (Cp).
Made a statistical plot, based on the heat capacities or air table.
Found the best fitted cubic function according to it's data.
Temperature needs to be in Kelvin.
*/

float get_heat_capacity(float temp)
{
    float cp;
    cp = ((-1.29142e-10)*(temp^3))+((3.2309586e-7)*(temp^2))+(-4.703434e-5*temp)+ 0.9917484975;
    return cp;
}

/* Calculating the Mass flow 
This function calculates the Mass flow.
Parameters are the temp1 and temp2 from the thermistors, and the heat capacity of the air.
*/

float get_mass_flow(float temp1, float temp2, float cp)
{
    float dtemp = temp1 - temp2;
    
    float M=(K*HEATER_POWER)/(Cp*dtemp);

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
