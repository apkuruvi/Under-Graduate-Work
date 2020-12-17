/*	Partner(s) Name & E-mail: Abraham Kuruvila akuru003@ucr.edu No Partner
 *	Lab Section: 21
 *	Assignment: Lab 8 Part 1
 *	Exercise Description: Rotating stepper motor with buttons  controlling direction
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
		//PORTD = 0xFF;
		if((GetBit(PINB,0) == 0) && (GetBit(PINB,1) == 0))
		{
			counter = 0;
			counter1 = 0;
		}
		else if((GetBit(PINB,0)) && (GetBit(PINB,1)))
		{
			counter = 0;
			counter1 = 0;
		}
		else if(GetBit(PINB,0) == 0)
		{
			counter = 1;
			//PORTD = 0xFF;
			counter1 = 0;
		}
		else if(GetBit(PINB,1) == 0)
		{
			counter = 0;
			counter1 = 1;
		}
		else
		{
			counter = 0;
			counter1 = 0;
		}
		break;
		
		default:
		break;
	}
}




enum LEDState {INIT,L0, L1, L2,L3,L4,L5,L6, start,INIT1,L01, L11, L21,L31,L41,L51,L61} led_state;
	
void LEDS_Init(){
	led_state = start;
}

void LEDS_Tick(){
	switch(led_state)
	{
		case start:
		if(counter == 1 && counter1 == 1)
		{
			led_state = start;
		}
		else if(counter == 0 && counter1 == 0)
		{
			led_state = start;
		}
		else if(counter == 1)
		{
			led_state = INIT;
		}
		else if(counter1 == 1)
		{
			led_state = INIT1;
		}
		else
		{
			led_state = start;
		}
		break;
		
		case INIT:
		if(counter == 1 && counter1 == 1)
		{
			led_state = start;
		}
		else if(counter == 0 && counter1 == 0)
		{
			led_state = start;
		}
		else if(counter)
		{
			led_state = L0;
		}
		else
		{
			led_state = start;
		}
		break;
		
		case L0:
		if(counter == 1 && counter1 == 1)
		{
			led_state = start;
		}
		else if(counter == 0 && counter1 == 0)
		{
			led_state = start;
		}
		else if(counter)
		{
			led_state = L1;
		}
		else
		{
			led_state = start;
		}
		break;
		
		case L1:
		if(counter == 1 && counter1 == 1)
		{
			led_state = start;
		}
		else if(counter == 0 && counter1 == 0)
		{
			led_state = start;
		}
		else if(counter)
		{
			led_state = L2;
		}
		else
		{
			led_state = start;
		}
		break;
		
		case L2:
		if(counter == 1 && counter1 == 1)
		{
			led_state = start;
		}
		else if(counter == 0 && counter1 == 0)
		{
			led_state = start;
		}
		else if(counter)
		{
			led_state = L3;;
		}
		else
		{
			led_state = start;
		}
		break;
		
		case L3:
		if(counter == 1 && counter1 == 1)
		{
			led_state = start;
		}
		else if(counter == 0 && counter1 == 0)
		{
			led_state = start;
		}
		else if(counter)
		{
			led_state = L4;
		}
		else
		{
			led_state = start;
		}
		break;
		
		case L4:
		if(counter == 1 && counter1 == 1)
		{
			led_state = start;
		}
		else if(counter == 0 && counter1 == 0)
		{
			led_state = start;
		}
		else if(counter)
		{
			led_state = L5;
		}
		else
		{
			led_state = start;
		}
		break;
		
		case L5:
		if(counter == 1 && counter1 == 1)
		{
			led_state = start;
		}
		else if(counter == 0 && counter1 == 0)
		{
			led_state = start;
		}
		else if(counter)
		{
			led_state = L6;
		}
		else
		{
			led_state = start;
		}
		break;
		
		case L6:
		if(counter == 1 && counter1 == 1)
		{
			led_state = start;
		}
		else if(counter == 0 && counter1 == 0)
		{
			led_state = start;
		}
		else if(counter)
		{
			led_state = INIT;
		}
		else
		{
			led_state = start;
		}
		break;	
		
		case INIT1:
		if(counter == 1 && counter1 == 1)
		{
			led_state = start;
		}
		else if(counter == 0 && counter1 == 0)
		{
			led_state = start;
		}
		else if(counter1)
		{
			led_state = L01;
		}
		else
		{
			led_state = start;
		}
		break;
		
		case L01:
		if(counter == 1 && counter1 == 1)
		{
			led_state = start;
		}
		else if(counter == 0 && counter1 == 0)
		{
			led_state = start;
		}
		else if(counter1)
		{
			led_state = L11;
		}
		else
		{
			led_state = start;
		}
		break;
		
		case L11:
		if(counter == 1 && counter1 == 1)
		{
			led_state = start;
		}
		else if(counter == 0 && counter1 == 0)
		{
			led_state = start;
		}
		else if(counter1)
		{
			led_state = L21;
		}
		else
		{
			led_state = start;
		}
		break;
		
		case L21:
		if(counter == 1 && counter1 == 1)
		{
			led_state = start;
		}
		else if(counter == 0 && counter1 == 0)
		{
			led_state = start;
		}
		else if(counter1)
		{
			led_state = L31;
		}
		else
		{
			led_state = start;
		}
		break;
		
		case L31:
		if(counter == 1 && counter1 == 1)
		{
			led_state = start;
		}
		else if(counter == 0 && counter1 == 0)
		{
			led_state = start;
		}
		else if(counter1)
		{
			led_state = L41;
		}
		else
		{
			led_state = start;
		}
		break;
		
		case L41:
		if(counter == 1 && counter1 == 1)
		{
			led_state = start;
		}
		else if(counter == 0 && counter1 == 0)
		{
			led_state = start;
		}
		else if(counter1)
		{
			led_state = L51;
		}
		else
		{
			led_state = start;
		}
		break;
		
		case L51:
		if(counter == 1 && counter1 == 1)
		{
			led_state = start;
		}
		else if(counter == 0 && counter1 == 0)
		{
			led_state = start;
		}
		else if(counter1)
		{
			led_state = L61;
		}
		else
		{
			led_state = start;
		}
		break;
		
		case L61:
		if(counter == 1 && counter1 == 1)
		{
			led_state = start;
		}
		else if(counter == 0 && counter1 == 0)
		{
			led_state = start;
		}
		else if(counter1)
		{
			led_state = INIT1;
		}
		else
		{
			led_state = start;
		}
		break;
		
		default:
		break;
	}
	//Transitions
	switch(led_state){
		case start:
		break;
		case INIT:
		PORTA = 0x01;
		break;
		case L0:
		PORTA = 0x03;
		break;
		case L1:
		PORTA = 0x02;
		break;
		case L2:
		PORTA = 0x06;
		break;
		case L3:
		PORTA = 0x04;
		break;
		case L4:
		PORTA = 0x0C;
		break;
		case L5:
		PORTA = 0x08;
		break;
		case L6:
		PORTA = 0x09;
		break;
		
		
		
		case INIT1:
		PORTA = 0x09;
		break;
		case L01:
		PORTA = 0x08;
		break;
		case L11:
		PORTA = 0x0C;
		break;
		case L21:
		PORTA = 0x04;
		break;
		case L31:
		PORTA = 0x06;
		break;
		case L41:
		PORTA = 0x02;
		break;
		case L51:
		PORTA = 0x03;
		break;
		case L61:
		PORTA = 0x01;
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
	vTaskDelay(5); 
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