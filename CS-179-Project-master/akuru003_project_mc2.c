/*	Partner(s) Name & E-mail: Abraham Kuruvila akuru003@ucr.edu 
 *	Lab Section: 21
 *	Assignment: CS 179J Senior Design Project
 *	
 *	
 *                     TEST CASES AT THE BOTTOM
 *                     CODE for MICROCONTROLLER 2
 *
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <stdint.h> 
#include <stdlib.h> 
#include <avr/eeprom.h>
#include <avr/io.h>
#include "io.c"
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
float temp;
int i = 0;
unsigned char test[];
char ar[32] = "Temp:           Dog Status:     ";
uint16_t dogstatus;         					  //eeprom
short sensorreading;
															
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b)   // Bit-access function
{
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}
void adc_init()
{
	ADMUX = (1<<REFS0);
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}
													        	// read adc value
uint16_t adc_read(uint8_t ch)
{
	ch &= 0b00000111;
	ADMUX = (ADMUX & 0xF8)|ch;
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	return (ADC);
}

void ftc(char* stringval,float Value)         // float to string converter
{
	unsigned long answer = 0;
	unsigned int int_part = 0;
	unsigned int whole = 7;
	int i = 0;
	int sizeoffrac = 0;
	unsigned int exponent = 8;
	char frac_part[3];
	unsigned int sign = 1;
	answer += ((unsigned long) whole << 16);
	answer += 0x00800000;
	exponent = (signed char) exponent - 127;
	if(sign)
	{
		stringval[0] = '-';
		sizeoffrac++;
	}
	int_part = answer >> (23 - exponent);       // does math to isolate fraction and whole part
	itoa(int_part, &stringval[sizeoffrac],10);
	stringval[sizeoffrac++] = '.';
	switch (0x7 & (answer  >> (20 - exponent)))         // fracation part 
	{
		case 1:
		frac_part[0] = '2';
		frac_part[1] = '5';
		frac_part[2] = '0';
		break;
		case 2:
		frac_part[0] = '3';
		frac_part[1] = '7';
		frac_part[2] = '5';
		break;
		case 3:
		frac_part[0] = '5';
		frac_part[1] = '0';
		frac_part[2] = '0';
		break;
		case 4:
		frac_part[0] = '6';
		frac_part[1] = '2';
		frac_part[2] = '5';
		break;
		case 5:
		frac_part[0] = '7';
		frac_part[1] = '5';
		frac_part[2] = '0';
		break;
	}
	for (i = 0; i < 3; i++)     // appends fraction
	{
	    if (sizeoffrac < 7)
	    {
	        stringval[sizeoffrac++] = frac_part[i];
	    }
	}
	stringval[sizeoffrac] = '\0';                     // appends ending char
}

												// takes adc value and calculates to Fahrenheit + calibrated
void adctof()
{
	sensorreading = adc_read(0);          // adc value
	temp = (float)sensorreading/1023;    // convert float
	temp = temp * 5000;
	temp = (temp - 500) * .1;
	temp = (temp * 1.8) + 12;
	temp *= 1.1;
}
                                       //Initilizers
enum State {start, s1} state;
void Array_Init()
{
	state = start;
}

void Array_Tick()
{
	switch(state)
	{
		case start:
		adctof();
		if((GetBit(PINB,0) == 1) && (GetBit(PINB,1) == 0))           // read IR sensor vals and change array
		{
			ar[29] = 'I';
			ar[30] = 'N';
			ar[31] = ' ';
			dogstatus = 1;
			eeprom_write_word((uint16_t*)70,dogstatus);           // store in eemprom dog status
		}
		else if((GetBit(PINB,0) == 0) && (GetBit(PINB,1) == 1))   // read IR sensor vals and change array
		{
			ar[29] = 'O';
			ar[30] = 'U';
			ar[31] = 'T';
			dogstatus = 2;
			eeprom_write_word((uint16_t*)70,dogstatus);    // store in eemprom dog status
		}
		else 
		{
			dogstatus = eeprom_read_word((uint16_t*)70);          // read dog status and update array for output to lcd
			if(dogstatus == 1) 
			{
				ar[29] = 'I';
				ar[30] = 'N';
				ar[31] = ' ';
			}
			else if(dogstatus == 2)
			{
				ar[29] = 'O';
				ar[30] = 'U';
				ar[31] = 'T';
			}
		}
		ftc(&test,temp);     // convert float to string
		LCD_ClearScreen();            
		ar[7] = test[0];               //set up array with proper temp
		ar[8] = test[1];
		ar[9] = test[2];
		ar[10] = test[3];
		ar[11] = test[4];
		LCD_DisplayString(1,ar);       // display dog status and temp
		state = start;
		break;
		
	    default:
	    state = start;
		break;
	}
}

void Setarray()           // setting period for sm
{
	Array_Init();
   for(;;) 
   { 	
	Array_Tick();
	vTaskDelay(400); 
   }
}

void StartSecPulse(unsigned portBASE_TYPE Priority)         // declare SMs
{
	xTaskCreate(Setarray, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}	
 
int main(void) 
{ 
   DDRA = 0x00; PORTA = 0xFF;
   DDRB = 0x00; PORTB = 0xFF;
   DDRC = 0xFF; PORTC = 0x00;
   DDRD = 0xFF; PORTD = 0x00;
   LCD_init();
   LCD_ClearScreen();
													//Start Tasks  
   adc_init();
   StartSecPulse(1);
													 //RunSchedular 
   vTaskStartScheduler(); 
   return 0; 
}

/*
I tested this microcontroller by lighting up specifc parts of ports using leds depending on which ir sensor
was sending me a high or low. If ir1 was sending me a low and ir2 a high, higher nipple of portc was set high. If
ir2 is set low and ir1 high, lower nipple of port c is set low. For testing my float to string function, I hardcoded
a temperature and made sure it came out in string form the temperature it should be. 
*/


