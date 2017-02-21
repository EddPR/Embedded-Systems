/*
 * ButtonFlash.c
 *
 * Created: 2/16/2017 2:49:13 PM
 * Author : padillae
 */ 

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	// INPUT = 0, OUTPUT = 1
	
	DDRF = 0x0F;	// Set Pins A0-A3 for OUTPOUT and A6-A7 for INPUT 
    /* Replace with your application code */
    while (1) 
    {
		PORTF = 0xC0;	// Set Pins A6-A7 for PULL_UP
		PORTF |= 0x01;	// Turn on LED1
		_delay_ms(1000);
		PORTF &= ~0x01;	// Turn off LED1
    }
}


