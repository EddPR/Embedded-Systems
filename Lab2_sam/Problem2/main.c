/*
 * Problem1.c
 *
 * Created: 1/31/2017 2:33:42 PM
 * Author : Sam Fenimore, Eduardo Padilla
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "EmSys.h"
#include "SWSerial.h"
extern int delay_usec(unsigned int);



int main(void) {

	// init rx_pin, tx_pin, baudrate, framing...
	int tx_pin = 24; //25 is pb6 or digital pin 12, 24 is pb5 or digital pin 11
	int rx_pin = 2; //pd2
	int framing = SERIAL_8N1;
	long baudrate = 115200;
	
	init_sw_serial(rx_pin, tx_pin, baudrate, framing);  // this is your function
	init_sw_serial_puts_test(baudrate, framing);   // library function: note we need the baudrate and framing here
	
	
	while(1) {
		sw_serial_puts("Hello,World!\n");
		test_sw_serial_puts();   // library test function
		_delay_ms(1000);
	}
}