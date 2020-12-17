/*	Partner(s) Name & E-mail: Abraham Kuruvila akuru003@ucr.edu No Partner
 *	Lab Section: 21
 *	Assignment: Lab 4 Part 4
 *	Exercise Description: Sending pattern and speed numbers to multiple servants with lcd display
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
	//  PORTB = SetBit(PORTB,4,0);
	  // data in SPDR will be transmitted, e.g. SPDR = cData;
	  SPDR = cData;
	  while(!(SPSR & (1<<SPIF))) { // wait for transmission to complete
		  ;
	  }
	  // set SS high
	  //PORTB = SetBit(PORTB,4,1);
	
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

enum States {start, s0} State;
	
void LEDS_Init(){
	State = s0;
}
 char ar[32] = "Ptrn: 1 Spd: 1  uC: 1           ";
 unsigned char speed;
 unsigned char pattern;
 unsigned char mc; 
 unsigned char send = 0x11;
void LEDS_Tick()
{
	switch(State)
	{
		case start:
		LCD_DisplayString(1,ar);
		speed = 1;
		pattern = 1;
		mc = 1;
		State = s0;
		break;
		
		case s0:
		LCD_DisplayString(1,ar);
		PORTC = 0xEF;
		asm("nop");
		if (GetBit(PINC,0)==0) { ar[13] = '1';  speed = 1; }
		if (GetBit(PINC,1)==0) {ar[13] = '4';  speed = 4; }
		if (GetBit(PINC,2)==0) { ar[20] = '1'; mc = 1;}
		//if (GetBit(PINC,3)==0) { State = s12; }
			
		PORTC = 0xDF;
		asm("nop");
		if (GetBit(PINC,0)==0) { ar[13] = '2'; speed = 2;  }
		if (GetBit(PINC,1)==0) {ar[13] = '5'; speed = 5;}
		if (GetBit(PINC,2)==0) { ar[20] = '2'; mc = 2;}
		//if (GetBit(PINC,3)==0) { State = s01; }
		
		PORTC = 0xBF;
		asm("nop");
		if (GetBit(PINC,0)==0) { ar[13] = '3'; speed = 3;}
		if (GetBit(PINC,1)==0) { ar[13] = '6'; speed = 6;}
		if (GetBit(PINC,2)==0) { ar[20] = '3'; mc = 3;}
		//if (GetBit(PINC,3)==0) { State = s11; }
		
		PORTC = 0x7F;
		asm("nop");
		if (GetBit(PINC,0)==0) { ar[6] = '1'; 	pattern = 1;}
		if (GetBit(PINC,1)==0) {  ar[6] = '2'; 	pattern = 2;}
		if (GetBit(PINC,2)==0) {  ar[6] = '3'; 	pattern = 3;}
		if (GetBit(PINC,3)==0) {  ar[6] = '4'; 	pattern = 4;}
	    State = s0;
		break;
	}
	switch(State)
	{
		case start:
		break;
		
		case s0:
		if(pattern == 1)
		{
			send = send & 0x0F;
			send = send | 0x10;
		}
		if(pattern == 2)
		{
			send = send & 0x0F;
			send = send | 0x20;
		}
		if(pattern == 3)
		{
			send = send & 0x0F;
			send = send | 0x30;
		}
		if(pattern == 4)
		{
			send = send & 0x0F;
			send = send | 0x40;
		}
		if(speed == 1)
		{
			send = send & 0xF0;
			send = send | 0x01;
		}
		if(speed == 2)
		{
			send = send & 0xF0;
			send = send | 0x02;
		}
		if(speed == 3)
		{
			send = send & 0xF0;
			send = send | 0x03;
		}
		if(speed == 4)
		{
			send = send & 0xF0;
			send = send | 0x04;
		}
		if(speed == 5)
		{
			send = send & 0xF0;
			send = send | 0x05;
		}
		if(speed == 6)
		{
			send = send & 0xF0;
			send = send | 0x06;
		}
		if(mc == 1)
		{
		 PORTB = SetBit(PORTB, 4,0);
		 PORTA = SetBit(PORTA, 7,1);
		 PORTA = SetBit(PORTA, 6,1);
		 SPI_MasterTransmit(send);
		 PORTB = SetBit(PORTB, 4,1);
		 PORTA = SetBit(PORTA, 7,1);
		 PORTA = SetBit(PORTA, 6,1);
		}
		if(mc == 2)
		{
			PORTA = SetBit(PORTA, 6,0);
			PORTA = SetBit(PORTA, 7,1);
			PORTB = SetBit(PORTB, 4,1);
			SPI_MasterTransmit(send);
			PORTB = SetBit(PORTB, 4,1);
			PORTA = SetBit(PORTA, 7,1);
			PORTA = SetBit(PORTA, 6,1);
		}
		if(mc == 3)
		{
			PORTA = SetBit(PORTA, 7,0);
			PORTA = SetBit(PORTA, 6,1);
			PORTB = SetBit(PORTB, 4,1);
			SPI_MasterTransmit(send);
			PORTB = SetBit(PORTB, 4,1);
			PORTA = SetBit(PORTA, 7,1);
			PORTA = SetBit(PORTA, 6,1);
		}
		break;
	}
}
void LedSecTask()
{
	LEDS_Init();
   for(;;) 
   { 	
	LEDS_Tick();
	vTaskDelay(500); 
   } 
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	
}	

int main(void) 
{ 
   DDRA = 0xFF; PORTA = 0x00;
   DDRB = 0xBF; PORTB = 0x40;
   DDRC = 0xF0; PORTC = 0x0F;
   DDRD = 0xFF; PORTD = 0x00;
   
   LCD_init();
   SPI_MasterInit();
   //Start Tasks  
   StartSecPulse(1);
   //RunSchedular 
   vTaskStartScheduler();
   return 0; 
}



