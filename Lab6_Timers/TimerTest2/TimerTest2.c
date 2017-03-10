/*
 * TimerTest2.c
 *
 * Created: 2/23/2017 2:58:08 PM
 * Author : fenim
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#define FREQcount 20832
#define OTIMEcount 4686

//this is timer1 ctc mode
int main(void)
{

	PORTB ^= 0x80;
	TCCR1B |= ((1<<WGM13) | (1 << WGM12)); // Configure timer 1 for CTC mode
	TCCR1B |= (1 << CS12);  // Set up timer at Fcpu/256
	TIMSK1 |= (1 << OCIE1A); // Enable CTC interrupt //MAYBE NO IDK
	
	TIMSK1 |= (1 << OCIE1A); //timer compare interrupt
	TIMSK1  |= (1<<ICIE1); //input capture interrupt enable
	
	OCR1A = OTIMEcount; //set compa
	ICR1 = FREQcount; //set capt? top val?
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
}
//ISR(TIMER1_CAPT_vect, ISR_ALIASOF(PCINT0_vect));
ISR(TIMER1_CAPT_vect)
{
	PORTB ^= 0x80;
}
