
/*
 * x_stack.s
 *
 * Created: 3/21/2017 8:40:06 PM
 *  Author: Eduardo
 */ 

// Stack is implemented as growing from higher to lower memory locations
// The Stack Pointer must be set to point above 0x200, 
// the initial value being the last address pf the internal SRAM.
// The Stack Pointer is decremented by 1 when data is pushed onto the stack, and
// THREE when the return address is pushed onto the stack.
// SPH	|	SP15	|	SP14	|	SP13	|	SP12	|	SP11	|	SP10	|	SP9		|	SP8		|
// SPL	|	SP7		|	SP6		|	SP5		|	SP4		|	SP3		|	SP2		|	SP1		|	SP0		|

 		.section .text
		.global changeStack
changeStack:
							;first paramter is in r24
							;SP value is in SREG
begin:
		
end:
		ret					;finish