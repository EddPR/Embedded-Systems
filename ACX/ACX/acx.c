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
#include <util/delay.h>
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
byte x_thread_id;
byte x_thread_mask;

//---------------------------------------------------
// Thread Delay Counters
//---------------------------------------------------
volatile Delay x_thread_delay[NUM_THREADS];
unsigned long timer;

//---------------------------------------------------
// Exec State Variables
//---------------------------------------------------
byte delay_status;
byte suspend_status;
byte disable_status;

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

/************************************************************************
* Function is meant to test x_new(). It creates a local variable 'i'
* for every thread created using x_new().
************************************************************************/
void testThread(void)
{
	volatile int i = 0;
	while (1)
	{
		i++;
		x_yield();
	}
}

/************************************************************************
* Function sets up an LED on Digital Pin 37, then toggles it at a given
* rate
************************************************************************/
void thread0()
{
	DDRC |= 0x01;	// PORTC.0 -> DP37
	while(1) 
	{
		PORTC ^= 0x01;	// Toggle LED
		x_delay(250);
	}
}

/************************************************************************
* Function sets up an LED on Digital Pin 36, then toggles it at a given
* rate
************************************************************************/
void thread1()
{
	DDRC |= 0x02;	// PORTC.1 -> DP36
	while(1)
	{
		PORTC ^= 0x02;	// Toggle LED
		x_delay(900);
	}
}

/************************************************************************
* Function sets up an LED on Digital Pin 37, then toggles it at a given
* rate
************************************************************************/
void thread2()
{
	DDRC |= 0x04;	// PORTC.2 -> DP35
	while(1)
	{
		PORTC ^= 0x04;	// Toggle LED
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
* Function causes a thread to place itself in a "blocked" condition for 
* a specified number of "system ticks". If there are other READY threads, 
* then one of them will be selected by the scheduler to be placed into 
* execution
* ticks:	delay value
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
* Function is used to assign a code (function) pointer to a particular 
* thread ID. 
* threadId:		ID of the thread to which "newthread' will be assigned (0-7)
* newthread:	function pointer that takes no params and return nothing.
* isEnabled:	initial status of thread - 1 -> enabled | 0 -> disabled
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
	
	if (isEnabled) 
	{
		disable_status &= ~(1 << threadID);	// Enable thread
	}
	else 
	{
		disable_status |= (1 << threadID);			// Disable thread
	}
}

/************************************************************************
* Function suspends the specified thread by setting its suspend status bit
* tid:	thread ID
************************************************************************/
void x_suspend(uint8_t tid)
{
	/*uint8_t temp = SREG;   // save SREG --holds global interrupt enable bit
	cli();  // disable interrupts
	do the atomic access
	SREG = temp;   // restore interrupt state*/
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) 
	{
		suspend_status |= (1 << tid);
	}
}

/************************************************************************
* Function resumes specified thread by clearing its suspend status bit
* tid:	thread ID
************************************************************************/
void x_resume(uint8_t tid)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		suspend_status &= ~(1 << tid);
	}
}

/************************************************************************
* Function disables specified thread by setting its disable status bit
* tid:	thread ID
************************************************************************/
void x_disable(uint8_t tid)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		disable_status |= (1 << tid);
	}
}

/************************************************************************
* Function enables specified thread by clearing its disable status bit
* tid:	thread ID
************************************************************************/
void x_enable(uint8_t tid)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		disable_status &= ~(1 << tid);
	}
}

/************************************************************************
* TIMER0 ISR for compare match, that decrements delays for all threads
* and enables threads whose delay value reaches 0.
************************************************************************/
ISR (TIMER0_COMPA_vect)
{
	cli();	// Disable interrupts
	timer++;	// Increment global timer
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

