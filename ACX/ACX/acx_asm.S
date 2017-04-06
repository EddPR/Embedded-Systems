
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
; Save thread context	; x -> r27:26   y -> r29:28   z -> r31:r30
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
		lds		r18, x_thread_id	; save thread ID
		lds		r19, x_thread_mask	; save thread mask
loadStack:
		ldi		r26, lo8(stack)		; load lower byte address of stack array (X)
		ldi 	r27, hi8(stack)		; load higher byte address of stack array (X+1)
storeSP:
		cli							; disable global interrupts
		ldi		r30, SPL			; save SPL in temporary register (Z) (5D?)
		ldi		r31, SPH			; save SPH in temporary register (Z+1) (5E?)
		sei							; enable global interrupts


;------------------------------------------------------------------------
; Schedule next thread -- must use caller save registers below here
;                         because we can reach this without saving regs
;                         if reached from k_new() function
;-------------------------------------------------------------------------
		.global	x_schedule
x_schedule:
		lds		r18, disable_status	; load disable status
		lds		r19, suspend_status	; load suspend status
		lds		r20, delay_status	; load delay status
		or		r18, r19			; compute available thread based on status
		or		r18, r20			; compute available thread based on status
		com		r18					; invert status mask
calcThreadMask:
		; Test for bit2mask8 is working
		; Possibly rework code in isThreadReady, if x_thread_mask isn't already calculated
		lds		r24, x_thread_id	; load parameter for bit2mask8
		call	bit2mask8			; relative call subroutine
		;sts		x_thread_mask, r24	; x_thread_mask is set to bit2mask8 returned mask
loadThread:
		lds		r21, x_thread_id	; save thread ID
		;lds		r22, x_thread_mask	; save thread mask
		mov		r22, r24			; save thread mask calculated in bit2mask8
checkThread:
		inc		r21					; increment x_thread_id
		cpi		r21, 8				; check if x_thread_id is 8
		brne	nextAvailable		; if x_thread_id != 8, continue on, otherwise clear
clear:
		clr		r21					; if x_thread_id == 8, set to 0 and
		ldi		r22, 1 				; set x_thread_mask to 1
		jmp		specialCase			; you shouldn't shift the pre-set mask in this case
nextAvailable:
		lsl		r22					; left shift x_thread_mask (next thread)
specialCase:
		cp		r22, r18			; check if next thread is equal to thread status
		brne	checkThread			; if not, check the next thread, otherwise
saveThread:
		cli							; disable global interrupts
		sts		x_thread_id, r21	; store new x_thread_id
		sts		x_thread_mask, r22	; store new x_thread_mask
		ldi		r26, lo8(stack)		; load lower byte address of stack array (X)
		ldi 	r27, hi8(stack)		; load higher byte address of stack array (X+1)
charOffset:
		lsl		r21					; working towards multiplying x_thread_id by 4...
		lsl		r21					; to create a 4 bit offset (x_thread_id * 4)
		add		r26, r21			; increment stack by ofset
		adc		r27, 0				; pull in the carry
updateSP:
		sts		SPL, r27			; update SPL to new thread context
		sts		SPH, r26			; update SPH to new thread context
		sei							; enable global interrupts

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
		mov		r23, r24				; copy paraqmeter, now is count	
		ldi		r24, 1					; mask beggins with a 1
		cpi		r23, 0					; if param is 0, return
		breq	finish
loop:	
		lsl		r24						; shift mask left
		dec		r23						; decrement count
		breq	finish					; if count is zero, return
		brne	loop					; if count > 0, loop
finish:
		ret