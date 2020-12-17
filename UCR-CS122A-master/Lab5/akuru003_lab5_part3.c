/*	Partner(s) Name & E-mail: Abraham Kuruvila akuru003@ucr.edu No Partner
 *	Lab Section: 21
 *	Assignment: Lab 5 Part 3
 *	Exercise Description: Sending patterns to multiple shift registers with shared lines
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
unsigned char temp1 = 0x00;
int counter2 = 0;
int counter11 = 0;
unsigned char pat11 = 0x80;
void setGround(int counter4,unsigned char x)
{	
	if(counter4 == 1)
	{
		for(int i = 7; i >= 0; i--)
		{
			PORTA = 0x08;
			PORTA |= ((x >> i) & 0x01);   // for first 4
			PORTA |= (((x >> i) << 4) & 0x10);  // for next 4
			PORTA |= 0x44;
		}
		PORTA |= 0x02;
		PORTA = 0x00;
	}
	else
	{
		for(int i = 7; i >= 0; i--)
		{
			PORTA = 0x80;
			PORTA |= ((x >> i) & 0x01);   // for first 4
			PORTA |= (((x >> i) << 4) & 0x10);  // for next 4
			PORTA |= 0x44;
		}
		PORTA |= 0x20;
		PORTA = 0x00;
	}
}


enum States1 {start1, s11, s21, s31} State1;
void LEDS_Init1(){
	State1 = start1;
}

void LEDS_Tick1()
{
	switch(State1)
	{
		case start1:
		temp1 = 0x00;
		setGround(2,temp1);
		if((GetBit(PINB, 2) == 0) && (GetBit(PINB, 3) == 0))
		{
			State1 = s11;
		}
		else
		{
			State1 = start1;
		}
		break;
		
		case s11:
		setGround(2,pat11);
		if(pat11 == 0x01)
		{
			pat11 = 0x80;
		}
		pat11 = pat11 >> 1;
		if((GetBit(PINB, 2) == 0))
		{
			State1 = s21;
		}
		if((GetBit(PINB, 2) == 0) && (GetBit(PINB, 3) == 0))
		{
			State1 = start1;
			temp1 = 0x00;
			setGround(2,temp1);
		}
		break;
		
		case s21:
		if(counter2 == 0)
		{
			counter2 = 1;
			temp1 = 0xAA;
			setGround(2,temp1);
		}
		else
		{
			counter2 = 0;
			temp1 = 0x55;
			setGround(2,temp1);
		}
		if((GetBit(PINB, 2) == 0))
		{
			State1 = s31;
		}
		if((GetBit(PINB, 3) == 0))
		{
			State1 = s11;
		}
		if((GetBit(PINB, 2) == 0) && (GetBit(PINB, 3) == 0))
		{
			State1 = start1;
			temp1 = 0x00;
			setGround(2,temp1);
		}
		break;
		
		case s31:
		if(counter2 == 0)
		{
			counter2 = 1;
			temp1 = 0xFF;
			setGround(2,temp1);
		}
		else
		{
			counter2 = 0;
			temp1 = 0x00;
			setGround(2,temp1);
		}
		if((GetBit(PINB, 3) == 0))
		{
			State1 = s21;
		}
		if((GetBit(PINB, 2) == 0) && (GetBit(PINB, 3) == 0))
		{
			State1 = start1;
			temp1 = 0x00;
			setGround(2,temp1);
		}
		break;
	}
}
enum States {start, s1, s2, s3 , swait, swait1, swait2, swait3, swait4, swait5} State;
void LEDS_Init(){
	State = start;
}
void LEDS_Tick()
{
	switch(State)
	{
		case start:
		temp = 0x00;
		setGround(1,temp);
		if((GetBit(PINB, 0) == 0) && (GetBit(PINB, 1) == 0))
		{
			State = swait;
		}
		else
		{
			State = start;
		}
		break;
		
		case swait:
		if((GetBit(PINB, 0) == 1) && (GetBit(PINB, 1) == 1))
		{
			State = s1;
		}
		break;
		
		case swait3:
		if((GetBit(PINB, 0) == 1) && (GetBit(PINB, 1) == 1))
		{
			State = start;
		}
		break;
		
		case s1:
		setGround(1,pat1);
		if(pat1 == 0x01)
		{
			pat1 = 0x80;
		}
		pat1 = pat1 >> 1;
		if((GetBit(PINB, 0) == 0))
		{
			State = swait1;
		}
		if((GetBit(PINB, 0) == 0) && (GetBit(PINB, 1) == 0))
		{
			State = swait3;
			temp = 0x00;
			setGround(1,temp);
		}
		break;
		
		case swait1:
		if((GetBit(PINB, 0) == 1))
		{
			State = s2;
		}
		break;
		
		case s2:
		if(counter == 0)
		{
			counter = 1;
			temp = 0xAA;
			setGround(1,temp);
		}
		else
		{
			counter = 0;
			temp = 0x55;
			setGround(1,temp);
		}
		if((GetBit(PINB, 0) == 0))
		{
			State = swait4;
		}
		if((GetBit(PINB, 1) == 0))
		{
			State = swait5;
		}
		if((GetBit(PINB, 0) == 0) && (GetBit(PINB, 1) == 0))
		{
			State = swait3;
			temp = 0x00;
			setGround(1,temp);
		}
		break;
		
		case swait4:
		if((GetBit(PINB, 0) == 1))
		{
			State = s3;
		}
		break;
		case swait5:
		if((GetBit(PINB, 1) == 1))
		{
			State = s1;
		}
		break;
		
		case s3:
		if(counter1 == 0)
		{
			counter1 = 1;
			temp = 0xFF;
			setGround(1,temp);
		}
		else
		{
			counter1 = 0;
			temp = 0x00;
			setGround(1,temp);
		}
		if((GetBit(PINB, 1) == 0))
		{
			State = swait2;
		}
		if((GetBit(PINB, 0) == 0) && (GetBit(PINB, 1) == 0))
		{
			State = swait3;
			temp = 0x00;
			setGround(1,temp);
		}
		break;
		
		case swait2:
		if((GetBit(PINB, 1) == 1))
		{
			State = s2;
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
	vTaskDelay(100); 
   } 
}
void LedSecTask1()
{
	LEDS_Init1();
	for(;;)
	{
		LEDS_Tick1();
		vTaskDelay(100);
	}
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(LedSecTask1, (signed portCHAR *)"LedSecTask1", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	
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



