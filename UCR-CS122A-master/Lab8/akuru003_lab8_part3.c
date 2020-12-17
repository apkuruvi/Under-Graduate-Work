/*	Partner(s) Name & E-mail: Abraham Kuruvila akuru003@ucr.edu No Partner
 *	Lab Section: 21
 *	Assignment: Lab 8 Part 3  
 *	Exercise Description: Moving motor based on keypad input
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
#include "keypad.h"
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
int check2 = 0;
int numsteps = 0;
int i = 0;
int j = 0;
// Bit-access function
/*
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}
*/

 unsigned char arr[20];
unsigned char ar[8] = {0x01,0x03,0x02,0x06,0x04,0x0C,0x08,0x09};
unsigned char ar1[8] = {0x09,0x08,0x0C,0x04,0x06,0x02,0x03,0x01};
enum LEDState2 {INIT2,s0} led_state2;

void LEDS_Init1(){
	led_state2 = INIT2;
}

void LEDS_Tick1(){
	switch(led_state2)
	{
		case INIT2:
		if(check2 > 0)
		{
			led_state2 = s0;
			i = 0;
			check = 0;
		}
		else
		{
			led_state2 = INIT2;
		}
		break;
		
		case s0:
		PORTA = ar1[i];
		i++;
		if(i >= 8)
		{
			i = 0;
		}
		check++;
		if(check <= check2)
		{
			led_state2 = s0;
		}
		else
		{
			check = 0;
			check2 = 0;
			i = 0;
			led_state2 = INIT2;
		}
		break;
		
		default:
		break;
	}
}

enum LEDState {start, s1, s2} led_state;
	
void LEDS_Init(){
	led_state = start;
}

void LEDS_Tick(){
	switch(led_state)
	{
		case start:
		PORTC = 0xEF;
		asm("nop");
		if (GetBit(PINC,0)==0) { arr[j] = '1'; j++;}
		if (GetBit(PINC,1)==0) {arr[j] = '4'; j++; }
		if (GetBit(PINC,2)==0) { arr[j] = '7'; j++;}
		if (GetBit(PINC,3)==0) { arr[j] = '*'; j++;}
		
		PORTC = 0xDF;
		asm("nop");
		if (GetBit(PINC,0)==0) { arr[j] = '2'; j++; }
		if (GetBit(PINC,1)==0) {arr[j] = '5'; j++;}
		if (GetBit(PINC,2)==0) { arr[j] = '8'; j++;}
		if (GetBit(PINC,3)==0) { arr[j] = '0'; j++; }
		
		PORTC = 0xBF;
		asm("nop");
		if (GetBit(PINC,0)==0) { arr[j] = '3'; PORTD = 0xFF; j++;}
		if (GetBit(PINC,1)==0) { arr[j] = '6'; PORTD = 0x00; j++;}
		if (GetBit(PINC,2)==0) { arr[j] = '9';PORTD = 0xFF; j++;}
		if (GetBit(PINC,3)==0) { arr[j] = '#'; PORTD = 0x00; j = 0; led_state = s1; }
		
		PORTC = 0x7F;
		asm("nop");
		if (GetBit(PINC,0)==0) { arr[0] = ' ';  arr[1] = '0'; arr[2] = ' '; j = 3; }
		if (GetBit(PINC,1)==0) {  arr[0] = ' '; arr[1] = '0'; arr[2] = ' '; j = 3;}
		if (GetBit(PINC,2)==0) {  arr[0] = ' ';  arr[1] = '0'; arr[2] = ' '; j = 3;}
		if (GetBit(PINC,3)==0) {  arr[0] = ' ';  arr[1] = '0'; arr[2] = ' '; j = 3;}
		break;
		
		
		case s1:
		numsteps = atoi(arr);
		for(int o = 0; o < 20; o++)
		{
			arr[o] = ' ';
		}
		if(numsteps > 0)
		{
			check2 = (numsteps/ 5.625) * 64;
		}
		led_state = s2;
		break;
		
		case s2:
		if(check2 > 0)
		{
			led_state = s2;
		}
		else
		{
			led_state = start;
		    j = 0;
			for(int o = 0; o < 20; o++)
			{
				arr[o] = ' ';
			}
			numsteps = 0;
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
	vTaskDelay(300); 
   } 
}

void LedSecTask1()
{
	LEDS_Init1();
	for(;;)
	{
		LEDS_Tick1();
		vTaskDelay(2);
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
   DDRC = 0xF0; PORTC = 0x0F;
   DDRD = 0xFF; PORTD = 0x00;
   
   for(int o = 0; o < 20; o++)
   {
	   arr[o] = ' ';
   }
   j = 0;
   //Start Tasks  
   StartSecPulse(1);
    //RunSchedular 
   vTaskStartScheduler(); 
 
   return 0; 
}