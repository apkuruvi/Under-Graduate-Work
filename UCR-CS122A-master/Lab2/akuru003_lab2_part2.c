/*	Partner(s) Name & E-mail: Abraham Kuruvila akuru003@ucr.edu No Partner
 *	Lab Section: 21
 *	Assignment: Lab 2 Part 2
 *	Exercise Description: Blinking leds different periods
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
int counter = 0;
int counter1 = 0;
unsigned char temp = 0x00;
//FreeRTOS include files 
#include "FreeRTOS.h" 
#include "task.h" 
#include "croutine.h" 
enum LEDState {INIT,L0} led_state;
enum LEDState1 {INIT1,L01} led_state1;
enum LEDState2 {INIT2,L02} led_state2;
	
void LEDS_Init(){
	led_state = INIT;
}
void LEDS_Init1(){
	led_state1 = INIT1;
}
void LEDS_Init2(){
	led_state2 = INIT2;
}

void LEDS_Tick(){
	//Actions
	switch(led_state){
		case INIT:
		temp = temp | 0x01;
		PORTD = temp;
		break;
		case L0:
		temp = temp & 0xFE;
		PORTD = temp;
		break;
		default:
		PORTD = 0;
		break;
	}
	//Transitions
	switch(led_state){
		case INIT:
			led_state = L0;
		break;
		case L0:
			led_state = INIT;
		break;
		default:
			led_state = INIT;
		break;
	}
}

void LEDS_Tick1(){
	//Actions
	switch(led_state1){
		case INIT1:
		counter++;
		if(counter == 2)
		{
			counter = 0;
			led_state1 = L01;
		}
		else
		{
			led_state1 = INIT1;
		}
		break;
		case L01:
		counter++;
		if(counter == 2)
		{
			counter = 0;
			led_state1 = INIT1;
		}
		else
		{
			led_state1 = L01;
		}
		break;
		default:
		PORTD = 0;
		break;
	}
	//Transitions
	switch(led_state1){
		case INIT1:
		temp = temp & 0xFB;
		PORTD = temp;
		break;
		case L01:
		temp = temp | 0x04;
		PORTD = temp;
		break;
		default:
		led_state1 = INIT1;
		break;
	}
}

void LEDS_Tick2(){
	//Actions
	switch(led_state2){
		case INIT2:
		counter1++;
		if(counter1 == 5)
		{
			counter1 = 0;
			led_state2 = L02;
		}
		else
		{
			led_state2 = INIT2;
		}
		break;
		case L02:
		counter1++;
		if(counter1 == 5)
		{
			counter1 = 0;
			led_state2 = INIT2;
		}
		else
		{
			led_state2 = L02;
		}
		break;
		default:
		PORTD = 0;
		break;
	}
	//Transitions
	switch(led_state2){
		case INIT2:
		temp = temp & 0xEF;
		PORTD = temp;
		break;
		case L02:
		temp = temp | 0x10;
		PORTD = temp;
		break;
		default:
		led_state2 = INIT2;
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

void LedSecTask1()
{
	LEDS_Init1();
	for(;;)
	{
		LEDS_Tick1();
		vTaskDelay(500);
	}
}

void LedSecTask2()
{
	LEDS_Init2();
	for(;;)
	{
		LEDS_Tick2();
		vTaskDelay(500);
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
   DDRA = 0x00; PORTA=0xFF;
   DDRD = 0xFF;
   //Start Tasks  
   StartSecPulse(1);
    //RunSchedular 
   vTaskStartScheduler(); 
 
   return 0; 
}