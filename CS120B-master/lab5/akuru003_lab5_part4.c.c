/* Partner 1 Name & E-mail: Abraham Kuruvila akuru003@ucr.edu
* Partner 2 Name & E-mail: Rogelio Vazquez rvazq002@ucr.edu
* Lab Section: 22
* Assignment: Lab # Exercise #: Lab 5 Exercise 4
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"


volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char tmp = 0x00;
unsigned char tmp3 = 0x05;

void TimerOn()
{
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff()
{
	TCCR1B = 0x00;
}

// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

void TimerISR()
{
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect)
{
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0)
	{
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
		
	}
}

void TimerSet(unsigned long M)
{
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


enum States {Init, s0, s1, s2, s3, s4, s5, s6, s7} State;

void Tick()
{
	unsigned char tmp2 = ~PINA;
	switch(State)
	{                                   // Transitions
		case Init:
		State = s0;
		break;
		
		case s0:
		if(GetBit(tmp2,0) == 1)
		{
			State = s3;
		}
		else
		{
			State = s1;
		}
		break;
		
		case s1:
		if(GetBit(tmp2,0) == 1)
		{
			State = s4;
		}
		else
		{
			State = s2;
		}
		break;
		
		case s2:
		if(GetBit(tmp2,0) == 1)
		{
			State = s5;
		}
		else
		{
			State = s0;
		}
		break;
		
		case s3:
		State = s6;
		break;
		
		case s4:
		State = s6;
		break;
		
		case s5:
		State = s6;
		break;
		
		case s6:
		if(GetBit(tmp2, 0) == 0)
		{
			State = Init;
		}
		else
		{
			State = s6;
		}
		break;
		
		case s7:
		LCD_ClearScreen();
		LCD_DisplayString(1, "Winner!");
		break;
		
		default:
		State = Init;
		break;
	} // Transitions

	switch(State)
	{
		// State actions
		case Init:
		break;
		
		case s0:
		tmp = 0x01;
		PORTB = tmp;
		break;
		
		case s1:
		tmp = 0x02;
		PORTB = tmp;
		break;
		
		case s2:
		tmp = 0x04;
		PORTB = tmp;
		break;
		
		case s3:
		tmp = 0x01;
		if(tmp3 == 0x09) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 8 + '0'); }
		if(tmp3 == 0x08) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 7 + '0'); }
		if(tmp3 == 0x07) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 6 + '0'); }
		if(tmp3 == 0x06) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 5 + '0'); }
		if(tmp3 == 0x05) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 4 + '0'); }
		if(tmp3 == 0x04) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 3 + '0'); }
		if(tmp3 == 0x03) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 2 + '0'); }
		if(tmp3 == 0x02) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 1 + '0'); }
		if(tmp3 == 0x01) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 0 + '0'); }
		if(tmp3 > 0x00)
		{
			tmp3 = tmp3 - 0x01;
		}
		PORTB = tmp;
		break;
		
		case s4:
		tmp = 0x02;
		if(tmp3 == 0x08) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 9 + '0'); }
		if(tmp3 == 0x07) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 8 + '0'); }
		if(tmp3 == 0x06) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 7 + '0'); }
		if(tmp3 == 0x05) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 6 + '0'); }
		if(tmp3 == 0x04) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 5 + '0'); }
		if(tmp3 == 0x03) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 4 + '0'); }
		if(tmp3 == 0x02) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 3 + '0'); }
		if(tmp3 == 0x01) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 2 + '0'); }
		if(tmp3 == 0x00) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 1 + '0'); }
	    if(tmp3 < 0x09)
		{
			tmp3 = tmp3 + 0x01;
		}
		if(tmp3 == 0x009)
		{
			State = s7;
		}
		PORTB = tmp;
		break;
		
		case s5:
		tmp = 0x04;
		if(tmp3 == 0x09) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 8 + '0'); }
		if(tmp3 == 0x08) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 7 + '0'); }
		if(tmp3 == 0x07) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 6 + '0'); }
		if(tmp3 == 0x06) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 5 + '0'); }
		if(tmp3 == 0x05) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 4 + '0'); }
		if(tmp3 == 0x04) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 3 + '0'); }
		if(tmp3 == 0x03) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 2 + '0'); }
		if(tmp3 == 0x02) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 1 + '0'); }
		if(tmp3 == 0x01) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 0 + '0'); }
		if(tmp3 > 0x00)
		{
			tmp3 = tmp3 - 0x01;
		}
		PORTB = tmp;
		break;
		
		case s6:
		break;
		
		case s7:
		tmp = 0x00;
		PORTB = tmp;
		break;
		
		default:
		PORTB = 0x00;
		break;
	} // State actions
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	// intermediate variable used for port updates
	/* Replace with your application code */
	
	State = Init;
	LCD_init();
	LCD_ClearScreen(); 
	LCD_Cursor(1);
	LCD_WriteData( 5 + '0');
	TimerSet(300);
	TimerOn();
	while (1)
	{
		Tick();
		while(!TimerFlag);
		TimerFlag = 0;
	}
}