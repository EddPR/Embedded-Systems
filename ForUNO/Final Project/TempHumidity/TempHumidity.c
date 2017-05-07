/*
 * TempHumidity.c
 *
 * Created: 3/21/2017 1:15:12 PM
 * Author : Eduardo Padilla	[padillae]
 * Member2: Sam Fenimore	[fenimoress]
 */ 

#include <stdbool.h>
#include "TempHumidity.h"

volatile int temperature;

SERIAL_REGS *serial_port[] = {
	(SERIAL_REGS *)(0xc0),	// serial port 0
};

int main(void)
{
	serial_open(0, 2400, SERIAL_8E1);
	
	while (1)
	{
		sensorSetup();
		storeDataBytes();
		checkSum();
		resetState();
		for(int i = 0; i < 150; i++)
		{
			serial_write(0, 0x00);
		}
		//_delay_ms(500);
		// Should never enter loop
		//runSensorThread();
	}
}

/***********************************************************************
* Preliminary pin configurations for the DHT22 sensor set up. 
* Function also checks the connection.
***********************************************************************/
void sensorSetup()
{
	cli();	// Disable interrupts
	
	pinMode(DATA_PIN, OUTPUT); // Configure digital pin for output
	digitalWrite(DATA_PIN, LOW); // Drive line low
	_delay_us(500); // Wait for a request to read
	
	pinMode(DATA_PIN, INPUT_PULLUP); // Activate pull-up resistor
	_delay_us(80); // Wait for a signal from sensor
	
	if (digitalRead(DATA_PIN)) // Check sensor connection during line LOW
	{
		serial_print("No connection response during LOW drive. Please check pin connections. \r");
		return;
	}
	_delay_us(80); // Line is driven LOW for 80 usecs, so wait for next check
	// Connection test when line is driven HIGH for last 80 usecs
	if (!digitalRead(DATA_PIN)) // Check sensor connection during line HIGH
	{
		serial_print("No connection response during HIGH drive. Please check pin connections. \r");
		return;
	}

}

/***********************************************************************
* Functions prepares for next data transmission
***********************************************************************/
void resetState()
{
	sei(); // Re-enable interrupts
	
	for (int i = 0; i < 5; i++) bytes[i] = 0; // Clear bytes
	
	//_delay_ms(3000); // Data should be sent every 3 seconds
}

/***********************************************************************
* Function expects the 2 byte temperature data to store them as 1 16 bit
* value to properly calculate the temperature from Celsius to Fahrenheit.
* bytes2: upper 8 bits from Temperature data
* bytes3: lower 8 bits from Temperature data
***********************************************************************/
void printTemperature(uint8_t bytes2, uint8_t bytes3)
{
	char buffer[8];
	
	int temp = ((bytes2) << 8) | bytes3; // store bytes into single var
	temp = temp / 10; // Calculate the whole number 
	int remainder = temp % 10; // Calculate the fraction
	temp = ((temp * 9) / 5) + 32; // Converting Celsius to Fahrenheit
	if (bytes2 < 0) temp = temp * -1; // If negative, print it as such
	itoa(temp, buffer, 10); // Convert data to string
	serial_print(" TMP= ");
	serial_print(buffer);
	itoa(remainder, buffer, 10); // Convert data to string
	serial_print(".");
	serial_print(buffer);
	serial_print(" \r");
	
	temperature = temp;
}

/***********************************************************************
* Function expects the 2 byte humidity data to store them as 1 16 bit
* value to properly interpret the humidity value. 
* bytes0: upper 8 bits from Humidity data
* bytes1: lower 8 bits from Humidity data
***********************************************************************/
void printHumidity(uint8_t bytes0, uint8_t bytes1)
{
	char buffer[8];
	
	int relHum = (bytes0 << 8) | bytes1; // store bytes into single var
	relHum = relHum / 10; // Calculate the whole number 
	int remainder = relHum % 10; // Calculate the fraction
	serial_print("RHUM= "); 
	itoa(relHum, buffer, 10); // Convert data to string
	serial_print(buffer);
	itoa(remainder, buffer, 10); // Convert data to string
	serial_print(".");
	serial_print(buffer);
	serial_print(", ");
}

/***********************************************************************
* Function insures that that the sun of all 16 humidity bits, 
* plus the sum of all 16 temperature bits, 
* matches the single byte checksum value
* If so, it prints the data. Otherwise, it'll print a checksum error.
***********************************************************************/
void checkSum()
{
	unsigned char sum;
	sum = bytes[0] + bytes[1] + bytes[2] + bytes[3];
	if(bytes[4] == sum)
	{
		//printHumidity(bytes[0], bytes[1]);
		//printTemperature(bytes[2], bytes[3]);
		serial_write(0, 0xAA);
		serial_write(0, bytes[0]);
		serial_write(0, bytes[1]);
		serial_write(0, bytes[2]);
		serial_write(0, bytes[3]);
		serial_write(0, bytes[4]);
		serial_write(0, 0x99);
		
	}
	else
	{
		serial_print("Checksum Error. \r");
	}
	
}

