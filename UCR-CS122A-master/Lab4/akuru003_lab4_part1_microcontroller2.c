/*	Partner(s) Name & E-mail: Abraham Kuruvila akuru003@ucr.edu No Partner
 *	Lab Section: 21
 *	Assignment: Lab 4 Part 1
 *	Exercise Description:  Send blinking val to servant to ouput on portc
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <stdint.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdbool.h> 
#include <string.h> 
#include <math.h> 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <avr/eeprom.h> 
#include <avr/portpins.h> 
#include <usart_ATmega1284.h>
#include <avr/pgmspace.h> 
//FreeRTOS include files 
#include "FreeRTOS.h" 
#include "task.h" 
#include "keypad.h"
#include "croutine.h" 
#include "lcd.h"
int counter = 0;
int counter2 = 0;
int counter3 = 0;
unsigned char temp = 0x00;
int time = 50;
int seconds = 0;

// Bit-access function
/*
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}
*/

// Master code
void SPI_MasterInit(void) {
	// Set DDRB to have MOSI, SCK, and SS as output and MISO as input
	// Set SPCR register to enable SPI, enable master, and use SCK frequency
	//   of fosc/16  (pg. 168)
	// Make sure global interrupts are enabled on SREG register (pg. 9)
	// Set MOSI, SCK as Output
	// Set DDRB to have MOSI, SCK, and SS as output and MISO as input
	DDRB = 0xBF; PORTB = 0x40;
	// Set SPCR register to enable SPI, enable master, and use SCK frequency
	SPCR |= (1<<SPE) | (1<<MSTR) | (1<<SPR0);
	//   of fosc/16  (pg. 168)
	// Make sure global interrupts are enabled on SREG register (pg. 9)
	SREG =0x80;
}

void SPI_MasterTransmit(unsigned char cData) {
	// data in SPDR will be transmitted, e.g. SPDR = cData;
	// set SS low
	  // set SS low
	  PORTB = SetBit(PORTB,4,0);
	  // data in SPDR will be transmitted, e.g. SPDR = cData;
	  SPDR = cData;
	  while(!(SPSR & (1<<SPIF))) { // wait for transmission to complete
		  ;
	  }
	  // set SS high
	  PORTB = SetBit(PORTB,4,1);
	
}

// Servant code
void SPI_ServantInit(void) {
	// set DDRB to have MISO line as output and MOSI, SCK, and SS as input
	// set SPCR register to enable SPI and enable SPI interrupt (pg. 168)
	// make sure global interrupts are enabled on SREG register (pg. 9)
	  // set DDRB to have MISO line as output and MOSI, SCK, and SS as input
	  DDRB = 0x40; PORTB = 0xBF;
	  // set SPCR register to enable SPI and enable SPI interrupt (pg. 168)
	  SPCR |= (1<<SPE) | (1<<SPIE);
	  // make sure global interrupts are enabled on SREG register (pg. 9)
	  SREG = 0x80;
}
unsigned receivedData;
ISR(SPI_STC_vect) { // this is enabled in with the SPCR register?s ?SPI
  // Interrupt Enable?
        // SPDR contains the received data, e.g. unsigned char receivedData =
// SPDR;
        receivedData = SPDR;
}

enum States {start} State;
	
void LEDS_Init(){
	State = start;
}
void LEDS_Tick()
{
	switch(State)
	{
		case start:
		PORTC = receivedData;
		break;
	}
	switch(State)
	{
		case start:
		break;
	}
}
void LedSecTask()
{
	LEDS_Init();
   for(;;) 
   { 	
	LEDS_Tick();
	vTaskDelay(100); 
   } 
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	
}	

int main(void) 
{ 
   DDRA = 0xFF; PORTA = 0x00;
   DDRB = 0x40; PORTB = 0xBF;
   DDRC = 0xFF; PORTC = 0x00;
   DDRD = 0xFF; PORTD = 0x00;
   
   //LCD_init();
   SPI_ServantInit();
   //Start Tasks  
   StartSecPulse(1);
   //RunSchedular 
   vTaskStartScheduler();
   return 0; 
}



