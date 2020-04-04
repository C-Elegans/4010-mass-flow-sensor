// A C library that sets and reads the AIN0 file to display 
// a single captured voltage on a BeagleBone Black
// Copyright 2020 by Tom Fallon

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define VRef 1.8

// Creating and returning the file pointer to the ADC virtual driver
// For next revision, pass in the AIN number

FILE * setADC()
{
	/* Access to the ADC inputs on the BBB is exported into the 
 	   virtual file system in_voltageX_raw files as seen below
	*/
	const char* ainFile = "in_voltage0_raw";
	const char* dir = "/sys/bus/iio/devices/iio:device0/";
	// printf("Have directory string %s\n",dir);

	const size_t path_size = strlen(dir) + strlen(ainFile)+1;
	// printf("Have path size %d\n", path_size);
	
	// Create string and allocate memory for the virtual path and filename	
	char* path = malloc(path_size);
	// printf("Successfully allocated memory\n");

	// Declare our file pointer
	FILE *in;
	
	// Initialize our path string
	if (path)
	{
    	   snprintf(path, path_size, "%s%s", dir, ainFile);
	   // printf("Path is %s\n", path);
	   // Open our virtual file to device AIN0 for read	
           in = fopen(path, "r");
	   // Free our allocated memory 
	   free(path);
	}
	return in;

} // end of setADC

// Convert the integer value representing the sampled voltage into a float
// and return it
	
float getVoltage(FILE * in)
{
	char value_str[7];
    	long int value_int = 0;

        fread(&value_str, 6, 1, in);
        value_int = strtol(value_str,NULL,0);

	float voltage = value_int*VRef/4095;

	return voltage;
} // end of getVoltage

// Close our file pointer

int closeADC(FILE * in)
{
	fclose(in);
} //end of library