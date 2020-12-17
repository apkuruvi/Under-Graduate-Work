/* Partner 1 Name & E-mail: Abraham Kuruvila akuru003@ucr.edu
* Partner 2 Name & E-mail: Rogelio Vazquez rvazq002@ucr.edu
* Lab Section: 22
* Assignment: Lab # Exercise #: Lab 9 Exercise 2
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
unsigned char threeleds = 0x00;
unsigned char blinkingled = 0x00;
unsigned char counter = 0x00;
unsigned char counter1 = 0x00;

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



enum States2 { Init2 } State2;

void Tick2()
{
	switch(State2)
	{                                   // Transitions
		case Init2:
		State2 = Init2;
		break;
		
		default:
		State2 = Init2;
		break;
	} // Transitions

	switch(State2)
	{
		// State actions
		case Init2:
		tmp = threeleds;
		tmp = SetBit(tmp,3,GetBit(blinkingled,3));
		PORTB = tmp;
		break;
		
		default:
		tmp = threeleds;
		tmp = SetBit(tmp,3,GetBit(blinkingled,3));
		PORTB = tmp;
		break;
	} // State actions
}




enum States1 { Init1, s01, s11} State1;

void Tick1()
{
	switch(State1)
	{                                   // Transitions
		case Init1:
		State1 = s01;
		break;
		
		case s01:
		counter1 = counter1 + 1;
		if(counter1 > 10)
		{
			State1 = s11;
			counter1 = 0x00;
		}
		else
		{
			State1 = s01;
		}
		break;
		
		case s11:
		counter1 = counter1 + 1;
		if(counter1 > 10)
		{
			State1 = s01;
			counter1 = 0x00;
		}
		else
		{
			State1 = s11;
		}
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
		blinkingled = 0x08;
		break;
		
		case s11:
		blinkingled = 0x00;
		break;
		
		default:
		blinkingled = 0x00;
		break;
	} // State actions
}




enum States { Init, s0, s1, s2} State;

void Tick()
{
	switch(State)
	{                                   // Transitions
		case Init:
		State = s0;
		break;
		
		case s0:
		counter = counter + 1;
		if(counter > 3)
		{
			State = s1;
			counter = 0x00;
		}
		else
		{
			State = s0;
		}
		break;
		
		case s1:
		counter = counter + 1;
		if(counter > 3)
		{
			State = s2;
			counter = 0x00;
		}
		else
		{
			State = s1;
		}
		break;
		
		case s2:
		counter = counter + 1;
		if(counter > 3)
		{
			State = s0;
			counter = 0x00;
		}
		else
		{
			State = s2;
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
		threeleds = 0x01;
		break;
		
		case s1:
		threeleds = 0x02;
		break;
		
		case s2:
		threeleds = 0x04;
		break;
		
		default:
		threeleds = 0x00;
		break;
	} // State actions
}


int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;
	
	// intermediate variable used for port updates
	/* Replace with your application code */
	
	State = Init;
	TimerSet(100);
	TimerOn();
	while (1)
	{
		Tick();
		Tick1();
		Tick2();
		while(!TimerFlag);
		TimerFlag = 0;
	}
}