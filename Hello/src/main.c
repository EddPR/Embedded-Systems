/*
 * Hello.c
 *
 * Created: 1/17/2017 2:36:39 PM
 * Author : Eduardo Padilla [padillae]
 * Member2: Samuel Fenimore [fenimoress]
 */ 

#include <avr/io.h>
#include <util/delay.h>

#define MAX_USEC_DELAY 0

//int add2(int, int);
extern int delay_usec(unsigned int);

int main(void)
{
	//return delay_usec(2);
    //int z = add2(5, 7);
	
	DDRB = 0X80;	// Set PORTB.7 for OUTPUT
    while (1) 
    {
		//PORTB ^= 0x80;
		PORTB = 0x80;
		// Delay for a while
		//_delay_ms(500);
		delay_usec(100);
		PORTB &= ~0x80;
		_delay_ms(50);
    }
}
