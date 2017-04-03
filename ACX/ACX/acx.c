/*
 * acx.c
 *
 * Created: 3/21/2017 12:34:55 PM
 * Author:	E. Frank Barry
 * Edited:	Eduardo Padilla	[padillae]
 * Edited2:	Sam Fenimore	[fenimoress]
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/common.h>
#include <util/atomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include "acx.h"


//---------------------------------------------------
// Stack Control
//---------------------------------------------------
Stack stack[NUM_THREADS];

//---------------------------------------------------
// Stack Memory
//---------------------------------------------------


//---------------------------------------------------
// Thread Delay Counters
//---------------------------------------------------
Delay delay_counter[NUM_THREADS];

//---------------------------------------------------
// Exec State Variables
//---------------------------------------------------



//---------------------------------------------------
// Local Functions
//---------------------------------------------------


//---------------------------------------------------
// ACX Functions
//---------------------------------------------------

int main(void)
{
	x_init();
	while (1)
	{
		// Nothing for now
	}
}

/************************************************************************
* Initializes thread stack, stack control structures, canary values, 
* kernel status variables, system timer, thread delay counters, along 
* with an 8-bit Timer0 in CTC mode.                                                             
************************************************************************/
void x_init(void)
{
	cli();	// Disable interrupts
	
	// Setup for 8-bit Timer0
	TCCR0A = CTC_MODE;
	OCR0A = TOP;
	TIMSK0 = CMI_ENABLED;
	TCCR0B = GI_ENABLED;
	
	timer = 0; // CLear global timer
	
	// Clear status bytes
	disable_status = 0xFE;	// Disable all but Thread 0
	suspend_status = 0x00;
	delay_status = 0x00;
	
	// Clear counter
	delay_counter[0] = 0;
	delay_counter[1] = 0;
	delay_counter[2] = 0;
	delay_counter[3] = 0;
	delay_counter[4] = 0;
	delay_counter[5] = 0;
	delay_counter[6] = 0;
	delay_counter[7] = 0;
	
	x_thread_id = 0;		// Current thread
	x_thread_mask = 0x01;	// Bit 0 set corresponds to Thread 0
	
	// Initialize stack control table
	stack[0].stack_head = (byte *) THREAD0_BASE;
	stack[0].stack_base = (byte *) THREAD0_BASE;
	stack[1].stack_head = (byte *) THREAD1_BASE;
	stack[1].stack_base = (byte *) THREAD1_BASE;
	stack[2].stack_head = (byte *) THREAD2_BASE;
	stack[2].stack_base = (byte *) THREAD2_BASE;
	stack[3].stack_head = (byte *) THREAD3_BASE;
	stack[3].stack_base = (byte *) THREAD3_BASE;
	stack[4].stack_head = (byte *) THREAD4_BASE;
	stack[4].stack_base = (byte *) THREAD4_BASE;
	stack[5].stack_head = (byte *) THREAD5_BASE;
	stack[5].stack_base = (byte *) THREAD5_BASE;
	stack[6].stack_head = (byte *) THREAD6_BASE;
	stack[6].stack_base = (byte *) THREAD6_BASE;
	stack[7].stack_head = (byte *) THREAD7_BASE;
	stack[7].stack_base = (byte *) THREAD7_BASE;
	
	// Initialize stack canaries
	*((byte *) THREAD0_CANARY) = CANARY_VALUE;
	*((byte *) THREAD1_CANARY) = CANARY_VALUE;
	*((byte *) THREAD2_CANARY) = CANARY_VALUE;
	*((byte *) THREAD3_CANARY) = CANARY_VALUE;
	*((byte *) THREAD4_CANARY) = CANARY_VALUE;
	*((byte *) THREAD5_CANARY) = CANARY_VALUE;
	*((byte *) THREAD6_CANARY) = CANARY_VALUE;
	*((byte *) THREAD7_CANARY) = CANARY_VALUE;
	
	// Copy the call stack into Thread 0 stack area
	int i = 0;
	byte * thread0_stackTop = ((byte *) THREAD0_BASE);
	byte * sp_bottom = (byte *) BASE_START;
	for (; BASE_START - i >= SP; i++)
	{
		*(thread0_stackTop - i) = *(sp_bottom - i);
	}
	
	// Set SP to point to this stack area
	SP = (int) (THREAD0_BASE - i + 1);

	sei();	// Re-enable interrupts

	// return to caller.
	return;
}

/************************************************************************
* 
************************************************************************/
void x_delay(unsigned int interval)
{
	
}

/************************************************************************
*
************************************************************************/
unsigned long x_gtime()
{
	return 0;
}

/************************************************************************
*
************************************************************************/
void x_new(uint8_t thread_id, PTHREAD pthread, bool isEnabled)
{
	
}

/************************************************************************
*
************************************************************************/
void x_suspend(uint8_t thread_id)
{
	
}

/************************************************************************
*
************************************************************************/
void x_resume(uint8_t thread_id)
{
	
}

/************************************************************************
*
************************************************************************/
void x_disable(uint8_t thread_id)
{
	
}

/************************************************************************
*
************************************************************************/
void x_enable(uint8_t thread_id)
{
	
}

/************************************************************************
* TIMER0 ISR for compare match, that for now does nothing.
************************************************************************/
ISR (TIMER0_COMPA_vect)
{
	
}

