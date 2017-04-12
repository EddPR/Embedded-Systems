/*
 * acx.h
 *
 * Created: 3/20/2014 11:08:37 AM
 * Author:	E. Frank Barry
 * Edited:	Eduardo Padilla	[padillae]
 * Edited2:	Sam Fenimore	[fenimoress]
 */


#ifndef ACX_H_
#define ACX_H_

// C and Assembly definitions

// define canary value
#define CANARY_VALUE			0xAA

// includes max number of threads and number used
#define MAX_THREADS				8
#define NUM_THREADS				8

// define stack sizes for each thread
#define DEFAULT_STACK_SIZE		128
#define THREAD0_STACK_SIZE		DEFAULT_STACK_SIZE
#define THREAD1_STACK_SIZE		DEFAULT_STACK_SIZE
#define THREAD2_STACK_SIZE		DEFAULT_STACK_SIZE
#define THREAD3_STACK_SIZE		DEFAULT_STACK_SIZE
#define THREAD4_STACK_SIZE		DEFAULT_STACK_SIZE
#define THREAD5_STACK_SIZE		DEFAULT_STACK_SIZE
#define THREAD6_STACK_SIZE		DEFAULT_STACK_SIZE
#define THREAD7_STACK_SIZE		DEFAULT_STACK_SIZE

// define total stack memory size using thread stack sizes
#define TOTAL_THREAD_SIZE		(DEFAULT_STACK_SIZE * MAX_THREADS)

// define offsets to each thread's stack base
#define BASE_START				0x21FF
#define INIT_OFFSET				DEFAULT_STACK_SIZE
#define THREAD0_BASE			(BASE_START - INIT_OFFSET)
#define THREAD1_BASE			(THREAD0_BASE - THREAD0_STACK_SIZE)
#define THREAD2_BASE			(THREAD1_BASE - THREAD1_STACK_SIZE)
#define THREAD3_BASE			(THREAD2_BASE - THREAD2_STACK_SIZE)
#define THREAD4_BASE			(THREAD3_BASE - THREAD3_STACK_SIZE)
#define THREAD5_BASE			(THREAD4_BASE - THREAD4_STACK_SIZE)
#define THREAD6_BASE			(THREAD5_BASE - THREAD5_STACK_SIZE)
#define THREAD7_BASE			(THREAD6_BASE - THREAD6_STACK_SIZE)


// define thread IDs for each thread
#define THREAD0_ID				0
#define THREAD1_ID				1
#define THREAD2_ID				2
#define THREAD3_ID				3
#define THREAD4_ID				4
#define THREAD5_ID				5
#define THREAD6_ID				6
#define THREAD7_ID				7

// define thread context size 
#define THREAD_CONTEXT_SIZE		128

// define canary values for stack checking
#define THREAD0_CANARY	(THREAD1_BASE + 1)
#define THREAD1_CANARY	(THREAD2_BASE + 1)
#define THREAD2_CANARY	(THREAD3_BASE + 1)
#define THREAD3_CANARY	(THREAD4_BASE + 1)
#define THREAD4_CANARY	(THREAD5_BASE + 1)
#define THREAD5_CANARY	(THREAD6_BASE + 1)
#define THREAD6_CANARY	(THREAD7_BASE + 1)
#define THREAD7_CANARY	(THREAD7_BASE - THREAD7_STACK_SIZE + 1)



#ifndef __ASSEMBLER__    // The following only apply to C files...

// macro to access the current thread id
#define x_getTID()		(x_thread_id)

// macros for stack
#define CALLEE_REGISTER_SPACE	18
#define RETURN_ADDRESS_SPACE	3
#define RESERVED_SPACE			CALLEE_REGISTER_SPACE + RETURN_ADDRESS_SPACE - 1

// macros for Timer
#define CTC_MODE		0x02
#define TOP				250		// Representative of 1 msec
#define CMI_ENABLED		0x02
#define GI_ENABLED		0x03	

// Data type for a byte
typedef uint8_t			byte;

// global timer field
int timer;

// thread fields
byte x_thread_id;
byte x_thread_mask;

// status fields
byte delay_status;
byte suspend_status;
byte disable_status;

//---------------------------------------------------------------------------
// PTHREAD is a type that represents how threads are called--
// It is just a pointer to a function returning void
// that is a passed an int and a char * as parameters.
//---------------------------------------------------------------------------
typedef void			(*PTHREAD)(void);

//---------------------------------------------------------------------------
// This union is used to provide access to individual bytes of a thread address
//---------------------------------------------------------------------------
typedef union {
		PTHREAD thread;
		byte address[3];
	} PTU; 
	// PTU u;
	// u.thread = newThread;
	// u.add[0] -> L
	//      [1] -> M
	//      [2] -> H

//---------------------------------------------------------------------------
// This type is used for entries in the stack control table
//---------------------------------------------------------------------------
typedef struct {
		byte * head;
		byte * base;
	} Stack;

//---------------------------------------------------------------------------
// This type is used for the delay length of a thread
//---------------------------------------------------------------------------
typedef	uint16_t		Delay;


//----------------------------------------------------------------------------
// ACX Function prototypes
//----------------------------------------------------------------------------
void			x_init(void);
void			x_delay(unsigned int);
unsigned long	x_gtime(void);
extern void		x_schedule(void);
void			x_new(uint8_t, PTHREAD , bool);
extern void		x_yield(void);
extern uint8_t	bit2mask8(uint8_t);
void			x_suspend(uint8_t);
void			x_resume(uint8_t);
void			x_disable(uint8_t);
void			x_enable(uint8_t);


#endif


#endif /* ACX_H_ */
