/*
 * ButtonFlashInt.c
 *
 * Created: 2/24/2017 7:26:13 AM
 * Author : fenim
 */ 

#include <avr/io.h>
#include <util/delay.h>

//PIN A8, PORTK0, PCINT16
#define FALLING_EDGE_ONE ((PINF >> 8) == 0x02)	// Button ONE
#define RISING_EDGE_ONE ((PINF >> 8) == 0x03)	// Button ONE
//PIN A9, PORTK1, PCINT17
#define FALLING_EDGE_ZERO ((PINF >> 9) == 0x00)	// Button ZERO
#define RISING_EDGE_ZERO ((PINF >> 9) == 0x01)	// Button ZERO

volatile char button0Pressed;
volatile char button1Pressed;

void lightsCycle();
void lightsOff();
void lightsFlash();

int main(void)
{
	//	INPUT = 0, OUTPUT = 1
	
	DDRF = 0x0F;	// Set Pins A0-A3 for OUTPOUT 
	//PORTF = 0xC0;	// Set Pins A6-A7 for PULL_UP			changed to portk cause the buttons are now shifted up to pin a8 and a9
	PORTK = 0x03; //set inturupts? but definately setting pullup
	DDRK = 0x00;  //set buttons to 0 so it will be input and configured correct for pullup and A8-A9 for INPUT
	
	lightsOff();
}

void lightsOff()
{
	while (1)
	{
		PORTF &= ~0x0F;	// Turn off all LEDs
		
		_delay_ms(5);	// Sample button press
		if (FALLING_EDGE_ZERO)
		{
			_delay_ms(500);	// Sample button release
			if (RISING_EDGE_ZERO)
			{
				lightsCycle();
			}
		}
		else if (FALLING_EDGE_ONE)
		{
			_delay_ms(500);	// Sample button release
			if (RISING_EDGE_ONE)
			{
				lightsFlash();
			}
		}
	}
}

void lightsCycle()
{
	while (1)
	{
		int leds = 1;
		
		for (int i = 0; i < 4; i++)
		{
			PORTF |= leds << i;		// LED on
			
			for (int j = 0; j < 100; j++)
			{
				
				if (FALLING_EDGE_ZERO)
				{
					_delay_ms(10);	// Sample button release
					if (RISING_EDGE_ZERO)
					{
						lightsOff();
					}
				}
				else if (FALLING_EDGE_ONE)
				{
					_delay_ms(10);	// Sample button release
					if (RISING_EDGE_ONE)
					{
						lightsFlash();
					}
				}
				else
				{
					_delay_ms(10);
				}
			}
			
			PORTF &= ~(leds << i);		// LED off
		}
	}
}


void lightsFlash()
{
	while (1)
	{
		PORTF |= 0x0F;	// Turn on all LEDs
		
		for (int j = 0; j < 20; j++)
		{
			if (FALLING_EDGE_ZERO)
			{
				_delay_ms(10);	// Sample button release
				if (RISING_EDGE_ZERO)
				{
					lightsCycle();
				}
			}
			else if (FALLING_EDGE_ONE)
			{
				_delay_ms(10);	// Sample button release
				if (RISING_EDGE_ONE)
				{
					lightsOff();
				}
			}
			else
			{
				_delay_ms(10);
			}
		}
		
		
		PORTF &= ~0x0F;	// Turn off all LEDs
		
		for (int j = 0; j < 100; j++)
		{
			if (FALLING_EDGE_ZERO)
			{
				_delay_ms(10);	// Sample button release
				if (RISING_EDGE_ZERO)
				{
					lightsCycle();
				}
			}
			else if (FALLING_EDGE_ONE)
			{
				_delay_ms(10);	// Sample button release
				if (RISING_EDGE_ONE)
				{
					lightsOff();
				}
			}
			else
			{
				_delay_ms(10);
			}
		}
	}
}