/*
#include <stdint.h> 
#include <stdlib.h> 
#include <avr/eeprom.h>
#include <avr/io.h>
#include "io.c"
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
float temp;
int i = 0;
unsigned char test[];
char ar[32] = "Temp:           Dog Status:     ";
uint16_t dogstatus;         					  //eeprom
short sensorreading;
															
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b)   // Bit-access function
{
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}
void adc_init()
{
	ADMUX = (1<<REFS0);
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}
													        	// read adc value
uint16_t adc_read(uint8_t ch)
{
	ch &= 0b00000111;
	ADMUX = (ADMUX & 0xF8)|ch;
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	return (ADC);
}

void ftc(char* stringval,float Value)         // float to string converter
{
	unsigned int whole = 7;
	int i = 0;
	unsigned int exponent = 8;
	char frac_part[3];
	unsigned int sign = 1;
	unsigned long answer;
	unsigned int int_part;
	answer += ((unsigned long) whole << 16);
	answer += 0x00800000;
	exponent = (signed char) exponent - 127;
	int sizeoffrac = 0;
	if(sign)
	{
		stringval[0] = '-';
		sizeoffrac++;
	}
	int_part = answer >> (23 - exponent);       // does math to isolate fraction and whole part
	itoa(int_part, &stringval[sizeoffrac],10);
	stringval[sizeoffrac++] = '.';
	switch (0x7 & (answer  >> (20 - exponent)))         // fracation part 
	{
		case 0:
		frac_part[0] = '0';
		frac_part[1] = '0';
		frac_part[2] = '0';
		break;
		case 1:
		frac_part[0] = '2';
		frac_part[1] = '5';
		frac_part[2] = '0';
		break;
		case 2:
		frac_part[0] = '3';
		frac_part[1] = '7';
		frac_part[2] = '5';
		break;
		case 3:
		frac_part[0] = '5';
		frac_part[1] = '0';
		frac_part[2] = '0';
		break;
		case 4:
		frac_part[0] = '6';
		frac_part[1] = '2';
		frac_part[2] = '5';
		break;
		case 5:
		frac_part[0] = '7';
		frac_part[1] = '5';
		frac_part[2] = '0';
		break;
	}
	for (i = 0; i < 3; i++)     // appends fraction
	{
	    if (sizeoffrac < 7)
	    {
	        stringval[sizeoffrac++] = frac_part[i];
	    }
	}
	stringval[sizeoffrac] = '\0';                     // appends ending char
}

												// takes adc value and calculates to Fahrenheit + calibrated
void adctof()
{
	sensorreading = adc_read(0);          // adc value
	temp = (float)sensorreading/1023;    // convert float
	temp = temp * 5000;
	temp = (temp - 500) * .1;
	temp = (temp * 1.8) + 12;
	temp *= 1.1;
}
                                       //Initilizers
enum State {start, s1} state;
void Array_Init()
{
	state = start;
}

void Array_Tick()
{
	switch(state)
	{
		case start:
		adctof();
		if((GetBit(PINB,0) == 1) && (GetBit(PINB,1) == 0))           // read IR sensor vals and set upper nipple port high
		{
			PORTC = 0x0F;
		}
		else if((GetBit(PINB,0) == 0) && (GetBit(PINB,1) == 1))   // read IR sensor vals and set lower nipple port low
		{
			PORTC = 0xF0;
		}
		else 
		{															// set portd high on one pin
			PORTD = 0x01;
		}
		temperature = 70.01;       // hardcoded temp expecting 70.000 on lcd
		ftc(&test,temp);       // convert float to string
		LCD_DisplayString(1,ar);       // display dog status and temp
		state = start;
		break;
		
	    default:
	    state = start;
		break;
	}
}

void Setarray()           // setting period for sm
{
	Array_Init();
   for(;;) 
   { 	
	Array_Tick();
	vTaskDelay(400); 
   }
}

void StartSecPulse(unsigned portBASE_TYPE Priority)         // declare SMs
{
	xTaskCreate(Setarray, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}	
 
int main(void) 
{ 
   DDRA = 0x00; PORTA = 0xFF;
   DDRB = 0x00; PORTB = 0xFF;
   DDRC = 0xFF; PORTC = 0x00;
   DDRD = 0xFF; PORTD = 0x00;
   LCD_init();
   LCD_ClearScreen();
													//Start Tasks  
   adc_init();
   StartSecPulse(1);
													 //RunSchedular 
   vTaskStartScheduler(); 
   return 0; 
}

*/