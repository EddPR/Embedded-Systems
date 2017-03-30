
/*
 * x_yield.s
 *
 * Created: 3/28/2017 2:48:25 PM
 *  Author: padillae
 */ 
		.section .text
		.global x_yield
x_yield:
							;first paramter is in r24
							;SP value is in SREG
save_context:
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
; restore all callee-saved registers
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

		ret				; finish