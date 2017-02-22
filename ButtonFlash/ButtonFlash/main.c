/*
 * ButtonFlash.c
 *
 * Created: 2/16/2017 2:49:13 PM
 * Author : padillae
 */ 

#include <avr/io.h>
#include <util/delay.h>

#define HIGH 1
#define LOW 0

void retainState();
void lightCycle();
void lightsOff();

int oldButtonOneState = LOW;
int oldButtonTwoState = HIGH;

int x = 0;

int main(void)
{
	//	INPUT = 0, OUTPUT = 1
	//	oldButtonOneState == HIGH && newButtonOneState == LOW // Falling edge
	//	oldButtonOneState == LOW && newButtonOneState == HIGH // Rising Edge
	
	DDRF = 0x0F;	// Set Pins A0-A3 for OUTPOUT and A6-A7 for INPUT 
	PORTF = 0xC0;	// Set Pins A6-A7 for PULL_UP
    
    while (1) 
    {
		int newButtonOneState = 0x80 & PINF;	// read button one state
		int newButtonTwoState = 0x40 & PINF;	// read button two state
		
		if (newButtonOneState == LOW)
		{
			lightCycle();	// Works only while button is being pressed
		}
    }
}

void lightCycle()
{
	PORTF |= 0x01;	// Turn on LED1
	_delay_ms(1000);
	PORTF &= ~0x01;	// Turn off LED1
	_delay_ms(1000);
	PORTF |= 0x02;	// Turn on LED2
	_delay_ms(1000);
	PORTF &= ~0x02;	// Turn off LED2
	_delay_ms(1000);
	PORTF |= 0x04;	// Turn on LED3
	_delay_ms(1000);
	PORTF &= ~0x04;	// Turn off LED3
	_delay_ms(1000);
	PORTF |= 0x08;	// Turn on LED4
	_delay_ms(1000);
	PORTF &= ~0x08;	// Turn off LED4
	_delay_ms(1000);
}

void lightsOff()
{
	PORTF &= ~0x0F;	// Turn off all LEDs
}


