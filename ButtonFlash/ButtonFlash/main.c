/*
 * ButtonFlash.c
 *
 * Created: 2/16/2017 2:49:13 PM
 * Author : padillae
 * member2: Sam Fenimore
 */ 

#include <avr/io.h>
#include <util/delay.h>

#define FALLING_EDGE_ONE ((PINF >> 6) == 0x02)	// Button ONE
#define RISING_EDGE_ONE ((PINF >> 6) == 0x03)	// Button ONE
#define FALLING_EDGE_ZERO ((PINF >> 7) == 0x00)	// Button ZERO
#define RISING_EDGE_ZERO ((PINF >> 7) == 0x01)	// Button ZERO

void lightsCycle();
void lightsOff();
void lightsFlash();

int main(void)
{
	//	INPUT = 0, OUTPUT = 1
	
	DDRF = 0x0F;	// Set Pins A0-A3 for OUTPOUT and A6-A7 for INPUT 
	PORTF = 0xC0;	// Set Pins A6-A7 for PULL_UP
    
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
