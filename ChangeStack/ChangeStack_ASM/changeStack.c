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

// Prototypes
//extern uint8_t * changeStack_ASM(uint8_t *pNewStack);

int main(void)
{
	uint8_t oldSREG = SREG;
	//oldSREG += 1;
	changeStack(3);
	/* Replace with your application code */
	while (1)
	{
		
	}
}


