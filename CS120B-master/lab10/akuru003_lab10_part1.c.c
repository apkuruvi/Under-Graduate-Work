/* Partner 1 Name & E-mail: Abraham Kuruvila akuru003@ucr.edu
* Partner 2 Name & E-mail: Rogelio Vazquez rvazq002@ucr.edu
* Lab Section: 22
* Assignment: Lab # Exercise #: Lab 10 Exercise 1
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/


/// PINOUTS -STARTING AT d, 1-c3,2 - c2,3-c1, 4 - c0,5-c8,6-c7,7-c6,8 -c5
/////
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>


volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a = b;
		b = c;
	}
	return 0;
}


typedef struct _task {
	/*Tasks should have members that include: state, period,
	a measurement of elapsed time, and a function pointer.*/
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;

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







// Returns '\0' if no key pressed, else returns char '1', '2', ... '9', 'A', ...
// If multiple keys pressed, returns leftmost-topmost one
// Keypad must be connected to port C
/* Keypad arrangement
 PC4 PC5 PC6 PC7
 col 1 2 3 4
row
PC0 1 1 | 2 | 3 | A
PC1 2 4 | 5 | 6 | B
PC2 3 7 | 8 | 9 | C
PC3 4 * | 0 | # | D
*/
enum States {s0, s1, s2 , s3 , s4 , s5 , s6 , s7 , s8 , s9 , sA , sB , sC , sD , s11 , s12 , s01} State; 
int GetKeypadKey() 
{
	switch(State)
	{
		case s0:
		PORTC = 0xEF; 
		asm("nop"); 
		if (GetBit(PINC,0)==0) { State = s1; }
		if (GetBit(PINC,1)==0) { State = s4; }
		if (GetBit(PINC,2)==0) { State = s7; }
		if (GetBit(PINC,3)==0) { State = s12; }
		PORTC = 0xDF; 
		asm("nop"); 
		if (GetBit(PINC,0)==0) { State = s2; }
		if (GetBit(PINC,1)==0) { State = s5; }
		if (GetBit(PINC,2)==0) { State = s8; }
		if (GetBit(PINC,3)==0) { State = s01; }
		
		PORTC = 0xBF; 
		asm("nop"); 
		if (GetBit(PINC,0)==0) { State = s3; }
		if (GetBit(PINC,1)==0) { State = s6; }
		if (GetBit(PINC,2)==0) { State = s9; }
		if (GetBit(PINC,3)==0) { State = s11; }
		
		PORTC = 0x7F; 
		asm("nop"); 
		if (GetBit(PINC,0)==0) { State = sA; }
		if (GetBit(PINC,1)==0) { State = sB; }
		if (GetBit(PINC,2)==0) { State = sC; }
		if (GetBit(PINC,3)==0) { State = sD; }
		break;
		
		case s1:
		PORTC = 0xEF;
		asm("nop");
		if (GetBit(PINC,0)==0) { State = s1; }
		else
		{
			 State = s0;
		}
		break;
		
		case s2:
		PORTC = 0xDF;
		asm("nop");
		if (GetBit(PINC,0)==0) { State = s2; }
		else
		{
			State = s0;
		}
		break;
		
		case s3:
		PORTC = 0xBF;
		asm("nop");
		if (GetBit(PINC,0)==0) { State = s3; }
		else
		{
			State = s0;
		}
		break;
		
		case s4:
		PORTC = 0xEF;
		asm("nop");
		if (GetBit(PINC,1)==0) { State = s4; }
		else
		{
			State = s0;
		}
		break;
		
		case s5:
		PORTC = 0xDF;
		asm("nop");
		if (GetBit(PINC,1)==0) { State = s5; }
		else
		{
			State = s0;
		}
		break;
		
		case s6:
		PORTC = 0xBF;
		asm("nop");
		if (GetBit(PINC,1)==0) { State = s6; }
		else
		{
			State = s0;
		}
		break;
				
		case s7:
		PORTC = 0xEF;
		asm("nop");
		if (GetBit(PINC,2)==0) { State = s7; }
		else
		{
			State = s0;
		}
		break;
		
		case s8:
		PORTC = 0xDF;
		asm("nop");
		if (GetBit(PINC,2)==0) { State = s8; }
		else
		{
			State = s0;
		}
		break;
		
		case s9:
		PORTC = 0xBF;
		asm("nop");
		if (GetBit(PINC,2)==0) { State = s9; }
		else
		{
			State = s0;
		}
		break;
		
		case sA:
		PORTC = 0x7F;
		asm("nop");
		if (GetBit(PINC,0)==0) { State = sA; }
		else
		{
			State = s0;
		}
		break;
		
		case sB:
		PORTC = 0x7F;
		asm("nop");
		if (GetBit(PINC,1)==0) { State = sB; }
		else
		{
			State = s0;
		}
		break;
		
		case sC:
		PORTC = 0x7F;
		asm("nop");
		if (GetBit(PINC,2)==0) { State = sC; }
		else
		{
			State = s0;
		}
		break;
		
		case sD:
		PORTC = 0x7F;
		asm("nop");
		if (GetBit(PINC,3)==0) { State = sD; }
		else
		{
			State = s0;
		}
		break;
		
		case s11:
		PORTC = 0xBF;
		asm("nop");
		if (GetBit(PINC,3)==0) { State = s11; }
		else
		{
			State = s0;
		}
		break;
		
		case s12:
		PORTC = 0xEF;
		asm("nop");
		if (GetBit(PINC,3)==0) { State = s12; }
		else
		{
			State = s0;
		}
		break;
		
		case s01:
		PORTC = 0xDF;
		asm("nop");
		if (GetBit(PINC,3)==0) { State = s01; }
		else
		{
			State = s0;
		}
		break;
		
	}
	
	switch(State)
	{
		case s0: PORTB = 0x1F; break; // All 5 LEDs on
		case s1: PORTB = 0x01; break; // hex equivalent
		case s2: PORTB = 0x02; break;
		case s3: PORTB = 0x03; break; // All 5 LEDs on
		case s4: PORTB = 0x04; break; // hex equivalent
		case s5: PORTB = 0x05; break;
		case s6: PORTB = 0x06; break; // All 5 LEDs on
		case s7: PORTB = 0x07; break; // hex equivalent
		case s8: PORTB = 0x08; break;
		case s9: PORTB = 0x09; break; // All 5 LEDs on
		case sA: PORTB = 0x0A; break; // hex equivalent
		case sB: PORTB = 0x0B; break;
		case sC: PORTB = 0x0C; break;
		case sD: PORTB = 0x0D; break;
		case s11: PORTB = 0x0F; break;
		case s01: PORTB = 0x00; break;
		case s12: PORTB = 0x0E; break;
		default: PORTB = 0x1B; break; // Should never occur. Middle LED
	}
	return State;
}
int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; // PORTB set to output, outputs init 0s
	DDRC = 0xF0; PORTC = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
	//unsigned long int SMTick1_calc = 100;
	static task task1;
	task *tasks[] = { &task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	task1.state = -1;//Task initial state.
	task1.period = 100;//Task Period.
	task1.elapsedTime = 100;//Task current elapsed time.
	task1.TickFct = &GetKeypadKey;//Function pointer for the tick.
	
	TimerSet(1);
	TimerOn();
	State = s0;
	unsigned char i = 0;
	while(1) 
	{
	    for ( i = 0; i < numTasks; i++ ) 
		{
		    if ( tasks[i]->elapsedTime == tasks[i]->period )
			 {
			    tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			    tasks[i]->elapsedTime = 0;
		    }
		    tasks[i]->elapsedTime += 1;
	    }
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 0;
}




