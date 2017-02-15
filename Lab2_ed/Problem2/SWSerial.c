/*
 * SWSerial.c
 *
 * Created: 2/6/2017 12:32:06 AM
 * Author : Eduardo Padilla [padille]
 * Member2: Sam Fenimore	[fenimoress]
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "SWSerial.h"

extern int delay_usec(unsigned int);

static unsigned char mask_on;
static unsigned char mask_off;
static long bps;


void init_sw_serial(int rx_pin, int tx_pin, long baudrate, int framing) {
	
	switch(baudrate) {
		case BAUD_1200:
			bps = BPS_1200;		// Does NOT work
			break;
		case BAUD_2400:
			bps = BPS_2400;		// WORKS
			break;
		case BAUD_4800:
			bps = BPS_4800;		// WORKS
			break;
		case BAUD_9600:
			bps = BPS_9600;		// WORKS
			break;
		case BAUD_14400:
			bps = BPS_14400;	// WORKS
			break;
		case BAUD_19200:
			bps = BPS_19200;	// WORKS
			break;
		case BAUD_28800:
			bps = BPS_28800;	// WORKS
			break;
		case BAUD_38400:
			bps = BPS_38400;	// WORKS
			break;
		case BAUD_57600:
			bps = BPS_57600;	// WORKS
			break;
		case BAUD_115200:
			bps = BPS_115200;	// Does NOT work
			break;
		case BAUD_250000:
			bps = BPS_250000;	// Does NOT work
			break;
		case BAUD_500000:
			bps = BPS_500000;	// Does NOT work
			break;
		case BAUD_1000000:
			bps = BPS_1000000;	// Does NOT work
			break;
	}
	
	switch(tx_pin) {
		case PIN_53:
			tx_pin = 0;
			break;
		case PIN_52:
			tx_pin = 1;
			break;
		case PIN_51:
			tx_pin = 2;
			break;
		case PIN_50:
			tx_pin = 3;
			break;
		case PIN_10:
			tx_pin = 4;
			break;
		case PIN_11:
			tx_pin = 5;
			break;
		case PIN_12:
			tx_pin = 6;
			break;
			
	}
	
	int mask = (1 << tx_pin);
	
	DDRB |= mask;
	
	mask_on = mask;
	mask_off = ~mask;
	
	PORTB |= mask;	// IDLE
}

void sw_serial_putc(char c)
{
	int i;
	unsigned int pos = 0x01;
	PORTB &= mask_off; //START
	delay_usec(bps);	//delay in ms based on BAUD
	for (i = 0; i < 8; i++)	//loop through all 8 bits of 'c'
	{
		if ((c & pos) == 0)
		{
			PORTB &= mask_off; //logical 0
		}
		else
		{
			PORTB |= mask_on;	//logical 1
		}
		delay_usec(bps);	//delay in ms based on BAUD
		pos <<= 1;
	}
	PORTB |= mask_on;	//STOP
	delay_usec(bps);	//delay in ms based on BAUD
}

void sw_serial_puts(char * word)
{
	char * i;
	for (i = word; *i; i++) {
		sw_serial_putc(*i);
	}
}

