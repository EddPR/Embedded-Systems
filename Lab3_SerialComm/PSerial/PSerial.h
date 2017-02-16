/*
 * PSerial.h
 *
 * Created: 2/11/2017 2:16:15 PM
 *  Author: Eduardo
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>

#ifndef PSERIAL_H_
#define PSERIAL_H_

typedef struct {
	volatile uint8_t ucsra;
	volatile uint8_t ucsrb;
	volatile uint8_t ucsrc;
	volatile uint8_t rsvd;
	volatile uint8_t ubrrL;
	volatile uint8_t ubrrH;
	volatile uint8_t udr;
} SERIAL_REGS;

//SERIAL_REGS *serial_port[];

void PSerial_open(unsigned char, long, int);
char PSerial_read(unsigned char);
void PSerial_write(unsigned char, char);

#endif /* PSERIAL_H_ */