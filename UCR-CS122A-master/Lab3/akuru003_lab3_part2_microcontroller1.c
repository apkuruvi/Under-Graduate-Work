/*	Partner(s) Name & E-mail: Abraham Kuruvila akuru003@ucr.edu No Partner
 *	Lab Section: 21
 *	Assignment: Lab 3 Part 2
 *	Exercise Description: Challenge -Blinking leds back and forth with changing leader and follower
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
#include "croutine.h" 
int counter = 0;
unsigned char temp = 0x00;
int time = 500;

// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}
enum LEDState {INIT,L0,L1, L2,L3,L5,L6} led_state;
	
void LEDS_Init(){
	led_state = INIT;
}

void LEDS_Tick(){
	//Actions
	switch(led_state){
		case INIT:
		led_state = L0;
		counter = 0;
		time = 1000;
		break;
		
		//leader
		case L0:
		time = 1000;
		PORTA = 0xFF;
		PORTC = 0xFF;
		if(USART_IsSendReady(1))
		{
			USART_Send(0xFF, 1);
		}
		if(GetBit(PINB,0) == 0)
		{
			if(counter == 0)
			{
				counter = 1;
			}
			else
			{
				counter = 0;
			}
			led_state = L5;
		}
		else
		{
			led_state = L1;
		}
		break;
		
		
		case L1:
		PORTA = 0x00;
		PORTC = 0xFF;
		if(USART_IsSendReady(1))
		{
			USART_Send(0x00, 1);
		}
		if(GetBit(PINB,0) == 0)
		{
			if(counter == 0)
			{
				counter = 1;
			}
			else
			{
				counter = 0;
			}
			led_state = L5;
		}
		else
		{
			led_state = L0;
		}
		break;
		
		
		// follower
		case L2:
		PORTC = 0x00;
		time = 50;
		if(USART_HasReceived(0))
		{
			temp = USART_Receive(0);
			PORTA = temp;
			USART_Flush(0);
		}
		if(GetBit(PINB,0) == 0)
		{
			if(counter == 0)
			{
				counter = 1;
			}
			else
			{
				counter = 0;
			}
			led_state = L6;
		}
	    else
	    {
		    led_state = L3;
	    }
		break;
		
		case L3:
		PORTC = 0x00;
		if(USART_HasReceived(0))
		{
			temp = USART_Receive(0);
			PORTA = temp;
			USART_Flush(0);
		}
		if(GetBit(PINB,0) == 0)
		{
			if(counter == 0)
			{
				counter = 1;
			}
			else
			{
				counter = 0;
			}
			led_state = L6;
		}
		else
		{
			led_state = L2;
		}
		break;
		
		case L5:
		PORTA = 0x00;
		PORTC = 0x00;
		if(USART_IsSendReady(1))
		{
			USART_Send(0x00, 1);
		}
		if(GetBit(PINB,0) == 1)
		{
			if(counter == 0)
			{
				led_state = L0;
				PORTC = 0xFF;
				time = 1000;
			}
			else
			{
				led_state = L2;
				PORTC = 0x00;
				time = 50;
			}
		}
		else
		{
			led_state = L5;
		}
		break;
		
		case L6:
		PORTA = 0x00;
		PORTC = 0x00;
		if(USART_IsSendReady(1))
		{
			USART_Send(0x00, 1);
		}
		if(GetBit(PINB,0) == 1)
		{
			if(counter == 0)
			{
				led_state = L0;
				PORTC = 0xFF;
				time = 1000;
			}
			else
			{
				led_state = L2;
				PORTC = 0x00;
				time = 50;
			}
		}
		else
		{
			led_state = L6;
		}
		break;
		
		
		default:
		break;
	}
	//Transitions
	switch(led_state){
		case INIT:
		break;
		case L0:
		break;
		case L1:
		break;
		case L2:
		break;
		case L3:
		break;
		case L5:
		break;
		case L6:
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
	vTaskDelay(time); 
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
   
   //Start Tasks  

   initUSART(0);
   initUSART(1);
   StartSecPulse(1);
   //RunSchedular 
   vTaskStartScheduler(); 
 
   return 0; 
}