/*
 * ReciveUNO.c
 * this one runs on the uno and is the reciever for everything
 * Created: 4/20/2017 12:34:57 PM
 * Author : fenim
 */ 
//-carduino
#include <avr/io.h>
#include <util/delay.h>
//-C"C:\Program Files (x86)\Arduino\hardware/tools/avr/etc/avrdude.conf" -v -patmega2560 -cwiring -PCOM3 -b115200 -D -Uflash:w:"$(ProjectDir)Debug\$(TargetName).hex":i
//-patmega328p
//C:\Program Files (x86)\Arduino\hardware\tools\avr/bin/avrdude -CC:\Program Files (x86)\Arduino\hardware\tools\avr/etc/avrdude.conf -v -patmega328p -carduino -PCOM4 -b115200 -D -Uflash:w:C:\Users\fenim\AppData\Local\Temp\arduino_build_274099/Arduino_ButtonFLash.ino.hex:i 

#define MAX_USEC_DELAY 0
//B5 is built in led
//D7 is going to be input


int main(void)
{
	//DDRB = 0X80;	// Set PORTB.5 for OUTPUT
	DDRB |= 0x20;//(1 << DDB5); //this one is for output
	DDRD &= ~(0x10);//(1 << DDD7); //pin 4 for input
	//_delay_ms(500);

	int temp;
	while (1)	{
		temp = PIND & 0x10;
		if(temp == 0x10) //check if pind7 is high which is just digital pin 4
		{
			PORTB &= 0x00;
			
		}
		else
		{
		//	_delay_ms(500);
			PORTB |= 0x20; //should turn on B5 which is builtin led
		}
		//_delay_ms(500);
	}
}
