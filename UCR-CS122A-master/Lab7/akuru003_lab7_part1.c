/*	Partner(s) Name & E-mail: Abraham Kuruvila akuru003@ucr.edu No Partner
 *	Lab Section: 21
 *	Assignment: Lab 7 Part 1
 *	Exercise Description: ADC value on led bar from joystick
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
unsigned short x = 0;



void InitADC()
{
	ADMUX=(1<<REFS0);   //set seventh bit of ADMUX to 1 so reference voltage is 5v
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //rescalar div factor =128
}
void convert(){
	ADCSRA |=(1<<ADSC);//start ADC conversion
	while ( !(ADCSRA & (1<<ADIF)));//wait till ADC conversion
	
}


enum states {start, wait, x_axis, y_axis} state;
void LEDS_Init1(){
	state = start;
}
unsigned short temp1,movx,movy, temp2;
unsigned char left,right,up,down = 0x00;
void tick()
{
	switch(state)
	{
		case start:
		state = wait;
		break;
		
		case wait:
		temp2 = 0x00;
		temp1 = ADMUX;
		if (ADMUX ==0x40)
		{
			state = x_axis;
		}
		else if (ADMUX ==0x41)
		{
			state = y_axis;
		}
		
		else{state = wait;}
		break;
		
		case x_axis:
		state = wait;
		break;
		
		case y_axis:
		state = wait;
		break;
		
		
	}
	switch(state)
	{
		case start:
		break;
		
		case wait:
		break;
		
		case x_axis:
		convert();
		movx =ADC;
		if (movx > 612)
		{
			temp2= (0x08);
			right++;
			
		}
		else if (movx <412)
		{
			temp2= (0x04);
			left++;
		}
		ADMUX = 0x41;// initialize value of ADMUX to switch over to next pin
		break;
		
		case y_axis:
		convert();
		movy =ADC;
		if (movy > 612)
		{
			temp2= (0x02);
			up++;
		}
		else if (movy <412)
		{
			temp2= (0x01);
			down++;
			
		}
		ADMUX = 0x40;
		break;
	}
	//PORTB = temp2;
}




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

enum States {start1} State;	
void LEDS_Init(){
	State = start1;
}
void LEDS_Tick()
{
	switch(State)
	{
		case start1:
		x = ADC;
		temp = (char)x;
		PORTB = temp;
		x = x >> 8;
		temp = (char)x;
		temp = temp << 6;
		PORTA = temp;
		break;
	}
}
void LedSecTask()
{
	LEDS_Init();
   for(;;) 
   { 	
	LEDS_Tick();
	vTaskDelay(1); 
   } 
}

void LedSecTask1()
{
	LEDS_Init1();
	for(;;)
	{
		tick();
		vTaskDelay(1);
	}
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(LedSecTask1, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}	

int main(void) 
{ 
	// down 2 up 1 left 8, right 4
   DDRA = 0xC0; PORTA = 0x3F;
   DDRB = 0xFF; PORTB = 0x00;
   DDRC = 0xFF; PORTC = 0x00;
   DDRD = 0xFF; PORTD = 0x00;
   
   InitADC();
   StartSecPulse(1);
   //RunSchedular 
   vTaskStartScheduler();
   return 0; 
}



