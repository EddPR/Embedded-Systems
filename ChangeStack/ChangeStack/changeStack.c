/*
 * changeStack.c
 *
 * Created: 3/21/2017 8:42:52 PM
 * Author : Eduardo Padilla [padille]
 * Member2: Sam Fenimore	[fenimoress]
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/common.h>
#include <util/delay.h>
#include "Serial.h"

#define PORT			0
#define BAUD_RATE		19200
#define STACK_BOTTOM	0x21FF
#define BUFFER_SIZE		128
#define BINARY			2
#define DECIMAL			10
#define HEX				16

// Stack is implemented as growing from higher to lower memory locations
// The Stack Pointer must be set to point above 0x200,
// the initial value being the last address pf the internal SRAM.
// The Stack Pointer is decremented by 1 when data is pushed onto the stack, and
// THREE when the return address is pushed onto the stack.
// SPH	|	SP15	|	SP14	|	SP13	|	SP12	|	SP11	|	SP10	|	SP9		|	SP8		|
// SPL	|	SP7		|	SP6		|	SP5		|	SP4		|	SP3		|	SP2		|	SP1		|	SP0		|

SERIAL_REGS *serial_port[] = {
	(SERIAL_REGS *)(0xc0),	// serial port 0
	(SERIAL_REGS *)(0xc8),	// serial port 1
	(SERIAL_REGS *)(0xd0),	// serial port 2
	(SERIAL_REGS *)(0x130)	// serial port 3
};

char * buffer[BUFFER_SIZE];
int stackFrameSize;

// Prototypes
uint8_t * changeStack(uint8_t *pNewStack);
extern char * itoa(int, char *, int);
extern void x_yield(void);
extern void stuff(void);
void testStack(void);

int main(void)
{
	
	
	serial_open(PORT, BAUD_RATE, SERIAL_8N1);
	_delay_ms(500);
	
	while (1)
	{
		testStack();
	}
}

/***********************************************************************
* Function changes the stack pointer to the base address specified by 
* pNewStack, copy the return address onto the new stack and return to 
* the caller with the new stack address as the return value.
***********************************************************************/
uint8_t * changeStack(uint8_t *pNewStack)
{	
	char *stackTop = (char *) SP;	// The stack pointer is the top of the stack
	stackFrameSize = STACK_BOTTOM - (int) stackTop + 1; // Range between the top and bottom of stack
	uint8_t *newStackTop = pNewStack - stackFrameSize;	// The new stack top is based on the offset of old stack size.
	SP = (int) newStackTop;	// Set stack pointer to new stack
	
	for (int i = 0; i < stackFrameSize; i++)	// Copy everything over to new stack
	{
		newStackTop[i] = stackTop[i];
	}
	
	return newStackTop;	// Return a pointer to the new stack
}

/***********************************************************************
* Function is supposed to call changeStack to relocate the SP to the
* address of the passed buffer and print the old SP and the new SP. 
***********************************************************************/
void testStack()
{
	char stackPointerBuffer[8];
	char stackPointerBuffer2[8];
	int temp, temp2;
	char *newAddress;
	
	char buf[8];
	char buf2[stackFrameSize];
	char *stackPointer = (char *) SP;
	temp = (int) stackPointer;
	
	newAddress = (char *) changeStack((uint8_t *) (buffer + (BUFFER_SIZE - 1)));
	temp2 = (int) newAddress;
	
	serial_print("We are in the loop testing stackFrameSize: ");
	itoa(stackFrameSize, buf, DECIMAL);
	serial_print(buf);
	serial_print("\r");
	_delay_ms(100);
	
	serial_print("The value of the old SP is: 0x");
	itoa(temp, stackPointerBuffer, HEX);
	serial_print(stackPointerBuffer);
	serial_print(".\r");
	_delay_ms(100);
	
	serial_print("The value of the new SP is: 0x");
	itoa(temp2, stackPointerBuffer2, HEX);
	serial_print(stackPointerBuffer2);
	serial_print(".\r");
	_delay_ms(100);
	serial_print("\r");
	
	serial_print("The values on the stack are:\r");
	for (int i = 0; i < stackFrameSize; i++)
	{
		itoa(newAddress[-i], buf2, HEX);
		serial_print(buf2);
		serial_print("\r");
		_delay_ms(100);
	}
	serial_print("\r");
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
	
	serial_port[port]->ucsrb = enableR | enableT;
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
	switch (port) {
		case 0:
		while (!(UCSR0A & (1 << UDRE0)));	// Wait for TXN
		UDR0 = data;
		break;
		case 1:
		while (!(UCSR1A & (1 << UDRE1)));	// Wait for TXN
		UDR1 = data;
		break;
		case 2:
		while (!(UCSR2A & (1 << UDRE2)));	// Wait for TXN
		UDR2 = data;
		break;
		case 3:
		while (!(UCSR3A & (1 << UDRE0)));	// Wait for TXN
		UDR3 = data;
		break;
		default:
		while (!(UCSR0A & (1 << UDRE0)));	// Wait for TXN
		UDR0 = data;
		break;
	}
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

