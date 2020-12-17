/*	Partner(s) Name & E-mail: Abraham Kuruvila akuru003@ucr.edu No Partner
 *	Lab Section: 21
 *	Assignment: Project
 *	Exercise Description: Coin Filtering Vending machine. 
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
unsigned char a = 0x00;
unsigned short temp = 0x0000;
int selection = 0;
int counter = 0;
int flag = 0;
int motor = 0; 
int i = 0;
int check = 0;
unsigned char ar[8] = {0x01,0x03,0x02,0x06,0x04,0x0C,0x08,0x09};
unsigned char ar1[8] = {0x09,0x08,0x0C,0x04,0x06,0x02,0x03,0x01};
// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

enum LEDState2 {start2, s12, s22,s32} led_state2;
void LEDS_Init2(){
	led_state2 = start2;
}
enum LEDState1 {start1, s11} led_state1;
void LEDS_Init1(){
	led_state1 = start1;
}
enum LEDState {start, s1,s2,s3} led_state;
void LEDS_Init(){
	led_state = start;
}

void LEDS_Tick2()
{
	switch(led_state2)
	{
		case start2:
		if(motor == 1)
		{
			PORTD = 0xFF;
			led_state2 = s12;
			i = 0;
			check = 0;
		}
		else if(motor == 2)
		{
			led_state2 = s22;
			i = 0;
			check = 0;
		}
		else if(motor == 3)
		{
			led_state2 = s32;
			i = 0;
			check = 0;
		}
		else
		{
			led_state2 = start2;
		}
		break;

        case s12:
        PORTA = ar[i];  // change appropriate port
        i++;
        if(i >= 8)
        {
	        i = 0;
        }
        check++;
        if(check < 6000)   // how much rotation we need
        {
	        led_state2 = s12;
			flag = 0;
        }
        else
        {
	        check = 0;
	        i = 0;
	        led_state2 = start2;
			flag = 1;
			motor = 0;
        }
        break;
		
		case s22:
		a = ar1[i];
		a = a << 4;
		PORTC = a;  // change appropriate port
		i++;
		if(i >= 8)
		{
			i = 0;
		}
		check++;
		if(check < 6000)   // how much rotation we need
		{
			led_state2 = s22;
			flag = 0;
		}
		else
		{
			check = 0;
			i = 0;
			led_state2 = start2;
			flag = 1;
			motor = 0;
		}
		break;
		
		case s32:
		PORTC = ar1[i];  // change appropriate port
		i++;
		if(i >= 8)
		{
			i = 0;
		}
		check++;
		if(check < 6000)   // how much rotation we need
		{
			led_state2 = s32;
			flag = 0;
		}
		else
		{
			check = 0;
			i = 0;
			led_state2 = start2;
			flag = 1;
			motor = 0;
		}
		break;
		
		default:
		break;
	}
}
void LEDS_Tick1()
{
	switch(led_state1)
	{
		case start1:
		if(GetBit(PINB,0) == 0)
		{
			counter++;
			led_state1 = s11;
		}
		else
		{
			led_state1 = start1;
		}
		break;
		
        case s11:
        if(GetBit(PINB,0) == 1)
        {
	        led_state1 = start1;
        }
		else
		{
			led_state1 = s11;
		}
        break;

		default:
		break;
	}
}
void LEDS_Tick()
{
	switch(led_state)
	{
		case start:
		if(USART_HasReceived(0))
		{
			temp = USART_Receive(0);
			if((temp == '1') || (temp == '2') || (temp == '3'))
			{
				led_state = s1;
			}
			else
			{
				led_state = start;
			}
			USART_Flush(0);
		}
		break;
	
		case s1:
		if(temp == '1')
		{
			//PORTD = 0xFF;
			selection = 1;
		}
		else if(temp == '2')
		{
			selection = 2;
		}
		else if(temp == '3')
		{
			selection = 3;
		}
		led_state = s2;
		break;
		
		case s2:
		if(counter == selection)
		{
			led_state = s3;
			motor = selection;
			flag = 0;
		}
		else
		{
			led_state = s2;
		}
		break;
		
		case s3:
		if(flag == 1)
		{
			led_state = start;
			selection = 0;
			counter = 0;
			flag = 0;
		}
		else
		{
			led_state = s3;
		}
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
	vTaskDelay(1); 
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
void LedSecTask2()
{
	LEDS_Init2();
	for(;;)
	{
		LEDS_Tick2();
		vTaskDelay(3);
	}
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(LedSecTask1, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(LedSecTask2, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}	
 
int main(void) 
{ 
   DDRA = 0xFF; PORTA = 0x00;
   DDRB = 0x00; PORTB = 0xFF;
   DDRC = 0xFF; PORTC = 0x00;
   DDRD = 0xFF; PORTD = 0x00;
  
   //Start Tasks  
   initUSART(0);
   StartSecPulse(1);
   //RunSchedular 
   vTaskStartScheduler(); 
 
   return 0; 
}