/* Partner 1 Name & E-mail: Abraham Kuruvila akuru003@ucr.edu
* Partner 2 Name & E-mail: Rogelio Vazquez rvazq002@ucr.edu
* Lab Section: 22
* Assignment: Lab # Exercise #: Lab 9 Exercise 3
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
unsigned char count = 0x00;
unsigned char b1time = 0x00;
unsigned char b2time = 0x00;
unsigned char increment = 0x00;
unsigned char decrement = 0x00;
unsigned char both = 0x00;
unsigned char ten = 0x00;
unsigned char ten1 = 0x00;
unsigned char four = 0x00;
unsigned char four1 = 0x00;

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



enum States1 { Init1, s01, s11, s21, s31, s41, s51, s61} State1;

void Tick1()
{
	switch(State1)
	{                                   // Transitions
		case Init1:
		if(both == 0x01)
		{
			State1 = s01;
		}
		else if(increment == 0x01)
		{
			State1 = s51;
			b1time = b1time + 1;
		}
		else if(decrement == 0x01)
		{
			State1 = s61;
			b2time = b2time + 1;
		}
		else
		{
			State1 = Init1;
			ten = 0;
			b1time = 0;
			b2time = 0;
			four = 0;
			ten1 = 0;
			four1 = 0;
		}
		break;
		
		case s01:
		if(both == 0x01)
		{
			State1 = s01;
		}
		else
		{
			State1 = Init1;
			ten = 0;
			b1time = 0;
			b2time = 0;
			four = 0;
			ten1 = 0;
			four1 = 0;
			
		}
		break;
		
		case s11:
		if(both == 0x01)
		{
			State1  = s01;
		}
		else if(b1time >= 30)
		{
			State1 = s31;
			b1time = 0;
		}
		else if(increment == 0x01)
		{
			State1 = s11;
			b1time = b1time + 1;
		}
		else
		{
			State1 = Init1;
			ten = 0;
			b1time = 0;
			b2time = 0;
			four = 0;
			ten1 = 0;
			four1 = 0;
		}
		break;
		
		case s21:
		if(both == 0x01)
		{
			State1 = s01;
		}
		else if(b2time >= 30)
		{
			State1 = s41;
			b2time = 0;
		}
		else if(decrement == 0x01)
		{
			State1 = s21;
			b2time = b2time + 1;
		}
		else
		{
			State1 = Init1;
			ten = 0;
			b2time = 0;
			b1time = 0;
			four = 0;
			ten1 = 0;
			four1 = 0;
		}
		break;
		
		case s31:
		if(both == 0x01)
		{
			State1 = s01;
		}
		else if(increment == 0x01)
		{
			State1 = s31;
		}
		else
		{
			State1 = Init1;
			ten = 0;
			b1time = 0;
			b2time = 0;
			four = 0;
			four1 = 0;
		}
		break;
		
		case s41:
		if(both == 0x01)
		{
			State1 = s01;
		}
		else if(decrement == 0x01)
		{
			State1 = s41;
		}
		else
		{
			State1 = Init1;
			ten = 0;
			b2time = 0;
			four1 = 0;
			ten1 = 0;
			b1time = 0;
		}
		break;
		
		case s51:
		State1 = s11;
		break;
		
		case s61:
		State1 = s21;
		break;
		
		
		
		default:
		State1 = Init1;
		break;
	} // Transitions

	switch(State1)
	{
		// State actions
		case Init1:
		break;
		
		case s01:
		count = 0;
		tmp = 0x00;
		PORTB = tmp;
		break;
		
		case s11:
		ten = ten + 1;
		if(ten == 10)
		{
			ten = 0;
			if(count >= 0x09)
			{}
			else
			{
				count = count + 1;
				PORTB = count;
			}
		}
		break;
		
		case s21:
		ten1 = ten1 + 1;
		if(ten1 == 10)
		{
			ten1 = 0;
			if(count <= 0x00)
			{}
			else
			{
				count = count - 1;
				PORTB = count;
			}
		}
		break;
		
		case s31:
		four = four + 1;
		if(four == 4)
		{
			four = 0;
			if(count >= 0x09)
			{}
			else
			{
				count = count + 1;
				PORTB = count;
			}
		}
		break;
		
		case s41:
		four1 = four1 + 1;
		if(four1 == 4)
		{
			four1 = 0;
			if(count <= 0x00)
			{}
			else
			{
				count = count - 1;
				PORTB = count;
			}
		}
		break;
		
		case s51:
		if(count >= 0x09)
		{}
		else
		{
			count = count + 1;
			PORTB = count;
		}
		State1 = s11;
		break;
		
		case s61:
		four1 = 0;
		if(count <= 0x00)
		{}
		else
		{
			count = count - 1;
			PORTB = count;
		}
		State1 = s21;
		break;
		
		default:
		PORTB = 0x00;
		break;
	} // State actions
}





enum States { Init, s0, s1, s2} State;

void Tick()
{
	unsigned temp = ~PINA;
	switch(State)
	{                                   // Transitions
		case Init:
		if((GetBit(temp, 0) == 1) && (GetBit(temp, 1) == 1))
		{
			State = s0;
			both = 0x01;
			increment = 0x00;
			decrement = 0x00;
		}
		else if((GetBit(temp, 0) == 1))
		{
			State = s1;
			both = 0x00;
			decrement = 0x00;
			increment = 0x01;
		}
		else if((GetBit(temp, 1) == 1))
		{
			State = s2;
			both = 0x00;
			increment = 0x00;
			decrement = 0x01;
		}
		else
		{
			State = Init;
		}
		break;
		
		case s0:
		if((GetBit(temp, 0) == 1) && (GetBit(temp, 1) == 1))
		{
			both = 0x01;
			State = s0;
			increment = 0x00;
			decrement = 0x00;
		}
		else
		{
			State = Init;
			both = 0x00;
		}
		break;
		
		case s1:
		if((GetBit(temp, 0) == 1) && (GetBit(temp, 1) == 1))
		{
			both = 0x01;
			State = s0;
			increment = 0x00;
			decrement = 0x00;
		}
		else if((GetBit(temp, 0) == 1))
		{
			both = 0x00;
			State = s1;
			increment = 0x01;
			decrement = 0x00;
		}
		else
		{
			State = Init;
			increment = 0x00;
		}
		break;
		
		case s2:
		if((GetBit(temp, 0) == 1) && (GetBit(temp, 1) == 1))
		{
			both = 0x01;
			State = s0;
			increment = 0x00;
			decrement = 0x00;
		}
		else if((GetBit(temp, 1) == 1))
		{
			State = s2;
			both = 0x00;
			decrement = 0x01;
			increment = 0x00;
		}
		else
		{
			State = Init;
			decrement = 0x00;
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
		break;
		
		case s1:
		break;
		
		case s2:
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
	State1 = Init1;
	TimerSet(100);
	TimerOn();
	while (1)
	{
		Tick();
		Tick1();
		while(!TimerFlag);
		TimerFlag = 0;
	}
}