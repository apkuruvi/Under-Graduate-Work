/* Partner 1 Name & E-mail: Abraham Kuruvila akuru003@ucr.edu
* Partner 2 Name & E-mail: Rogelio Vazquez rvazq002@ucr.edu
* Lab Section: 22
* Assignment: Lab # Exercise #: Lab 7 Exercise 1
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



void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; } //stops timer/counter
		else { TCCR3B |= 0x03; } // resumes/continues timer/counter
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		// prevents OCR3A from underflowing, using prescaler 64
		// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR3A = 0x0000; }
		// set OCR3A based on desired frequency
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }
		TCNT3 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}


void PWM_on() {
	TCCR3A = (1 << COM3A0);
	// COM3A0: Toggle PB6 on compare match between counter and OCR3A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	// WGM32: When counter (TCNT3) matches OCR3A, reset counter
	// CS31 & CS30: Set a prescaler of 64
	set_PWM(0);
}
void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}




enum States {Init, s0, s1, s2} State;

void Tick()
{
	unsigned char tmp2 = ~PINA;
	switch(State)
	{                                   // Transitions
		case Init:
		if(((GetBit(tmp2,0) == 1) && (GetBit(tmp2,2) == 1)) || ((GetBit(tmp2,1) == 1) && (GetBit(tmp2,2) == 1)) || ((GetBit(tmp2,0) == 1) && (GetBit(tmp2,2) == 1)) || ((GetBit(tmp2,0) == 1) && (GetBit(tmp2,2) == 1)  && (GetBit(tmp2,1) == 1)))
		{
			State = Init;
		}
		else if((GetBit(tmp2,0) == 1) && (GetBit(tmp2,1) == 0) && (GetBit(tmp2,2) == 0))
		{
			State = s0;
		}
		else if((GetBit(tmp2,1) == 1) && (GetBit(tmp2,0) == 0) && (GetBit(tmp2,2) == 0))
		{
			State = s1;
		}
		else if((GetBit(tmp2,2) == 1) && (GetBit(tmp2,1) == 0) && (GetBit(tmp2,0) == 0))
		{
			State = s2;
		}
		else
		{
			State = Init;
		}
		break;
		
		case s0:
		if(((GetBit(tmp2,0) == 1) && (GetBit(tmp2,2) == 1)) || ((GetBit(tmp2,1) == 1) && (GetBit(tmp2,2) == 1)) || ((GetBit(tmp2,0) == 1) && (GetBit(tmp2,2) == 1)) || ((GetBit(tmp2,0) == 1) && (GetBit(tmp2,2) == 1)  && (GetBit(tmp2,1) == 1)))
		{
			State = Init;
		}
		else if(GetBit(tmp2,0) == 0)
		{
			State = Init;
		}
		else
		{
			State = s0;
		}
		break;
		
		case s1:
		if(((GetBit(tmp2,0) == 1) && (GetBit(tmp2,2) == 1)) || ((GetBit(tmp2,1) == 1) && (GetBit(tmp2,2) == 1)) || ((GetBit(tmp2,0) == 1) && (GetBit(tmp2,2) == 1)) || ((GetBit(tmp2,0) == 1) && (GetBit(tmp2,2) == 1)  && (GetBit(tmp2,1) == 1)))
		{
			State = Init;
		}
		else if(GetBit(tmp2,1) == 0)
		{
			State = Init;
		}
		else
		{
			State = s1;
		}
		break;
		
		case s2:
		if(((GetBit(tmp2,0) == 1) && (GetBit(tmp2,2) == 1)) || ((GetBit(tmp2,1) == 1) && (GetBit(tmp2,2) == 1)) || ((GetBit(tmp2,0) == 1) && (GetBit(tmp2,2) == 1)) || ((GetBit(tmp2,0) == 1) && (GetBit(tmp2,2) == 1)  && (GetBit(tmp2,1) == 1)))
		{
			State = Init;
		}
		else if(GetBit(tmp2,2) == 0)
		{
			State = Init;
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
		set_PWM(0);
		//tmp = 0x00;
		//PORTB = tmp;
		break;
		
		case s0:
		set_PWM(261.63);
		//tmp = 0x40;
		//PORTB = tmp;
		break;
		
		case s1:
		set_PWM(293.66);
		//tmp = 0x40;
		//PORTB = tmp;
		break;
		
		case s2:
		set_PWM(329.63);
		//tmp = 0x40;
		//PORTB = tmp;
		break;
		
		default:
		
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
	//TimerSet(2);
	//TimerOn()
	PWM_on();
	set_PWM(0);
	while (1)
	{
		Tick();
		//while(!TimerFlag);
		//TimerFlag = 0;
	}
}