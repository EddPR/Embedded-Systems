/*
 * TimerTest3.c
 *
 * Created: 2/23/2017 2:58:08 PM
 * Author : Sam Fenimore [fenimoress]
 * member2: Eduardo Padilla [padillae]
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>

#define FREQcount 20832/2
#define OTIMEcount 4686/2

//this is timer1 pcm mode
int main(void)
{
	DDRB = 0x80;
	ICR1 = FREQcount;
	OCR1C = OTIMEcount;
		

	TCCR1B |= (1 << CS12);  // Set up timer at Fcpu/256

	TCCR1B |= (1 << WGM13); // Set OC1A when up counting, Clear when down counting

	
	

	TCCR1A |= (1 << COM1C1); // Enable timer 1 Compare Output channel A in toggle mode
	

	while(1)
	{

	}
}