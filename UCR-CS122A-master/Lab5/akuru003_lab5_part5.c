/*	Partner(s) Name & E-mail: Abraham Kuruvila akuru003@ucr.edu No Partner
 *	Lab Section: 21
 *	Assignment: Lab 5 Part 5
 *	Exercise Description: Sending game to daisy chained registers
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
unsigned short y = 0x0001;
unsigned short z = 0x8000;
unsigned short game = 0x0000;

void setGround(unsigned short x)
{	
	for(int i = 15; i >= 0; i--)
	{
		PORTA = 0x88;
		PORTA |= ((x >> i) & 0x0001);   // for first 4
		PORTA |= 0x44;
		PORTA |= 0x22;
	}
	PORTA |= 0x22;
	PORTA = 0x00;
}

enum States1 {start1} State1;
void LEDS_Init1(){
	State1 = start1;
}
void LEDS_Tick1()
{
	switch(State1)
	{
		case start1:
		if(GetBit(PINB,0) == 0)
		{
			if(z != 0x0001)
			{
				z = z >> 1;
			}
		}
		if(GetBit(PINB,1) == 0)
		{
			if(z != 0x8000)
			{
				z = z << 1;
			}
		}
		if((GetBit(PINB,0) == 0) && (GetBit(PINB,2) == 0))
		{
			if(z >= 0x0010)
			{
				z= z >> 4;
			}
			else
			{
				z = 0x0001;
			}
		}
		if((GetBit(PINB,1) == 0) && (GetBit(PINB,2) == 0))
		{
			if(z  <= 0x0100)
			{
				z = z << 4;
			}
			else
			{
				z = 0x8000;
			}
		}
		break;
	}
}

enum States2 {start2} State2;
void LEDS_Init2(){
	State2 = start2;
}
void LEDS_Tick2()
{
	switch(State2)
	{
		case start2:
		if(counter != 1)
		{
		   game = y | z;
		   setGround(game);	
		}
		if(y == z)
		{
			counter = 1;
			setGround(0x0000);
		}
		break;
	}
}

enum States {start, s1, s2} State;	
void LEDS_Init(){
	State = start;
}
void LEDS_Tick()
{
	switch(State)
	{
		case start:
		//setGround(y);
		State = s1;
		break;
		
		case s1:
		//setGround(y);
		y = y << 1;
		if(y == 0x8000)
		{
			State = s2;
		}
		else
		{
			State  = s1;
		}
		break;
		
		case s2:
		//setGround(y);
		y = y >> 1;
		if(y == 0x0001)
		{
			State = s1;
		}
		else
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
	vTaskDelay(400); 
   } 
}
void LedSecTask1()
{
	LEDS_Init1();
	for(;;)
	{
		LEDS_Tick1();
		vTaskDelay(400);
	}
}
void LedSecTask2()
{
	LEDS_Init2();
	for(;;)
	{
		LEDS_Tick2();
		vTaskDelay(400);
	}
}



void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(LedSecTask1, (signed portCHAR *)"LedSecTask1", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(LedSecTask2, (signed portCHAR *)"LedSecTask2", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
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



