/*
 * PSerial.c
 *
 * Created: 2/11/2017 2:13:04 PM
 * Member1 : Eduardo
 * Member2 : Sam Fenimore
 */ 
//#define BAUD 19200 
#include <avr/io.h>
#include <util/delay.h>
#include "PSerial.h"
#include "EmSys.h"

int main(void)
{
    /* Replace with your application code */
	init_ports(2400, SERIAL_8E1);
	//char test = '1';
	
    while (1) 
    {
		//	char label2[] ="helloWorld";
			char label2[] = "0123456789ABCDEF";

			for(int i = 0; i<8; i++)
			{
				PSerial_write(0, label2[i]);
			}
			PSerial_write(0, '\r');
			PSerial_write(0, '\n');
			//_delay_ms(500);
    }
}