/*
 * TimerTest.c
 *
 * Created: 2/23/2017 2:58:08 PM
 * Author : fenim
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#define FREQcount 20832
#define OTIMEcount 4686

//this is timer1 normal mode leapfrog
int main(void)
{

	OCR1A = FREQcount; //that is for the 3hz val //for the 75ms or 13.33333 val its 4686.500011718750 so we will start count at 16145.833 for the ontime toggle vs starting at 0
	TIMSK1 |= (1 << OCIE1A); //timer compare interrupt
	TCCR1B |= (1 << CS12);  // Set up timer at Fcpu/256
	TCNT1 = 0;
	sei();
	
	DDRB = 0x80; //built in led

	//loop forever
	while(1)
	{

	}
}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
	PORTB ^= 0x80;
	if(PORTB == 0x80) //ontime
	{
		OCR1A += OTIMEcount;
	}
	else if(PORTB != 0x80)//period
	{
		OCR1A += (FREQcount-OTIMEcount);
	}
}
