/* Partner 1 Name & E-mail: Abraham Kuruvila akuru003@ucr.edu
* Partner 2 Name & E-mail: Rogelio Vazquez rvazq002@ucr.edu
* Lab Section: 22
* Assignment: Lab # Exercise #: Lab 3 Exercise 3
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>


// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

enum States { Start, Init, s0, s1, s2, s3, s4, s5} State;

void Tick()
{
	unsigned char temp = PINA;
	unsigned char start1 = 0x00;
	unsigned char one = 0x01;
	switch(State)
	{
		temp = PINA;
		case Start:
		State = Init;
		break;
		// Transitions
		case Init:                     // Initial transition
		if(temp == 0x80)
		{
			State = s0;
		}
		if(temp == 0x04)
		{
			State = s2;
		}
		break;

		case s0:   // do something here
		State = s1;
		break;
		
		case s1:   // do something here
		if(GetBit(temp,7) == 1)
		{
			State = s1;
		}
		else
		{
			State = Init;
		}
		break;
		
		case s2:
		State = s3;
		break;
		
		case s3:
		if(temp == 0x04)
		{
			State = s3;
		}
		else
		{
			State = s4;
		}
		break;
		
		case s4:
		if(temp == 0x02)
		{
			State = s5;
		}
		else if(temp == 0x04)   // remove if you dont need to repeat ##
		{
			State = s3;
		}
		else
		{
			State = Init;
		}
		break;
		
		case s5:
		if(temp == 0x02)
		{
			State = s5;
		}
		else
		{
			State = Init;
		}
		break;
		
		default:
		State = Init;
		break;
	} // Transitions

	switch(State)
	{   // State actions
		case Start:
		PORTB = start1;  // 0x00
		PORTC = Start;
		break;
		
		case Init:
		PORTC = Init;
		break;
		
		case s0:
		PORTB = start1;
		PORTC = s0;
		break;
		
		case s1:
		PORTC = s1;
		break;
		
		case s2:
		PORTC = s2;
		break;
		
		case s3:
		PORTC = s3;
		break;
		
		case s4:
		PORTC = s4;
		break;
		
		case s5:
		PORTB = one;
		PORTC = s5;
		break;
		
		default:
		PORTB = start1;
		PORTC = Start;
		break;
	} // State actions
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	
	unsigned char tmpB = 0x00; // intermediate variable used for port updates
	/* Replace with your application code */
	PORTB = tmpB;
	PORTC = Start;
	State = Start;
	while (1)
	{
		Tick();
	}
}
