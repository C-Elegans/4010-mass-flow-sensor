

const float heater_resistance = 40; // Ohms
const float heater_voltage = 13.8;
const float heater_power = heater_voltage * heater_voltage / heater_resistance;
/* Paul, you want to take this one? This should return the mass flow
rate given the temperature of the two sensors in the tube */
float get_mass_flow(float temp1, float temp2){

    return 0.0;
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


// I (Michael) can fill this in with the networking code to send it to
// the server
int main(int argc, char** argv){

    return 1;
}
