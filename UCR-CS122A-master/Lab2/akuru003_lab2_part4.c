/*	Partner(s) Name & E-mail: Abraham Kuruvila akuru003@ucr.edu No Partner
 *	Lab Section: 21
 *	Assignment: Lab 2 Part 4
 *	Exercise Description: Challenge -Blinking leds back and forth with button reversal
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


// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}
enum LEDState {INIT,L0, L1, L2,L3,L4,L5,L6,L7,} led_state;
	
void LEDS_Init(){
	led_state = INIT;
}

void LEDS_Tick(){
	//Actions
	switch(led_state){
		case INIT:
		led_state = L0;
		break;
		
		case L0:
		led_state = L1;
		break;
		
		case L1:
		if(GetBit(~PINA, 0) == 1)
		{
			if(counter == 0)
			{
				counter = 1;
			} 
			else
			{
				counter = 0;
			}
		}
		
		if(counter == 0)
		{
			counter++;
			led_state = L2;
		}
		else
		{
			counter--;
			led_state = L0;
		}
		break;
		
		case L2:
		if(GetBit(~PINA, 0) == 1)
		{
			if(counter1 == 0)
			{
				counter1 = 1;
			}
			else
			{
				counter1 = 0;
			}
		}
		if(counter1 == 0)
		{
			counter1++;
			led_state = L3;
		}
		else
		{
			counter1--;
			led_state = L1;
		}
		break;
		
		case L3:
		if(GetBit(~PINA, 0) == 1)
		{
			if(counter2 == 0)
			{
				counter2 = 1;
			}
			else
			{
				counter2 = 0;
			}
		}
		if(counter2 == 0)
		{
			counter2++;
			led_state = L4;
		}
		else
		{
			counter2--;
			led_state = L2;
		}
		break;
		
		case L4:
		if(GetBit(~PINA, 0) == 1)
		{
			if(counter3 == 0)
			{
				counter3 = 1;
			}
			else
			{
				counter3 = 0;
			}
		}
		if(counter3 == 0)
		{
			counter3++;
			led_state = L5;
		}
		else
		{
			counter3--;
			led_state = L3;
		}
		break;
		
		case L5:
		if(GetBit(~PINA, 0) == 1)
		{
			if(counter4 == 0)
			{
				counter4 = 1;
			}
			else
			{
				counter4 = 0;
			}
		}
		if(counter4 == 0)
		{
			counter4++;
			led_state = L6;
		}
		else
		{
			counter4--;
			led_state = L4;
		}
		break;
		
		case L6:
		if(GetBit(~PINA, 0) == 1)
		{
			if(counter5 == 0)
			{
				counter5 = 1;
			}
			else
			{
				counter5 = 0;
			}
		}
		if(counter5 == 0)
		{
			counter5++;
			led_state = L7;
		}
		else
		{
			counter5--;
			led_state = L5;
		}
		break;
		
		case L7:
		led_state = L6;
		break;
		
		
		default:
		break;
	}
	//Transitions
	switch(led_state){
		case INIT:
		break;
		case L0:
		PORTD = 0x01;
		break;
		case L1:
		PORTD = 0x02;
		break;
		case L2:
		PORTD = 0x04;
		break;
		case L3:
		PORTD = 0x08;
		break;
		case L4:
		PORTD = 0x10;
		break;
		case L5:
		PORTD = 0x20;
		break;
		case L6:
		PORTD = 0x40;
		break;
		case L7:
		PORTD = 0x80;
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
	vTaskDelay(500); 
   } 
}


void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	
}	
 
int main(void) 
{ 
   DDRA = 0x00; PORTA=0xFF;
   DDRD = 0xFF;
   //Start Tasks  
   StartSecPulse(1);
    //RunSchedular 
   vTaskStartScheduler(); 
 
   return 0; 
}