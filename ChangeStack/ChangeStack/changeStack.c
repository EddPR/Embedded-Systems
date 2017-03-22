/*
 * changeStack.c
 *
 * Created: 3/21/2017 8:42:52 PM
 *  Author: Eduardo
 */ 

//#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/common.h>
//#include <util/atomic.h>
//#include <stdlib.h>
//#include <stdint.h>

// Stack is implemented as growing from higher to lower memory locations
// The Stack Pointer must be set to point above 0x200,
// the initial value being the last address pf the internal SRAM.
// The Stack Pointer is decremented by 1 when data is pushed onto the stack, and
// THREE when the return address is pushed onto the stack.
// SPH	|	SP15	|	SP14	|	SP13	|	SP12	|	SP11	|	SP10	|	SP9		|	SP8		|
// SPL	|	SP7		|	SP6		|	SP5		|	SP4		|	SP3		|	SP2		|	SP1		|	SP0		|

// Prototypes
extern uint8_t * changeStack(uint8_t *pNewStack);
int sum(int, int);


int main(void)
{
	//changeStack(50);
	sum(3,2);
	while (1)
	{
		
	}
}

int sum(int x, int y){
	return x+y;
}

