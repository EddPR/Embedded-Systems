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
			char data = PSerial_read(1);
			PSerial_write(0, data);
			/*char data[16];
			char data2[16];
			for(int i = 0; i<16; i++)
			{
				data[i] = PSerial_read(1);
				data2[i] = PSerial_read(2);
			}

			for(int i = 15; i>=0; i--)
			{
				if(data[i] != data2[i])
				{
					PSerial_write(0, '\r');
					PSerial_write(0, '\n');
					PSerial_write(0, '\r');
					PSerial_write(0, '\n');
					break;
				}
				PSerial_write(0, data[i]);
			}*/
			//char label[] = "Single";
			/*//PSerial_write(0, PSerial_read(1));
			char label2[];
			char checksum = 0;

			for(int i = 0; i<14; i++)
			{
				label2[i] = PSerial_read(1);
				PSerial_write(0, label2[i]);
				checksum ^= label2[i];
			}
			if(checksum == label2[14])
			{
				PSerial_write(0, checksum);
			}*/
    }
}
