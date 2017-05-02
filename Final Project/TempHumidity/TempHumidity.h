/*
 * TempHumidity.h
 *
 * Created: 3/21/2017 1:32:29 PM
 * Author : Eduardo Padilla	[padillae]
 * Member2: Sam Fenimore	[fenimoress]
 */ 


#ifndef TEMPHUMIDITY_H_
#define TEMPHUMIDITY_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Digital.h"
#include "Serial.h"

#define DATA_PIN 7

short int getByte;
short int bytes[5];

// Prototypes
extern char * itoa(int, char *, int);

void sensorSetup(void);
void resetState(void);
void printHumidity(uint8_t, uint8_t);
void printTemperature(uint8_t, uint8_t);
void storeDataBytes(void);
void checkSum(void);

#endif /* TEMPHUMIDITY_H_ */