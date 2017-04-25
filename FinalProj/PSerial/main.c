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
	init_ports(600, SERIAL_8N1);
	//char test = '1';
	
    while (1) 
    {
			//char label[] = "Single";
			//PSerial_write(1, PSerial_read(0));
			PSerial_write(0, PSerial_read(1));
			//test();

			/*test = test + 1;
			if(test==':')
			{
				test = '1';
			}
			_delay_ms(500);*/
    }
}

