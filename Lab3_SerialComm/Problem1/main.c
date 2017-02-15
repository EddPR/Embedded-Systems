/*
 * Problem1.c
 *
 * Created: 1/31/2017 1:20:39 PM
 * Author : Eduardo Padilla [padille]
 * Member2: Sam Fenimore	[fenimoress]
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "EmSys.h"

#define MAX_USEC_DELAY 0

extern int delay_usec(unsigned int);
void sw_serial_putc(char);

int main(void) {
	// init PORTB so that bit 6 (digital pin 12) is set for OUTPUT
	DDRB |= (1 << PB6);  // Remember that this is the DATA DIRECTION register--use PORTB to set/clear bit 6 output
	init_sw_serial_putc_test(9600,SERIAL_8N1);
	while(1) {
		sw_serial_putc('U');
		test_sw_serial_putc();
		_delay_ms(1000);
	}
}

void sw_serial_putc(char c)
{
	int i;
	unsigned int pos = 0x01;
	PORTB |= 0x40;	//idle state 
	delay_usec(104);	//delay 104 ms based on BAUD
	PORTB &= 0xBF; //logical 0
	delay_usec(104);	//delay 104 ms based on BAUD
	for (i = 0; i < 8; i++)	//loop through all 8 bits of 'c'
	{
		if ((c & pos) == 0)
		{
			PORTB &= 0xBF; //logical 0
		}
		else
		{
			PORTB |= 0x40;	//logical 1
		}
		delay_usec(104);	//delay 103 ms based on BAUD
		pos <<= 1;
	}
	PORTB |= 0x40;	//stop
	delay_usec(104);	//delay 104 ms based on BAUD
}

