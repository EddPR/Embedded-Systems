/*
 * PSerial.c
 *
 * Created: 2/11/2017 2:15:48 PM
 *  Author: Eduardo
 */ 
#include "PSerial.h"

//static uint8_t ucsrna;

SERIAL_REGS *serial_port[] = {
	(SERIAL_REGS *)(0xc0),	// serial port 0
	(SERIAL_REGS *)(0xc8),	// serial port 1
	(SERIAL_REGS *)(0xd0),	// serial port 2
	(SERIAL_REGS *)(0x130)	// serial port 3
};

void PSerial_open(unsigned char port, long speed, int config)
{	
	uint8_t enableT;
	uint8_t enableR;
	
	switch (port) {
		case 0:
			enableR = (1 << RXEN0);
			enableT = (1 << TXEN0);
			break;
		case 1:
			enableR = (1 << RXEN1);
			enableT = (1 << TXEN1);
			break;
		case 2:
			enableR = (1 << RXEN2);
			enableT = (1 << TXEN2);
			break;
		case 3:
			enableR = (1 << RXEN3);
			enableT = (1 << TXEN3);
			break;
		default:
			enableR = (1 << RXEN0);
			enableT = (1 << TXEN0);
			break;
	}
	
	//ucsrna = 0x20; //initilize with data reg empty
	serial_port[port]->ucsrb = enableR | enableT;
	serial_port[port]->ucsrc = config;
	speed = (F_CPU / 16 / speed - 1);
	serial_port[port]->ubrrH = (unsigned char) (speed >> 8);
	serial_port[port]->ubrrL = (unsigned char) speed;
}


char PSerial_read(unsigned char port)
{
	char data;
	uint8_t rxcn;
	
	switch (port) {
		case 0:
	//		ucsrna = UCSR0A;
			rxcn = (1 << RXC0);
			data = UDR0;
			break;
		case 1:
	//		ucsrna = UCSR1A;
			rxcn = (1 << RXC1);
			data = UDR1;
			break;
		case 2:
	//		ucsrna = UCSR2A;
			rxcn = (1 << RXC2);
			data = UDR2;
			break;
		case 3:
	//		ucsrna = UCSR3A;
			rxcn = (1 << RXC3);
			data = UDR3;
			break;
		default:
	//		ucsrna = UCSR0A;
			rxcn = (1 << RXC0);
			data = UDR0;
			break;
	}
	
	while (!(serial_port[port]->ucsra & rxcn)) {
		// wait for RXC
	}
	return data;
}

void PSerial_write(unsigned char port, char data)
{
	static int udren;
	switch (port) {
		case 0:
			udren = (1 << UDRE0);
			break;
		case 1:
			udren = (1 << UDRE1);
			break;
		case 2:
			udren = (1 << UDRE2);
			break;
		case 3:
			udren = (1 << UDRE3);
			break;
		default:
			udren = (1 << UDRE0);
			break;
	}
	
	while (!(serial_port[port]->ucsra & udren))
	{
		//wait for RXC
	}
	serial_port[port]->udr = data;
}