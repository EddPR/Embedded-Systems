/*
 * delay_usec.s
 *
 * Created: 1/24/2017 2:18:48 PM
 * Author : Eduardo Padilla [padillae]
 * Member2: Samuel Fenimore [fenimoress]
 */ 
		.section .text
		.global delay_usec
delay_usec:
							;preliminary call cycles	[4]
begin:
		sbiw	r24, 1		;is parameter 1?			[2]
		breq	prelim		;if equal to 0, go to end	[true:2 false:1]
		jmp		comp		;burn 2 cycles				[2]
comp:
		call	end			;burn 9 cycles				[4]
		jmp		begin		;complete remaining cycles	[2]
prelim:
		jmp		almost		;burn 2 cycles				[2]
almost:				
		nop					;burn a cycle				[1]
end:
		ret					;finish						[5]