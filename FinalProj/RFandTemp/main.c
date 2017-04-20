/*
 * RFandTemp.c
 * this one runs on the mega2560
 * Created: 4/20/2017 12:28:39 PM
 * Author : fenim
 */ 
#include <avr/io.h>
#include <util/delay.h>

#define MAX_USEC_DELAY 0
//use pb6 for output
void wTime(void);
int main(void)
{
	DDRB |= 0x80;	// Set PORTB.7 for OUTPUT which is the led one and pin 13
	DDRB |= 0x40;	//setup this one for output to other arduino pin 12 portB.6
	//DDRB |= 0x10;
	while (1)
	{
		//PORTB = 0xC0;
		PORTB |= 0x80; //pin 13 which does led
		PORTB |= 0x40; //pin 12
		//PORTB |= 0x10; //pin 10
		// Delay for a while
		_delay_ms(500);
		PORTB = 0x00;		//should toggle this output pin 12 on and off which should toggle other boards led
		//PORTB &= ~0x80;		//will toggle boards led	
		//PORTB &= ~0xC0;		//will toggle boards led	
		_delay_ms(500);
		/*if(0x10 == (PORTB & 0x10))
		{
			wTime();
		}*/
	}
}
void wTime(void)
{
	_delay_ms(2000);
}

