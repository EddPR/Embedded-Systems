/*
 * PSerial.c
 *
 * Created: 2/11/2017 2:13:04 PM
 * Author : Eduardo
 */ 
//#define BAUD 19200 
#include <avr/io.h>
#include <util/delay.h>
#include "PSerial.h"
#include "EmSys.h"

int main(void)
{
    /* Replace with your application code */
	PSerial_open(0, 19200, SERIAL_8N1);
    while (1) 
    {
			PSerial_write(0, PSerial_read(0));
    }
}

