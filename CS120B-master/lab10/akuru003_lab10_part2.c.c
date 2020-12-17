/* Partner 1 Name & E-mail: Abraham Kuruvila akuru003@ucr.edu
* Partner 2 Name & E-mail: Rogelio Vazquez rvazq002@ucr.edu
* Lab Section: 22
* Assignment: Lab # Exercise #: Lab 10 Exercise 2
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


volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char counter1 = 0x00;

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





enum States {s0} State; 
int GetKeypadKey() 
{
	//LCD_ClearScreen();
	switch(State)
	{
		case s0:
		if(counter1 == 0){LCD_ClearScreen(); LCD_DisplayString(1, "               C");}
	        if(counter1 == 1){LCD_ClearScreen(); LCD_DisplayString(1, "              CS");}
		if(counter1 == 2){LCD_ClearScreen(); LCD_DisplayString(1, "             CS1");}
		if(counter1 == 3){LCD_ClearScreen(); LCD_DisplayString(1, "            CS12");}
		if(counter1 == 4){LCD_ClearScreen(); LCD_DisplayString(1, "           CS120");}
		if(counter1 == 5){LCD_ClearScreen(); LCD_DisplayString(1, "          CS120B");}
		if(counter1 == 6){LCD_ClearScreen(); LCD_DisplayString(1, "         CS120B ");}
		if(counter1 == 7){LCD_ClearScreen(); LCD_DisplayString(1, "        CS120B i");}
		if(counter1 == 8){LCD_ClearScreen(); LCD_DisplayString(1, "       CS120B is");}
		if(counter1 == 9){LCD_ClearScreen(); LCD_DisplayString(1, "      CS120B is ");}
		if(counter1 == 10){LCD_ClearScreen(); LCD_DisplayString(1, "     CS120B is L");}
		if(counter1 == 11){LCD_ClearScreen(); LCD_DisplayString(1, "    CS120B is Le");}
		if(counter1 == 12){LCD_ClearScreen(); LCD_DisplayString(1, "   CS120B is Leg");}
		if(counter1 == 13){LCD_ClearScreen(); LCD_DisplayString(1, "  CS120B is Lege");}
		if(counter1 == 14){LCD_ClearScreen(); LCD_DisplayString(1, " CS120B is Legen");}
		if(counter1 == 15){LCD_ClearScreen(); LCD_DisplayString(1, "CS120B is Legend");}
		if(counter1 == 16){LCD_ClearScreen(); LCD_DisplayString(1, "S120B is Legend.");}
		if(counter1 == 17){LCD_ClearScreen(); LCD_DisplayString(1, "120B is Legend..");}
		if(counter1 == 18){LCD_ClearScreen(); LCD_DisplayString(1, "20B is Legend...");}
		if(counter1 == 19){LCD_ClearScreen(); LCD_DisplayString(1, "0B is Legend... ");}
		if(counter1 == 20){LCD_ClearScreen(); LCD_DisplayString(1, "B is Legend... w");}
		if(counter1 == 21){LCD_ClearScreen(); LCD_DisplayString(1, " is Legend... wa");}
		if(counter1 == 22){LCD_ClearScreen(); LCD_DisplayString(1, "is Legend... wai");}
		if(counter1 == 23){LCD_ClearScreen(); LCD_DisplayString(1, "s Legend... wait");}		
		if(counter1 == 24){LCD_ClearScreen(); LCD_DisplayString(1, " Legend... wait ");}
		if(counter1 == 25){LCD_ClearScreen(); LCD_DisplayString(1, "Legend... wait f");}
		if(counter1 == 26){LCD_ClearScreen(); LCD_DisplayString(1, "egend... wait fo");}
		if(counter1 == 27){LCD_ClearScreen(); LCD_DisplayString(1, "gend... wait for");}
		if(counter1 == 28){LCD_ClearScreen(); LCD_DisplayString(1, "end... wait for ");}
		if(counter1 == 29){LCD_ClearScreen(); LCD_DisplayString(1, "nd... wait for i");}	
		if(counter1 == 30){LCD_ClearScreen(); LCD_DisplayString(1, "d... wait for it");}
		if(counter1 == 31){LCD_ClearScreen(); LCD_DisplayString(1, "... wait for it ");}
		if(counter1 == 32){LCD_ClearScreen(); LCD_DisplayString(1, ".. wait for it D");}
		if(counter1 == 33){LCD_ClearScreen(); LCD_DisplayString(1, ". wait for it Da");}
		if(counter1 == 34){LCD_ClearScreen(); LCD_DisplayString(1, " wait for it Dar");}
		if(counter1 == 35){LCD_ClearScreen(); LCD_DisplayString(1, "wait for it Dary");}
		if(counter1 == 36){LCD_ClearScreen(); LCD_DisplayString(1, "ait for it Dary!");}				
		if(counter1 == 37){LCD_ClearScreen(); LCD_DisplayString(1, "it for it Dary! ");}
		if(counter1 == 38){LCD_ClearScreen(); LCD_DisplayString(1, "t for it Dary!  ");}
		if(counter1 == 39){LCD_ClearScreen(); LCD_DisplayString(1, " for it Dary!   ");}
		if(counter1 == 40){LCD_ClearScreen(); LCD_DisplayString(1, "for it Dary!    ");}
		if(counter1 == 41){LCD_ClearScreen(); LCD_DisplayString(1, "or it Dary!     ");}
		if(counter1 == 42){LCD_ClearScreen(); LCD_DisplayString(1, "r it Dary!      ");}
		if(counter1 == 43){LCD_ClearScreen(); LCD_DisplayString(1, " it Dary!       ");}
		if(counter1 == 44){LCD_ClearScreen(); LCD_DisplayString(1, "it Dary!        ");}
		if(counter1 == 45){LCD_ClearScreen(); LCD_DisplayString(1, "t Dary!         ");}
		if(counter1 == 46){LCD_ClearScreen(); LCD_DisplayString(1, "t Dary!         ");}
		if(counter1 == 47){LCD_ClearScreen(); LCD_DisplayString(1, " Dary!          ");}			
		if(counter1 == 48){LCD_ClearScreen(); LCD_DisplayString(1, "Dary!           ");}
		if(counter1 == 49){LCD_ClearScreen(); LCD_DisplayString(1, "ary!            ");}
		if(counter1 == 50){LCD_ClearScreen(); LCD_DisplayString(1, "ry!             ");}
		if(counter1 == 51){LCD_ClearScreen(); LCD_DisplayString(1, "y!              ");}
		if(counter1 == 52){LCD_ClearScreen(); LCD_DisplayString(1, "!               ");}
		if(counter1 == 53){LCD_ClearScreen(); LCD_DisplayString(1, "                ");}
		if(counter1 == 53)
		{
			counter1 = 0x00;
		}	
		else
		{
			counter1 = counter1 + 1;
		}
		State = s0;
		break;
	}
	
	switch(State)
	{
		case s0:
		break;
		//default: PORTB = 0x1B; break; // Should never occur. Middle LED
	}
	return State;
}
int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00; // PORTB set to output, outputs init 0s
	DDRC = 0x00; PORTC = 0x00; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
	//unsigned long int SMTick1_calc = 100;
	DDRD = 0xFF; PORTD = 0x00;
	static task task1;
	task *tasks[] = { &task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	task1.state = -1;//Task initial state.
	task1.period = 100;//Task Period.
	task1.elapsedTime = 100;//Task current elapsed time.
	task1.TickFct = &GetKeypadKey;//Function pointer for the tick.
	
	LCD_init();
	TimerSet(7);
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
