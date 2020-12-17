/*	Partner(s) Name & E-mail: Abraham Kuruvila akuru003@ucr.edu No Partner
 *	Lab Section: 21
 *	Assignment: Lab 8 Part 2
 *	Exercise Description: 6 buttons controlling which way and how much the motor turns 
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
#include <avr/pgmspace.h> 
unsigned char temp = 0x00;
//FreeRTOS include files 
#include "FreeRTOS.h" 
#include "task.h" 
#include "croutine.h" 
int counter = 0;
int counter1 = 0;
int counter2 = 0;
int counter3 = 0;
int counter4 = 0;
int counter5 = 0;
int check = 0;
int i = 0;

// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

enum LEDState2 {INIT2} led_state2;

void LEDS_Init1(){
	led_state2 = INIT2;
}

void LEDS_Tick1(){
	switch(led_state2)
	{
		case INIT2:
		if(GetBit(PINB,0) == 0)
		{
			counter = 1;
			counter1 = 0;
			counter2 = 0;
			counter3 = 0;
			counter4 = 0;
			counter5 = 0;
		}
		else if(GetBit(PINB,1) == 0)
		{
			counter = 0;
			counter1 = 1;
			counter2 = 0;
			counter3 = 0;
			counter4 = 0;
			counter5 = 0;
		}
		else if(GetBit(PINB,2) == 0)
		{
			counter = 0;
			counter1 = 0;
			counter2 = 1;
			counter3 = 0;
			counter4 = 0;
			counter5 = 0;
		}
		else if(GetBit(PINB,3) == 0)
		{
			counter = 0;
			counter1 = 0;
			counter2 = 0;
			counter3 = 1;
			counter4 = 0;
			counter5 = 0;
		}
		else if(GetBit(PINB,4) == 0)
		{
			counter = 0;
			counter1 = 0;
			counter2 = 0;
			counter3 = 0;
			counter4 = 1;
			counter5 = 0;
		}
		else if(GetBit(PINB,5) == 0)
		{
			counter = 0;
			counter1 = 0;
			counter2 = 0;
			counter3 = 0;
			counter4 = 0;
			counter5 = 1;
		}
		else
		{
			counter = 0;
			counter1 = 0;
			counter2 = 0;
			counter3 = 0;
			counter4 = 0;
			counter5 = 0;
		}
		break;
		
		default:
		break;
	}
}

enum LEDState {start, s1, s2, s3,s4, s5, s7} led_state;
	
void LEDS_Init(){
	led_state = start;
}

unsigned char ar[8] = {0x01,0x03,0x02,0x06,0x04,0x0C,0x08,0x09};
unsigned char ar1[8] = {0x09,0x08,0x0C,0x04,0x06,0x02,0x03,0x01};	
void LEDS_Tick(){
	switch(led_state)
	{
		case start:
		if(counter == 1)
		{
			led_state = s1;
			i = 0;
			check = 0;
		}
		else if(counter1 == 1)
		{
			led_state = s2;
			i = 0;
			check = 0;
		}
		else if(counter2 == 1)
		{
			led_state = s3;
			i = 0;
			check = 0;
		}
		else if(counter3 == 1)
		{
			led_state = s4;
			i = 0;
			check = 0;
		}
		else if(counter4 == 1)
		{
			led_state = s5;
			i = 0;
			check = 0;
		}
		else if(counter5 == 1)
		{
			led_state = s7;
			i = 0;
			check = 0;
		}
		else
		{
			led_state = start;
		}
		break;
		
		case s1:
		PORTA = ar[i];
		i++;
		if(i >= 8)
		{
			i = 0;
		}
		check++;
		if(check < 1000)
		{
			led_state = s1;
		}
		else
		{
			check = 0;
			i = 0;
			led_state = start;
		}
		break;
		
		case s2:
		PORTA = ar[i];
		i++;
		if(i >= 8)
		{
			i = 0;
		}
		check++;
		if(check < 2084)
		{
			led_state = s2;
		}
		else
		{
			check = 0;
			i = 0;
			led_state = start;
		}
		break;
		
		case s3:
		PORTA = ar[i];
		i++;
		if(i >= 8)
		{
			i = 0;
		}
		check++;
		if(check < 4168)
		{
			led_state = s3;
		}
		else
		{
			check = 0;
			i = 0;
			led_state = start;
		}
		break;
		
		case s4:
		PORTA = ar1[i];
		i++;
		if(i >= 8)
		{
			i = 0;
		}
		check++;
		if(check < 1000)
		{
			led_state = s4;
		}
		else
		{
			check = 0;
			i = 0;
			led_state = start;
		}
		break;
		
		case s5:
		PORTA = ar1[i];
		i++;
		if(i >= 8)
		{
			i = 0;
		}
		check++;
		if(check < 2084)
		{
			led_state = s5;
		}
		else
		{
			check = 0;
			i = 0;
			led_state = start;
		}
		break;
		
		case s7:
		PORTA = ar1[i];
		i++;
		if(i >= 8)
		{
			i = 0;
		}
		check++;
		if(check < 4168)
		{
			led_state = s7;
		}
		else
		{
			check = 0;
			i = 0;
			led_state = start;
		}
		break;
		
		default:
		break;
	}
}



void LedSecTask()
{
	LEDS_Init();
   for(;;) 
   { 	
	LEDS_Tick();
	vTaskDelay(2); 
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
   //Start Tasks  
   StartSecPulse(1);
    //RunSchedular 
   vTaskStartScheduler(); 
 
   return 0; 
}