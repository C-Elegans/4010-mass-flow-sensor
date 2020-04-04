// Server side C program to acquire and
// transfer to the Analyzer GUI
// Author: Tom Fallon
// Copyright 2020
 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h>
#include <netinet/tcp.h> 
#include <string.h>
#include <unistd.h>
#include <pthread.h> 
#include <arpa/inet.h>
#include "ADC.h"
#define PORT 8081 

// Our ADC-related protofunctions referenced in ADC.h

FILE * setADC();
float getVoltage(FILE * in);
int closeADC(FILE * in);

// Protofunction for Analyzer Receive thread
void *AnalyzerReceive(void *arg);

// Declare our global ADC file pointer
// Add a lock to it in subsequent version
FILE *in;

// Make our new socket global to use it in receive thread
int new_socket;

// Create our delay variable to be used in the acquisition loop
useconds_t delay = 10000;

// Create our getVoltage bypass variable to pause acquisition
long start = 0;

void *AnalyzerReceive(void *arg)
{
	// Continously loop while blocking while waiting to receive information	
    	while(1)
	{   
	    char buffer[1024] = {0}; 
	    int valread = read(new_socket , buffer, 1024);
	    if (strcmp(buffer,"start"))
	    {
		start = 1; 	    
	    }
	    else if (strcmp(buffer,"delay"))
	    {
		// Modify delay based on scrollbar setting
		// in AnalyzerGUI
		delay = (useconds_t)(atoi(buffer)*1000);
 	    }

	    printf("%s\n",buffer );
	    
            usleep(50000);
	}

}

int main(int argc, char const *argv[]) 
{ 
	// Open the virtual driver for our ADC
	in = setADC();
	
	// Declare our pthread objects for our ADC work and analyzer receive
	pthread_t adc, arcv;

	int valread, server_fd;

	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 

	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	// Forcefully attaching socket to the port 8081 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	
	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address,  sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
					(socklen_t*)&addrlen))<0) 
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	} 
 	
	int i =0;
	// Create our receive thread to receive commands from the GUI
	if (pthread_create(&arcv, NULL, &AnalyzerReceive, &i) != 0)
	{
		perror("Error in creating thread");
	}
	
	// Read voltage and transmit via our socket
   	while(1)
	{   
	    if (start)
	    {
		    float v = getVoltage(in);
		    char buffer2[1024] = {0};
		    char c = '\n'; 
		    snprintf(buffer2, sizeof(buffer2), "%f%c",v,c);
		    send(new_socket , buffer2 , strlen(buffer2) , 0 );
		    printf("Value of sensor is %f\n",v); 
	            rewind(in);
	    }
            usleep(10000);
 	}

	close(new_socket);
	return 0; 
} 