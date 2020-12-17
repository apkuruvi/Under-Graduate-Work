/*	Partner(s) Name & E-mail: Abraham Kuruvila akuru003@ucr.edu No Partner
 *	Lab Section: 21
 *	Assignment: Lab 5 Part 2
 *	Exercise Description: Sending patterns to a shift register depending on button press
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
unsigned char temp = 0x00;
int counter = 0;
int counter1 = 0;
unsigned char pat1 = 0x80;
void setGround(unsigned char x)
{	
	for(int i = 7; i >= 0; i--)
	{
		PORTA = 0x88;
		PORTA |= ((x >> i) & 0x01);   // for first 4 
		PORTA |= (((x >> i) << 4) & 0x10);  // for next 4
		PORTA |= 0x44;
	}
	PORTA |= 0x22;
	PORTA = 0x00;
}

enum States {start, s1, s2, s3} State;	
void LEDS_Init(){
	State = start;
}
void LEDS_Tick()
{
	switch(State)
	{
		case start:
		temp = 0x00;
		setGround(temp);
		if((GetBit(PINB, 0) == 0) && (GetBit(PINB, 1) == 0))
		{
			State = s1;
		}
		else
		{
			State = start;
		}
		break;
		case s1:
		setGround(pat1);
		if(pat1 == 0x01)
		{
			pat1 = 0x80;
		}
		pat1 = pat1 >> 1;
		if((GetBit(PINB, 0) == 0))
		{
			State = s2;
		}
		if((GetBit(PINB, 0) == 0) && (GetBit(PINB, 1) == 0))
		{
			State = start;
			temp = 0x00;
			setGround(temp);
		}
		break;
		
		case s2:
		if(counter == 0)
		{
			counter = 1;
			temp = 0xAA;
			setGround(temp);
		}
		else
		{
			counter = 0;
			temp = 0x55;
			setGround(temp);
		}
		if((GetBit(PINB, 0) == 0))
		{
			State = s3;
		}
		if((GetBit(PINB, 1) == 0))
		{
			State = s1;
		}
		if((GetBit(PINB, 0) == 0) && (GetBit(PINB, 1) == 0))
		{
			State = start;
			temp = 0x00;
			setGround(temp);
		}
		break;
		
		case s3:
		if(counter1 == 0)
		{
			counter1 = 1;
			temp = 0xFF;
			setGround(temp);
		}
		else
		{
			counter1 = 0;
			temp = 0x00;
			setGround(temp);
		}
		if((GetBit(PINB, 1) == 0))
		{
			State = s2;
		}
		if((GetBit(PINB, 0) == 0) && (GetBit(PINB, 1) == 0))
		{
			State = start;
			temp = 0x00;
			setGround(temp);
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
	vTaskDelay(200); 
   } 
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	
}	

int main(void) 
{ 
   DDRA = 0xFF; PORTA = 0x00;
   DDRB = 0x00; PORTB = 0xFF;
   DDRC = 0xFF; PORTC = 0x00;
   DDRD = 0xFF; PORTD = 0x00;
   
   StartSecPulse(1);
   //RunSchedular 
   vTaskStartScheduler();
   return 0; 
}



