/*
 * Problem1.c
 *
 * Created: 1/31/2017 2:33:42 PM
 * Author : fenim
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "EmSys.h"
extern int delay_usec(unsigned int);

void sw_serial_putc(char c);

int main(void) 
{
	// init PORTB so that bit 6 (digital pin 12) is set for OUTPUT
	DDRB |= (1 << PB6);  // Remember that this is the DATA DIRECTION register--use PORTB to set/clear bit 6 output
	debug_init();
	init_sw_serial_putc_test(9600,SERIAL_8N1);
	
	while(1) 
	{
		sw_serial_putc('U');
		test_sw_serial_putc();
		_delay_ms(1000);
	}
}
void sw_serial_putc(char c)
{
	PORTB &= 0b10111111;
	delay_usec(104);
	
	for(int i = 0; i < 8; i++)
	{
		if((c >> i) & 1 == 1)
		{
			PORTB |=  0b01000000;
		}
		else
		{
			PORTB &= 0b10111111;
		}
		delay_usec(104);
	}
	
	PORTB |=  0b01000000;
}