
/*
 * x_stack.s
 *
 * Created: 3/21/2017 10:15:06 PM
 *  Author: Eduardo
 */ 

// Stack is implemented as growing from higher to lower memory locations
// The Stack Pointer must be set to point above 0x200,
// the initial value being the last address pf the internal SRAM.
// The Stack Pointer is decremented by 1 when data is pushed onto the stack, and
// THREE when the return address is pushed onto the stack.
// SPH	|	SP15	|	SP14	|	SP13	|	SP12	|	SP11	|	SP10	|	SP9		|	SP8		|
// SPL	|	SP7		|	SP6		|	SP5		|	SP4		|	SP3		|	SP2		|	SP1		|	SP0		|
#define __SFR_OFFSET 0
#include <avr/io.h>
#include <avr/common.h>


 		.section .text
		.global x_stack
x_stack:
							;first paramter is in r24
							;SP value is in SREG
save_context:
; perserve all general purpose registers and status register
		push	r0
		;in		r0, SREG
		cli
		push	r0
		push	r1
		;clr		r1
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
		push	r18
		push	r19
		push	r20
		push	r21
		push	r22
		push	r23
		push	r24
		push	r25
		push	r26
		push	r27
		push	r28
		push	r29
		push	r30
		push	r31
;perserve status register for last task
		in		r16, SREG	; save SREG in temporary register
		st		Z, r16		; save SREG
;perserve stack pinter for last task
		;ldi		r16, lo8(RAMEND)	; save SPL in temporary register
		;out		SPL, r16			; save SPL
		;ldi		r16, hi8(RAMEND)	; save SPH in temporary register
		;out		SPH, r16			; save SPH
		ldi		r16, lo8(RAMEND)		; save SPL in temporary register
		ldi		r17, hi8(RAMEND)		; save SPH in temporary register
		std		Z+0, r16				; save SPL
		std		Z+1, r17				; save SPH
; load stack pointer for next stack
		ldd		r16, Z+0				; read in new SPL value
		ldd		r17, Z+1				; read in new SPH value
; load status register for next task
		ld		r16, Z
;restore general purpose registers for next task
		pop		r31
		pop		r30
		pop		r29
		pop		r28
		pop		r27
		pop		r26
		pop		r25
		pop		r24
		pop		r23
		pop		r22
		pop		r21
		pop		r20
		pop		r19
		pop		r18
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
		pop		r1
		pop		r0
end:
		reti					;finish