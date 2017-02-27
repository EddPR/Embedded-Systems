/*
 * ButtonFlashInt.c
 *
 * Created: 2/24/2017 7:26:13 AM
 * Author : Sam Fenimore	[fenimoress]
 * Member2: Eduardo Padilla [padille]
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//working with pin k0 =a8 and k1=a9 for the buttons
volatile char button0Pressed = 0;
volatile char button1Pressed = 0;
volatile char prevPins = 0xFF; //since its a pullup we have it set to true as a default thing

void lightsCycle();
void lightsOff();
void lightsFlash();

int main(void)
{

	DDRF = 0x0F;	// Set Pins A0-A3 for OUTPOUT 
	PORTF = 0x00;	// clear out the portf pins			changed to portk cause the buttons are now shifted up to pin a8 and a9
	
	PORTK |= ((1 << PORTK0) | (1 << PORTK1)); //setting pullup
	DDRK &= ~((1 << DDK0) | (1 << DDK1));  //set buttons to 0 so it will be input and configured correct for pullup and A8-A9 for INPUT
	
	//inturupt vecotor setup
	PCMSK2 |= (1<<PCINT16);
	PCMSK2 |= (1<<PCINT17);
	PCICR |= (1<<PCIE2);
	sei();
	
	lightsOff();
}

ISR(PCINT2_vect) //INTurpt vect enable for for a8 & a9 ie pcint16 & 17
{ 
	// your interrupt handler code
	char changed = prevPins ^ PINK; //get the changed things
	prevPins = PINK; //set previous pins to the current one
	
	//now do logic to see if pink0 or k1 ie. a8 or a9 respectavly was changed
	if ((changed & (1 << PINK0))) //look at a8
	{
		if((PINK & 0x01) == 0x01) //if it changed & had a falling edge
		{
			button0Pressed = 1;
		}
	}
	else if ((changed & (1 << PINK1))) //look at a9
	{
		
		if((PINK & 0x02) == 0x02)
		{
			button1Pressed = 1;
		}
	}
}


void lightsOff()
{
	PORTF &= ~0x0F;	// Turn off all LEDs
	while (1)
	{

		if (button0Pressed == 1)
		{
			button0Pressed = 0;
			lightsCycle();
		}
		else if (button1Pressed == 1)
		{
			button1Pressed = 0;
			lightsFlash();
		}
	}
}

void lightsCycle()
{
	PORTF &= ~0x0F;
	while (1)
	{
		int leds = 1;
		
		for (int i = 0; i < 4; i++)
		{
			PORTF |= leds << i;		// LED on
			
			if (button0Pressed == 1)
			{
				button0Pressed = 0;
				lightsOff();
			}
			else if (button1Pressed == 1)
			{
				button1Pressed = 0;
				lightsFlash();
			}
			_delay_ms(200);

			PORTF &= ~(leds << i);		// LED off
		}
	}
}

	
void lightsFlash()
{
	PORTF &= ~0x0F;
	while (1)
	{
		//compiler does very weird things without this first delay
		PORTF |= 0x0F;	// Turn on all LEDs
		_delay_ms(200);
		PORTF &= ~0x0F; //turn off leds
		_delay_ms(200);
		if (button0Pressed == 1)
		{
			button0Pressed = 0;
			lightsCycle();
		}
		else if (button1Pressed == 1)
		{
			button1Pressed = 0;
			lightsOff();
		}

	}
}
