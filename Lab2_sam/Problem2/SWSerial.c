/*
 * SWSerial.c
 *
 * Created: 2/12/2017 4:15:26 PM
 *  Author: fenim
 */ 
#include "SWSerial.h"
#include <avr/io.h>
#include <util/delay.h>
extern int delay_usec(unsigned int);

static int tx_pin;
static int rx_pin;
static int framing;
static long baudrate;
static int bitTime;
static int mask;
static int recip_mask;

void init_sw_serial(int rx_pin, int tx_pin, long baudrate, int framing)
{
	tx_pin = (tx_pin - 19);
	rx_pin = rx_pin;
	baudrate = baudrate;
	framing = framing;
	switch(baudrate) {

		case 1200  :
		bitTime =  833;
		break; /* optional */
		
		case 2400  :
		bitTime = 417;
		break; /* optional */
		
		case 4800  :
		bitTime = 208;
		break; /* optional */

		case 9600  :
		bitTime = 104;
		break; /* optional */
		
		case 14400  :
		bitTime = 69;
		break; /* optional */
		
		case 19200  :
		bitTime = 52;
		break; /* optional */
		
		case 28800  :
		bitTime = 35;
		break; /* optional */
		
		case 38400  :
		bitTime = 26;
		break; /* optional */

		case 57600  :
		bitTime = 17;
		break; /* optional */
		
		case 115200  :
		bitTime = 9; //should be 9
		break; /* optional */
		
		case 250000  :
		bitTime = 4;
		break; /* optional */
		
		case 500000  :
		bitTime = 2;
		break; /* optional */
		
		case 1000000  :
		bitTime = 1;
		break; /* optional */
				
		/* you can have any number of case statements */
		default : /* Optional */
		bitTime = -1;
		break;
	}

	tx_pin = (tx_pin);
	DDRB |= (1 << tx_pin);

	
}
void sw_serial_putc(char c)
{
	PORTB &= recip_mask;
	delay_usec(bitTime);
	
	for(int i = 0; i < 8; i++)
	{
		if((c >> i) & 1 == 1)
		{
			PORTB |=  mask;
		}
		else
		{
			PORTB &= recip_mask;
		}
		delay_usec(bitTime);
	}
	
	PORTB |=  mask;
}
void sw_serial_puts(char *str)
{
	mask = ((-1<<1)<<(tx_pin));
	recip_mask = ~mask;
	int len = strlen(str);
	for(short i = 0; i < len; i++)
	{
		sw_serial_putc(str[i]);
		delay_usec(bitTime);
	}
}