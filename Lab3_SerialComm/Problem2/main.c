/*
 * main.c
 *
 * Created: 2/6/2017 12:17:48 AM
 * Author : Eduardo Padilla [padille]
 * Member2: Sam Fenimore	[fenimoress]
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "EmSys.h"
#include "SWSerial.h"

#define MAX_USEC_DELAY 0

int main(void) {

	// init rx_pin, tx_pin, baudrate, framing...
	
	init_sw_serial(TX_PIN, PIN_12, BAUD_38400, SERIAL_8N1);  // this is your function
	init_sw_serial_puts_test(BAUD_38400, SERIAL_8N1);   // library function: note we need the baudrate and framing here

	while(1) {
		sw_serial_puts("Hello,World!\n");
		test_sw_serial_puts();   // library test function
		_delay_ms(1000);
	}
}





