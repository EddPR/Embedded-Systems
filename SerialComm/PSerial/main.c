/*
 * PSerial.c
 *
 * Created: 2/11/2017 2:13:04 PM
 * Author : Eduardo
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "PSerial.h"
#include "EmSys.h"

int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
		PSerial_open(0, 19200, SERIAL_8N1);
		if (PSerial_read(0)) {
			PSerial_write(0, 'U');
		}
    }
}

