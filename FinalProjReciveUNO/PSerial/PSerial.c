/*
 * PSerial.c
 *
 * Created: 2/11/2017 2:15:48 PM
 *  Member1 : Eduardo Padilla [padille]
 *	Member2: Sam Fenimore	[fenimoress]
 */ 
#include "PSerial.h"

//static uint8_t ucsrna;

SERIAL_REGS *serial_port[] = {
	(SERIAL_REGS *)(0xc0),	// serial port 0

};

void PSerial_open(unsigned char port, long speed, int config)
{	

	serial_port[port]->ucsrb = (1 << RXEN0) | (1 << TXEN0);
	serial_port[port]->ucsrc = config;
	speed = (F_CPU / 16 / speed - 1);
	serial_port[port]->ubrrH = (unsigned char) (speed >> 8);
	serial_port[port]->ubrrL = (unsigned char) speed;
}


char PSerial_read(unsigned char port)
{
	uint8_t rxcn;
	

	//ucsrna = UCSR0A;
	rxcn = (1 << RXC0);
	
	while (!(serial_port[port]->ucsra & rxcn)) {
		// wait for RXC
	}
	return UDR0;
}

void PSerial_write(unsigned char port, char data)
{
	static int udren;
	udren = (1 << UDRE0);
	
	/*while (!(serial_port[port]->ucsra & udren))
	{
		//wait for RXC
	}*/
	serial_port[port]->udr = data;
}

void init_ports(long baud, int framing) 
{ 

		PSerial_open(0, baud, framing);
	
}