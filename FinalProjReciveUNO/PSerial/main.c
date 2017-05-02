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
		//	char label2[] ="helloWorld";
			char label[] = "Hello World!!!";
			char checksum = 0;

			for(int i = 0; i<14; i++)
			{
				PSerial_write(0, label[i]);
				checksum += label[i];
			}
			PSerial_write(0, '\n');
			PSerial_write(0, checksum);
		/*
			for(int i = 0; i<10; i++)
			{
				PSerial_write(0, label2[i]);
				checksum += label2[i];
			}
			PSerial_write(0, '\n');
			_delay_ms(500);*/
    }
}
