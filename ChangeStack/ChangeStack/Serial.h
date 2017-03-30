/*
 * Serial.h
 *
 * Created: 3/28/2017 5:14:51 PM
 *  Author: Eduardo
 */ 


#ifndef SERIAL_H_
#define SERIAL_H_

#define DATABITS 1
#define STOPBITS 3
#define PARITYBITS 4

typedef struct {
	volatile uint8_t ucsra;
	volatile uint8_t ucsrb;
	volatile uint8_t ucsrc;
	volatile uint8_t rsvd;
	volatile uint16_t ubrr;
	volatile uint8_t udr;
} SERIAL_REGS;

// Data bits / Parity / Stop bits
#define SERIAL_5N1  (0x00 | (0 << DATABITS))
#define SERIAL_6N1  (0x00 | (1 << DATABITS))
#define SERIAL_7N1  (0x00 | (2 << DATABITS))
#define SERIAL_8N1  (0x00 | (3 << DATABITS))     // (the default)
#define SERIAL_5N2  (0x08 | (0 << DATABITS))
#define SERIAL_6N2  (0x08 | (1 << DATABITS))
#define SERIAL_7N2  (0x08 | (2 << DATABITS))
#define SERIAL_8N2  (0x08 | (3 << DATABITS))
#define SERIAL_5E1  (0x20 | (0 << DATABITS))
#define SERIAL_6E1  (0x20 | (1 << DATABITS))
#define SERIAL_7E1  (0x20 | (2 << DATABITS))
#define SERIAL_8E1  (0x20 | (3 << DATABITS))
#define SERIAL_5E2  (0x28 | (0 << DATABITS))
#define SERIAL_6E2  (0x28 | (1 << DATABITS))
#define SERIAL_7E2  (0x28 | (2 << DATABITS))
#define SERIAL_8E2  (0x28 | (3 << DATABITS))
#define SERIAL_5O1  (0x30 | (0 << DATABITS))
#define SERIAL_6O1  (0x30 | (1 << DATABITS))
#define SERIAL_7O1  (0x30 | (2 << DATABITS))
#define SERIAL_8O1  (0x30 | (3 << DATABITS))
#define SERIAL_5O2  (0x38 | (0 << DATABITS))
#define SERIAL_6O2  (0x38 | (1 << DATABITS))
#define SERIAL_7O2  (0x38 | (2 << DATABITS))
#define SERIAL_8O2  (0x38 | (3 << DATABITS))

// Prototypes
void serial_open(unsigned char, long, int);
void serial_write(unsigned char, char);
void serial_print(char *);


#endif /* SERIAL_H_ */