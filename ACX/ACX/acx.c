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
volatile Delay x_thread_delay[NUM_THREADS];

//---------------------------------------------------
// Exec State Variables
//---------------------------------------------------



//---------------------------------------------------
// Local Functions
//---------------------------------------------------
void testThread(void);
void thread0(void);
void thread1(void);
void thread2(void);

//---------------------------------------------------
// ACX Functions
//---------------------------------------------------

int main(void)
{
	/*volatile int j = 0;
	x_init();
	x_new(1, testThread, true);
	x_new(0, testThread, true);*/
	x_init();
	x_new(2, (PTHREAD)thread2, 1);
	x_new(1, (PTHREAD)thread1, 1);
	x_new(0, (PTHREAD)thread0, 1);
	while (1)
	{
		/*j++;
		x_yield();*/
	}
}

void testThread(void)
{
	volatile int i = 0;
	while (1)
	{
		i++;
		x_yield();
	}
}

void thread0()
{
	DDRC |= 0x01;
	while(1) 
	{
		PORTC ^= 0x01;
		x_delay(250);
	}
}

void thread1()
{
	DDRC |= 0x02;
	while(1)
	{
		PORTC ^= 0x02;
		x_delay(900);
	}
}

void thread2()
{
	DDRC |= 0x04;
	while(1)
	{
		PORTC ^= 0x04;
		x_delay(65);
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
	x_thread_delay[0] = 0;
	x_thread_delay[1] = 0;
	x_thread_delay[2] = 0;
	x_thread_delay[3] = 0;
	x_thread_delay[4] = 0;
	x_thread_delay[5] = 0;
	x_thread_delay[6] = 0;
	x_thread_delay[7] = 0;
	
	x_thread_id = 0;		// Current thread
	x_thread_mask = 0x01;	// Bit 0 set corresponds to Thread 0
	
	// Initialize stack control table
	stack[0].head = (byte *) THREAD0_BASE;
	stack[0].base = (byte *) THREAD0_BASE;
	stack[1].head = (byte *) THREAD1_BASE;
	stack[1].base = (byte *) THREAD1_BASE;
	stack[2].head = (byte *) THREAD2_BASE;
	stack[2].base = (byte *) THREAD2_BASE;
	stack[3].head = (byte *) THREAD3_BASE;
	stack[3].base = (byte *) THREAD3_BASE;
	stack[4].head = (byte *) THREAD4_BASE;
	stack[4].base = (byte *) THREAD4_BASE;
	stack[5].head = (byte *) THREAD5_BASE;
	stack[5].base = (byte *) THREAD5_BASE;
	stack[6].head = (byte *) THREAD6_BASE;
	stack[6].base = (byte *) THREAD6_BASE;
	stack[7].head = (byte *) THREAD7_BASE;
	stack[7].base = (byte *) THREAD7_BASE;
	
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
void x_delay(unsigned int ticks)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)	// Disable Interrupts
	{
		x_thread_delay[x_thread_id] = ticks;	// Copies ticks to calling thread's delay counter
		disable_status |= x_thread_mask;		// Sets x_delay_status bit corresponding to calling thread's ID
	}
	x_yield();		// Reschedule 
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
* threadId: ID of the thread to which "newthread' will be assigned (0-7)
* newthread: function pointer that takes no params and return nothing.
* isEnabled: initial status of thread - 1 -> enabled | 0 -> disabled
************************************************************************/
void x_new(byte threadID, PTHREAD newthread, bool isEnabled)
{
	PTU u;
	u.thread = newthread;
	
	*(stack[threadID].head) = u.address[0]; // LOW
	*(stack[threadID].head - 1) = u.address[1]; // MID
	*(stack[threadID].head - 2) = u.address[2]; // HIGH
	
	stack[threadID].head = (byte *) ((int) (stack[threadID].head - (RESERVED_SPACE)));	// Reserve Register Space
	//*(stack[threadID].head) = 1;			// Simple marker in stack
	
	x_thread_mask = bit2mask8(threadID);	// Mask with 1 << threadID
	
	if (isEnabled) 
	{
		disable_status &= ~(x_thread_mask);	// Enable thread
	}
	else 
	{
		disable_status |= x_thread_mask;			// Disable thread
	}
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
	cli();	// Disable interrupts
	for (int i = 0; i < NUM_THREADS; i++)
	{
		if (x_thread_delay[i] > 0)	// If count is non-zero
		{
			x_thread_delay[i]--;	// Decrement count
			
			if (x_thread_delay[i] == 0)	// If current x_thread_id isn't disabled
			{
				disable_status &= ~(1 << i);	// Enable thread
			}
		}
		
	}
	sei();	// Enable interrupts
}

