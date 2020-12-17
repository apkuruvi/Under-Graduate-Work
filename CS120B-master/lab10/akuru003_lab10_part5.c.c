/* Partner 1 Name & E-mail: Abraham Kuruvila akuru003@ucr.edu
* Partner 2 Name & E-mail: Rogelio Vazquez rvazq002@ucr.edu
* Lab Section: 22
* Assignment: Lab # Exercise #: Lab 10 Exercise 5
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/


/// PINOUTS -STARTING AT d, 1-c3,2 - c2,3-c1, 4 - c0,5-c8,6-c7,7-c6,8 -c5
/////
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "io.c"

unsigned char pos1 = 100;
unsigned char pos2 = 100;
unsigned char pos3 = 1;
volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char counter = 0;
unsigned char counter2 = 1;
unsigned char counter3 = 0;
unsigned char counter4 = 0;
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





enum States1 {Start1, Start2, s01, s11, s12, s13, s21} State1; 

int GetKeypadKey1()
{
	switch(State1)
	{
		case Start1:
		if(GetBit(PINB,0)== 0)
		{
			counter3 = 1;
			counter2 = 0;  // ok to move screen
			counter = 0;  // which patten 
			State1 = s11;
		}
		else
		{
		   State1 = Start1;	
		}
		break;
		
		case Start2:
		if(GetBit(PINB,0)== 0)
		{
			counter3 = 1;
			counter2 = 0;  // ok to move screen
			counter = counter4;
			State1 = s11;
		}
		else
		{
			State1 = Start2;
		}
		break;
		
		case s11:
		if(GetBit(PINB,0)== 1)
		{
			State1 = s01;
		}
		else
		{
			State1 = s11;
		}
		break;
		
		case s01:
		if(GetBit(PINB,0)== 0)
		{
			counter2 = 1;
			counter4 = counter;
			State1 = Start2;
		}
		else if((pos3 == pos1) || (pos3 == pos2))
		{
			counter2 = 1;
			State1 = s21;
		}
		else if(GetBit(PINB,1)== 0)
		{
			LCD_Cursor(1);
			pos3 = 1;
			State1 = s12;
		}
		else if(GetBit(PINB,2)== 0)
		{
			LCD_Cursor(17);
			pos3 = 17;
			State1 = s13;
		}
		break;
		
		case s12:
		if((pos3 == pos1) || (pos3 == pos2))
		{
			counter2 = 1;
			State1 = s21;
		}
		else if(GetBit(PINB,1)== 1)
		{
			State1 = s01;
		}
		else
		{
			LCD_Cursor(1);
			State1 = s12;
		}
		break;
		
		case s13:
        if((pos3 == pos1) || (pos3 == pos2))
        {
			counter2 = 1;
	        State1 = s21;
        }
		else if(GetBit(PINB,2)== 1)
		{
			State1 = s01;
		}
		else
		{
			LCD_Cursor(17);
			State1 = s13;
		}
		break;
		
		case s21:
		counter2 = 1;
		counter = 1;
		LCD_ClearScreen(); LCD_DisplayString(1,"Game Over!");
		State1 = Start1;
		counter3 = 0;
		break;
		
	}
	switch(State1)
	{
		case Start1:
		break;
		case Start2:
		break;
		case s01:
		break;
		case s11:
		break;
		case s12:
		break;
		case s13:
		break;
		case s21:
		break;
		
	}
	return State1;
}
	
enum States {Start, s0} State; 
int GetKeypadKey() 
{
	switch(State)
	{
		case Start:
		State = s0;
		break;
		
		case s0:
		if(counter == 15)
		{
			counter = 0;
		}
		else
		{
			counter = counter + 1;
		}		
		State = s0;
		break;
		
		
	}
	
	switch(State)
	{
		case Start:
		break;
		
		case s0:
		if((counter == 0) && (counter2 == 0)) { pos1 = 10; pos2 = 30; LCD_ClearScreen(); LCD_DisplayString(1,"         #                   *  ");}
		if((counter == 1) && (counter2 == 0))  { pos1 = 9; pos2 = 29; LCD_ClearScreen(); LCD_DisplayString(1,"        #                   *   ");}
		if((counter == 2) && (counter2 == 0))  { pos1 = 8; pos2 = 28; LCD_ClearScreen(); LCD_DisplayString(1,"       #                   *    ");}
		if((counter == 3) && (counter2 == 0))  { pos1 = 7; pos2 = 27; LCD_ClearScreen(); LCD_DisplayString(1,"      #                   *     ");}
		if((counter == 4) && (counter2 == 0))  { pos1 = 6; pos2 = 26; LCD_ClearScreen(); LCD_DisplayString(1,"     #                   *      ");}		
		if((counter == 5) && (counter2 == 0))  { pos1 = 5; pos2 = 25; LCD_ClearScreen(); LCD_DisplayString(1,"    #                   *       ");}
		if((counter == 6) && (counter2 == 0))  { pos1 = 4; pos2 = 24; LCD_ClearScreen(); LCD_DisplayString(1,"   #                   *        ");}
		if((counter == 7) && (counter2 == 0))  { pos1 = 3; pos2 = 23; LCD_ClearScreen(); LCD_DisplayString(1,"  #                   *         ");}
		if((counter == 8) && (counter2 == 0))  { pos1 = 2; pos2 = 22; LCD_ClearScreen(); LCD_DisplayString(1," #                   *          ");}
		if((counter == 9) && (counter2 == 0))  { pos1 = 1; pos2 = 21; LCD_ClearScreen(); LCD_DisplayString(1,"#                   *           ");}	
		if((counter == 10) && (counter2 == 0))  { pos1 = 16; pos2 = 20; LCD_ClearScreen(); LCD_DisplayString(1,"               #   *            ");}
		if((counter == 11) && (counter2 == 0))  { pos1 = 15; pos2 = 19; LCD_ClearScreen(); LCD_DisplayString(1,"              #   *             ");}
		if((counter == 12) && (counter2 == 0))  { pos1 = 14; pos2 = 18; LCD_ClearScreen(); LCD_DisplayString(1,"             #   *              ");}
		if((counter == 13) && (counter2 == 0))  { pos1 = 13; pos2 = 17; LCD_ClearScreen(); LCD_DisplayString(1,"            #   *               ");}
		if((counter == 14) && (counter2 == 0))  { pos1 = 12; pos2 = 32; LCD_ClearScreen(); LCD_DisplayString(1,"           #                   *");}
		if((counter == 15) && (counter2 == 0))  { pos1 = 11; pos2 = 31; LCD_ClearScreen(); LCD_DisplayString(1,"          #                   * ");}		
		if(counter3 == 1)
		{
		   LCD_Cursor(pos3);	
		}
		else if(counter == 0)
		{
			LCD_Cursor(1);	
		}
		else
		{
			LCD_Cursor(11);
		}
		break;
		
		
		default:
		 break; 
	}
	return State;
}
int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0x00; PORTB = 0xFF; // PORTB set to output, outputs init 0s
	DDRC = 0xF0; PORTC = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s[[
	//unsigned long int SMTick1_calc = 100;
	DDRD = 0xFF; PORTD = 0x00;
	static task task1, task2;
	task *tasks[] = { &task1, &task2};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	task1.state = -1;//Task initial state.
	task1.period = 200;//Task Period.
	task1.elapsedTime = 200;//Task current elapsed time.
	task1.TickFct = &GetKeypadKey1;//Function pointer for the tick.
	task2.state = -1;//Task initial state.
	task2.period = 200;//Task Period.
	task2.elapsedTime = 200;//Task current elapsed time.
	task2.TickFct = &GetKeypadKey;//Function pointer for the tick.
	LCD_init();
	TimerSet(1);
	TimerOn();
	State = Start;
	State1 = Start1;
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




