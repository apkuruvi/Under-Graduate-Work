/*	Partner(s) Name & E-mail: Abraham Kuruvila akuru003@ucr.edu No Partner
 *	Lab Section: 21
 *	Assignment: Lab 6 Part 4
 *	Exercise Description: Outputting rectangle on led matrix and buttons to move it
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
unsigned char ground = 0xFF;
unsigned char lights = 0x00;
int groundStart = 0;
int rowStart = 0;
int ar[5][8];
int row1 = 1;
int row2 = 2;
int row3 = 3;
int column2 = 2;
int column3 = 3;
int column4 = 4;
int column5 = 5;

void setGround()
{
	if(groundStart == 0x7F || rowStart == 4)
	{
		groundStart = 0xFE;
		rowStart = 0;
	}
	else
	{
		groundStart = (groundStart << 1) | 0x01;
		rowStart++;
	}
	lights =  0x00;
	
	for( int b = 0; b < 8; b++)
	{
		if(ar[rowStart][b] == 1) lights |= 0x80;
		if( b < 7 )
		{
			lights = lights >> 1;
		}
	}
	for(int i = 7; i >= 0; i--)
	{
		PORTC = 0x88;
		PORTD = 0x88;
		PORTC |= ((lights >> i) & 0x01);
		PORTD |= ((groundStart >> i) & 0x01);
		PORTC |= 0x44;
		PORTD |= 0x44;
	}
	PORTC |= 0x22;
	PORTD |= 0x22;
	PORTC = 0x00;
	PORTD = 0x00;
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
		setGround();
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
		for(int i = 0; i < 5; i++)
		{
			for(int j = 0; j < 8; j++)
			{
				ar[i][j] = 0;
			}
		}
		State = s1;
		ar[row1][column2] = 1;
		ar[row1][column3] = 1;
		ar[row1][column4] = 1;
		ar[row1][column5] = 1;
		ar[row2][column2] = 1;
		ar[row2][column5] = 1;
		ar[row3][column2] = 1;
		ar[row3][column3] = 1;
		ar[row3][column4] = 1;
		ar[row3][column5] = 1;
		break;
		
		
		case s1:
		if(GetBit(PINB, 0) == 0)
		{
			if(row1 != 0)
			{
				row1--;
				row2--;
				row3--;	
			}
			for(int i = 0; i < 5; i++)
			{
				for(int j = 0; j < 8; j++)
				{
					ar[i][j] = 0;
				}
			}
			State = s2;
			ar[row1][column2] = 1;
			ar[row1][column3] = 1;
			ar[row1][column4] = 1;
			ar[row1][column5] = 1;
			ar[row2][column2] = 1;
			ar[row2][column5] = 1;
			ar[row3][column2] = 1;
			ar[row3][column3] = 1;
			ar[row3][column4] = 1;
			ar[row3][column5] = 1;
		}
		else if(GetBit(PINB, 1) == 0)
		{
			if(row3 != 4)
			{
				row1++;
				row2++;
				row3++;
			}
			for(int i = 0; i < 5; i++)
			{
				for(int j = 0; j < 8; j++)
				{
					ar[i][j] = 0;
				}
			}
			State = s2;
			ar[row1][column2] = 1;
			ar[row1][column3] = 1;
			ar[row1][column4] = 1;
			ar[row1][column5] = 1;
			ar[row2][column2] = 1;
			ar[row2][column5] = 1;
			ar[row3][column2] = 1;
			ar[row3][column3] = 1;
			ar[row3][column4] = 1;
			ar[row3][column5] = 1;
		}
		else if(GetBit(PINB,2) == 0)
		{
			if(column5 != 7)
			{
				column2++;
				column3++;
				column4++;
				column5++;
			}
			for(int i = 0; i < 5; i++)
			{
				for(int j = 0; j < 8; j++)
				{
					ar[i][j] = 0;
				}
			}
			State = s2;
			ar[row1][column2] = 1;
			ar[row1][column3] = 1;
			ar[row1][column4] = 1;
			ar[row1][column5] = 1;
			ar[row2][column2] = 1;
			ar[row2][column5] = 1;
			ar[row3][column2] = 1;
			ar[row3][column3] = 1;
			ar[row3][column4] = 1;
			ar[row3][column5] = 1;
		}
		else if(GetBit(PINB,3) == 0)
		{
			if(column2 != 0)
			{
				column2--;
				column3--;
				column4--;
				column5--;
			}
			for(int i = 0; i < 5; i++)
			{
				for(int j = 0; j < 8; j++)
				{
					ar[i][j] = 0;
				}
			}
			State = s2;
			ar[row1][column2] = 1;
			ar[row1][column3] = 1;
			ar[row1][column4] = 1;
			ar[row1][column5] = 1;
			ar[row2][column2] = 1;
			ar[row2][column5] = 1;
			ar[row3][column2] = 1;
			ar[row3][column3] = 1;
			ar[row3][column4] = 1;
			ar[row3][column5] = 1;
		}
		else
		{
			State = s1;
		}
		break;
		
		case s2:
		if((GetBit(PINB, 0)) && (GetBit(PINB, 1))  && (GetBit(PINB, 2))  && (GetBit(PINB, 3)))
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
	vTaskDelay(200); 
   } 
}

void LedSecTask1()
{
	LEDS_Init1();
	for(;;)
	{
		LEDS_Tick1();
		vTaskDelay(1);
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



