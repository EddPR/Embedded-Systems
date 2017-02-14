/*
 * SWSerial.h
 *
 * Created: 2/6/2017 12:38:18 AM
 * Author : Eduardo Padilla [padille]
 * Member2: Sam Fenimore	[fenimoress]
 */ 


#ifndef SWSERIAL_H_
#define SWSERIAL_H_

#define BAUD_1200		1200
#define BAUD_2400		2400
#define BAUD_4800		4800
#define BAUD_9600		9600
#define BAUD_14400		14400
#define BAUD_19200		19200
#define BAUD_28800		28800
#define BAUD_38400		38400
#define BAUD_57600		57600
#define BAUD_115200		115200
#define BAUD_250000		250000
#define BAUD_500000		500000
#define BAUD_1000000	1000000

#define BPS_1200	834
#define BPS_2400	417
#define BPS_4800	209
#define BPS_9600	104
#define BPS_14400	70
#define BPS_19200	52
#define BPS_28800	35
#define BPS_38400	26
#define BPS_57600	17
#define BPS_115200	9
#define BPS_250000	4
#define BPS_500000	2
#define BPS_1000000	1

#define PIN_53 53
#define PIN_52 52
#define PIN_51 51
#define PIN_50 50
#define PIN_10 10
#define PIN_11 11
#define PIN_12 12

#define TX_PIN 19

void sw_serial_putc(char);
void sw_serial_puts(char *);
void init_sw_serial(int, int, long, int);

#endif /* SWSERIAL_H_ */