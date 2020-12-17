/*	Partner(s) Name & E-mail: Abraham Kuruvila akuru003@ucr.edu No Partner
 *	Lab Section: 21
 *	Assignment: Lab 5 Part 1
 *	Exercise Description: Sending pattern to shift register to put on led bar
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
unsigned char temp = 0;

void setGround()
{	
	for(int i = 7; i >= 0; i--)
	{
		PORTA = 0x88;
		PORTA |= ((temp >> i) & 0x01);   // for first 4 
		PORTA |= (((temp >> i) << 4) & 0x10);  // for next 4
		PORTA |= 0x44;
	}
	PORTA |= 0x22;
	PORTA = 0x00;
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
		if(GetBit(PINB, 0) == 0)
		{
			if(temp != 0xFF)
			temp++;
		}
		if(GetBit(PINB, 1) == 0)
		{
			if(temp != 0x00)
			temp--;
		}
		if((GetBit(PINB, 0) == 0) && (GetBit(PINB, 1) == 0))
		{
			temp = 0x00;
		}
		setGround();
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
   DDRB = 0x00; PORTB = 0xFF;
   DDRC = 0xFF; PORTC = 0x00;
   DDRD = 0xFF; PORTD = 0x00;
   
   StartSecPulse(1);
   //RunSchedular 
   vTaskStartScheduler();
   return 0; 
}



