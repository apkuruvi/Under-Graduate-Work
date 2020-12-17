/* Partner 1 Name & E-mail: Abraham Kuruvila akuru003@ucr.edu
* Partner 2 Name & E-mail: Rogelio Vazquez rvazq002@ucr.edu
* Lab Section: 22
* Assignment: Lab # Exercise #: Lab 5 Exercise 3
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
unsigned char t = 0x00;

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


enum States { Init, s0, s1, s2, s3, s4, s5, s6} State;

void Tick()
{
	//unsigned char temp1 = 0x00;
	unsigned char temp = ~PINA;
	switch(State)
	{                                   // Transitions
		case Init:                     // Initial transition
		if((GetBit(temp, 0) == 1) && (GetBit(temp, 1) == 1))
		{
			State = s2;
		}
		else if(GetBit(temp, 0) == 1)
		{
			State = s0;
		}
		else if(GetBit(temp, 1) == 1)
		{
			State = s1;
		}
		else
		{
			State = Init;
		}
		break;
		
		case s0:
		if((GetBit(temp, 0) == 1) && (GetBit(temp, 1) == 1))
		{
			State = s2;
		}
		else if(GetBit(temp,0) == 0)
		{
			State = s5;
		}
		else if(GetBit(temp,0) == 1)
		{
			State = s3;
		}
		break;
		
		
		case s1:
		if((GetBit(temp, 0) == 1) && (GetBit(temp, 1) == 1))
		{
			State = s2;
		}
		else if(GetBit(temp,1) == 0)
		{
			State = s5;
		}
		else
		{
			State = s4;
		}
		break;
		
		case s2:
		State = s6;
		break;
		
		case s3:
		if((GetBit(temp, 0) == 1) && (GetBit(temp, 1) == 1))
		{
			State = s2;
		}
		else if(GetBit(temp, 0) == 1)
		{
			State = s3;
		}
		else if(GetBit(temp, 0) == 0)
		{
			State = s5;
		}
		break;
		
		case s4:
		if((GetBit(temp, 0) == 1) && (GetBit(temp, 1) == 1))
		{
			State = s2;
		}
		else if(GetBit(temp, 1) == 1)
		{
			State = s4;
		}
		else if(GetBit(temp, 1) == 0)
		{
			State = s5;
		}
		break;
		
		case s5:
		if((GetBit(temp, 0) == 1) && (GetBit(temp, 1) == 1))
		{
			State = s2;
		}
		else if(GetBit(temp, 0) == 1)
		{
			State = s0;
		}
		else if(GetBit(temp, 1) == 1)
		{
			State = s1;
		}
		else
		{
			State = s5;
		}
		break;
		
		
		case s6:
		if((GetBit(temp, 0) == 0) && (GetBit(temp, 1) == 0))
		{
			State = s5;
		}
		else
		{
			
			State = s6;
		}
		break;
		
		default:
		State = Init;
		break;
	} // Transitions

	switch(State)
	{
		// State actions
		case Init:
		t = 0x00;
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_WriteData( 0 + '0' );
		break;
		
		case s0:
		if(t < 0x09)
		{
			if(t == 0x00) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 1 + '0'); }
			if(t == 0x01) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 2 + '0'); }
			if(t == 0x02) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 3 + '0'); }	
			if(t == 0x03) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 4 + '0'); }
			if(t == 0x04) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 5 + '0'); }
			if(t == 0x05) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 6 + '0'); }
			if(t == 0x06) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 7 + '0'); }
			if(t == 0x07) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 8 + '0'); }
			if(t == 0x08) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 9 + '0'); }
			t = t + 1;
		}
		break;
		
		case s1:
		if(t > 0x00)
		{
			if(t == 0x09) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 8 + '0'); }
			if(t == 0x08) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 7 + '0'); }
			if(t == 0x07) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 6 + '0'); }
			if(t == 0x06) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 5 + '0'); }
			if(t == 0x05) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 4 + '0'); }
			if(t == 0x04) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 3 + '0'); }
			if(t == 0x03) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 2 + '0'); }
			if(t == 0x02) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 1 + '0'); }
			if(t == 0x01) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 0 + '0'); }
			t = t - 1;
		}
		break;
		
		case s2:
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_WriteData( 0 + '0' );
		t = 0x00;
		break;

		case s3:
		if(t < 0x09)
		{
			if(t == 0x00) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 1 + '0'); }
			if(t == 0x01) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 2 + '0'); }
			if(t == 0x02) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 3 + '0'); }
			if(t == 0x03) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 4 + '0'); }
			if(t == 0x04) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 5 + '0'); }
			if(t == 0x05) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 6 + '0'); }
			if(t == 0x06) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 7 + '0'); }
			if(t == 0x07) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 8 + '0'); }
			if(t == 0x08) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 9 + '0'); }
			t = t + 1;
		}
		break;
		
		case s4:
		if(t > 0x00)
		{
			if(t == 0x09) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 8 + '0'); }
			if(t == 0x08) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 7 + '0'); }
			if(t == 0x07) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 6 + '0'); }
			if(t == 0x06) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 5 + '0'); }
			if(t == 0x05) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 4 + '0'); }
			if(t == 0x04) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 3 + '0'); }
			if(t == 0x03) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 2 + '0'); }
			if(t == 0x02) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 1 + '0'); }
			if(t == 0x01) { LCD_ClearScreen(); LCD_Cursor(1); LCD_WriteData( 0 + '0'); }
			t = t - 1;
		}
		break;
		
		case s5:
		break;
		
		case s6:
		break;
		
		default:
		break;
	} // State actions
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	State = Init;
	LCD_init();
	TimerSet(1000);
	TimerOn();
	while (1)
	{
		Tick();
		while(!TimerFlag);
		TimerFlag = 0;
	}
}