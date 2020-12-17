/* Partner 1 Name & E-mail: Abraham Kuruvila akuru003@ucr.edu
* Partner 2 Name & E-mail: Rogelio Vazquez rvazq002@ucr.edu
* Lab Section: 22
* Assignment: Lab # Exercise #: Lab 5 Exercise 2
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
#include <avr/interrupt.h>


volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char tmp = 0x00;

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


enum States { Init, s0, s1, s2, s3, s4, s5, s6, s7, s8} State;

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
			State = s7;
		}
		else
		{
			State = s6;
		}
		break;
		
		case s7:
		if(GetBit(tmp2, 0) == 1)
		{
			State = s8;
		}
		else
		{
			State = s7;
		}
		break;
		
		case s8:
		if(GetBit(tmp2, 0) == 0)
		{
			State = Init;
		}
		else
		{
			State = s8;
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
		PORTB = tmp;
		break;
		
		case s4:
		tmp = 0x02;
		PORTB = tmp;
		break;
		
		case s5:
		tmp = 0x04;
		PORTB = tmp;
		break;
		
		case s6:
		break;
		
		case s7:
		break;
		
		case s8:
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
	
	// intermediate variable used for port updates
	/* Replace with your application code */
	
	State = Init;
	TimerSet(300);
	TimerOn();
	while (1)
	{
		Tick();
		while(!TimerFlag);
		TimerFlag = 0;
	}
}