/***********************************************************************
* After the response signal has been received, the function
* Interprets and stores the 40-bit data that's transmitted as follows:
* While the line is HIGH ** 26-28 ms indicates a logical 0 
*                        ** 70 ms indicates a logical 1
* bytes[ 0-1 ] will contain humidity
* bytes[ 2-4 ] will contain temperature
* bytes[  5  ] will contain check sum
***********************************************************************/
void storeDataBytes()
{
	for (int i = 0; i < 5; i++) // 2B RH data, 2B T data, 1B checksum
	{
		getByte = 0; // reset data
		for (int j = 0; j < 8; j++) // 8 bits for each byte
		{
			getByte <<= 1; // mask
			while (digitalRead(DATA_PIN)); // Wait for sensor to drive LOW
			while (!digitalRead(DATA_PIN)); // Wait for sensor to drive HIGH
			
			_delay_us(30); // while in high, 26-30 ms indicates a logical 0
			
			// so more than that, indicates a logical 1
			if (digitalRead(DATA_PIN)) getByte |= 1; 
		}
		bytes[i] = getByte; // store byte
	}
}

/***********************************************************************
* Configures the specified pin to behave either as an input, 
* internal pull-up, or an output.
* pin: the number of the pin whose mode you wish to set
* mode: INPUT, OUTPUT, or INPUT_PULLUP
***********************************************************************/
void pinMode(int pin, char mode)
{
	// Not a port or not a pin
	if (pin > 53 || pin < 0)
	{
		serial_print("Error: Invalid PIN number (0 - 53). \r");
	}
	if (mode < 0 || mode > 2)
	{
		serial_print("Error: Invalid MODE (INPUT, OUTPUT, INPUT_PULLUP). \r");
	}

	if (mode == INPUT)
	{
		*(map[pin].ddr) &= ~(1 << map[pin].bitShift);
		*(map[pin].ddr + 1) &= ~(1 << map[pin].bitShift);
	}
	else if (mode == OUTPUT)
	{
		*(map[pin].ddr) |= (1 << map[pin].bitShift);
	}
	else if(mode == INPUT_PULLUP)
	{
		*(map[pin].ddr) &= ~(1 << map[pin].bitShift);
		*(map[pin].ddr + 1) |= (1 << map[pin].bitShift);
	}
	
}

/***********************************************************************
* Reads the value from a specified digital pin, either HIGH or LOW.
* pin: the number of the pin whose mode you wish to set
***********************************************************************/
int digitalRead(int pin)
{
	// Not a pin
	if (pin > 53 || pin < 0)
	{
		serial_print("Error: Invalid PIN number (0 - 53). \r");
	}
	
	return (*(map[pin].ddr - 1) & (1 << map[pin].bitShift)) ? 1 : 0;

}

/***********************************************************************
* Writes a HIGH or a LOW value to a digital pin.
* pin: the number of the pin whose mode you wish to set
* value: HIGH or LOW
***********************************************************************/
void digitalWrite(int pin, int value)
{
	// Not a pin or not a possible value
	if (pin > 53 || pin < 0)
	{
		serial_print("Error: Invalid PIN number (0 - 53). \r");
	}
	if (value < 0 || value > 1)
	{
		serial_print("Error: Invalid VALUE (HIGH, LOW). \r");
	}
	
	if (value == HIGH)
	{
		*(map[pin].ddr + 1) |= (1 << map[pin].bitShift);
	}
	else
	{
		*(map[pin].ddr + 1) &= ~(1 << map[pin].bitShift);
	}
}

/***********************************************************************
* Function configures the atmega 2560 for communication using the 
* specified port.
* port: port number (0, 1, 2, 3)
* speed: baud rate calculated depending on F_CPU
* config: framing parameters (Data bits / Parity / Stop bits)
***********************************************************************/
void serial_open(unsigned char port, long speed, int config)
{
	
	serial_port[port]->ucsrb = (1 << RXEN0) | (1 << TXEN0);
	serial_port[port]->ucsrc = config;
	speed = (F_CPU / 16 / speed - 1);
	serial_port[port]->ubrr = speed;
}

/***********************************************************************
* Function that waits for the write buffer to be available, then
* writes a byte value to the buffer.
* port: port number (0, 1, 2, 3)
* data: byte character
***********************************************************************/
void serial_write(unsigned char port, char data)
{
	while (!(UCSR0A & (1 << UDRE0)));	// Wait for TXN
	UDR0 = data;
}

/***********************************************************************
* Function that waits for an available character in the serial receive 
* buffer, then reads and returns the character. 
* port: port number (0, 1, 2, 3)
***********************************************************************/
char serial_read(unsigned char port)
{
	char data;
	while (!(UCSR0A & (1 << RXC0)));	// Wait for RXN
	data = UDR0;
	return data;
}

/***********************************************************************
* Function that writes a string to the terminal
* data: character buffer
***********************************************************************/
void serial_print(char* data)
{
	while(*data)
	{
		serial_write(0, *data++);
	}   
}