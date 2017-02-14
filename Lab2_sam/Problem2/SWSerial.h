/*
 * SWSerial.h
 *
 * Created: 2/12/2017 4:15:43 PM
 *  Author: fenim
 */ 


#ifndef SWSERIAL_H_
#define SWSERIAL_H_

void init_sw_serial(int rx_pin, int tx_pin, long baudrate, int framing);
void sw_serial_putc(char c);
void sw_serial_puts(char *str);

#endif /* SWSERIAL_H_ */