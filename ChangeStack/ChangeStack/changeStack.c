/*
 * changeStack.c
 *
 * Created: 3/21/2017 8:42:52 PM
 *  Author: Eduardo
 *  Author2: Sam Fenimore
 */ 

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/common.h>
#include <util/atomic.h>
#include <stdlib.h>
#include <stdint.h>

// Stack is implemented as growing from higher to lower memory locations
// The Stack Pointer must be set to point above 0x200,
// the initial value being the last address pf the internal SRAM.
// The Stack Pointer is decremented by 1 when data is pushed onto the stack, and
// THREE when the return address is pushed onto the stack.
// SPH	|	SP15	|	SP14	|	SP13	|	SP12	|	SP11	|	SP10	|	SP9		|	SP8		|
// SPL	|	SP7		|	SP6		|	SP5		|	SP4		|	SP3		|	SP2		|	SP1		|	SP0		|

// Prototypes
uint8_t * changeStack(uint8_t *pNewStack);

int main(void)
{
	uint8_t buffer[128];
	for(int i = 0; i < 128; i++)
	{
		buffer[i] = i;
	}
	changeStack(buffer);
	while (1)
	{
		
	}
}

uint8_t * changeStack(uint8_t * pNewStack)
{
	cli();
	uint8_t * p = (uint8_t *) SP;
	int temp = ((int) SP - 0x21FF- 30);
	uint8_t * dp = (uint8_t *) &pNewStack[temp];

	for(int i = 1; ((int)&p[i])<0x21FF;i++)
	{
		dp[i] = p[i];
	}
	SP = *pNewStack;
	sei();
	return (uint8_t *) SP;
}
