
/*
 * acx_asm.S
 *
 * Created: 3/20/2014 11:08:37 AM
 * Author:	E. Frank Barry
 * Edited:	Eduardo Padilla	[padillae]
 * Edited2:	Sam Fenimore	[fenimoress]
 */

 #define __SFR_OFFSET 0
 #include <avr/io.h>
 #include <avr/common.h>
 #include "acx.h"


		.section .text
		
		.global x_yield
x_yield:
;------------------------------------------------------------------------
; Save thread context
;-------------------------------------------------------------------------
; perserve all callee-saved registers
		push	r2
		push	r3
		push	r4
		push	r5
		push	r6
		push	r7
		push	r8
		push	r9
		push	r10
		push	r11
		push	r12
		push	r13
		push	r14
		push	r15
		push	r16
		push	r17
		push	r28
		push	r29

getThread:
		lds		r18, x_thread_id
		lds		r19, x_thread_mask
storeSP:
		ldi		r20, lo8(RAMEND)	; save SPL in temporary register
		out		SPL, r20			; save SPL
		ldi		r16, hi8(RAMEND)	; save SPH in temporary register
		out		SPH, r20			; save SPH


;------------------------------------------------------------------------
; Schedule next thread -- must use caller save registers below here
;                         because we can reach this without saving regs
;                         if reached from k_new() function
;-------------------------------------------------------------------------
		.global	x_schedule
x_schedule:

; your code here



;---------------------------------------------------
; Restore context of next READY thread
;---------------------------------------------------
restore:
		pop		r29
		pop		r28
		pop		r17
		pop		r16
		pop		r15
		pop		r14
		pop		r13
		pop		r12
		pop		r11
		pop		r10
		pop		r9
		pop		r8
		pop		r7
		pop		r6
		pop		r5
		pop		r4
		pop		r3
		pop		r2

	// Return to next thread
		ret

//----------------------------------------------------------------------------------------------
//  Function: bit2mask8
//
//  Description:   Returns a bit mask corresponding to thread ID parameter.
//
//  Input params:  uint8_t id  -- must by integer between 0 and 7
//  Returns:       mask - a bit mask having a single bit set corresponding to input ID
//						  (interpreted as bit number)
//-----------------------------------------------------------------------------------------------
		.global bit2mask8
bit2mask8:
		;r24 containts thread ID input
		
		
		ret
