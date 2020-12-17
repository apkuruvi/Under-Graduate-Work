/* Partner 1 Name & E-mail: Abraham Kuruvila akuru003@ucr.edu
* Partner 2 Name & E-mail: Rogelio Vazquez rvazq002@ucr.edu
* Lab Section: 22
* Assignment: Lab # Exercise #: Lab 4 Exercise 3
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

enum States { Init, s0, s1, s2, s3, s4, s5, s6, s7} State;

void Tick()
{
	//unsigned char temp1 = 0x00;
	unsigned char temp = ~PINA;
	unsigned char t1 = PINC;
	unsigned char t = PINC;
	switch(State)
	{                                   // Transitions
		case Init:                     // Initial transition
		if(temp == 0x01)
		{
			State = s0;
		}
		else
		{
			State = Init;
		}
		break;
		
		case s0:         // light up first one 
		State = s1;
		break;
		
		
		case s1:
		if(GetBit(temp,0) == 1)
		{
			State = s1;                     // wait for release
		}
		else
		{
			State = s2;
		}
		break;
		
		case s2:
		if(GetBit(temp,0) == 1)
		{
			State = s3;
		}
		else
		{
			State = s2;
		}
		break;
		
		case s3:
		if(GetBit(temp,0) == 1)
		{
			State = s3;
		}
		else
		{
			State = s4;
		}
		break;
		
		case s4:
		if(GetBit(temp,0) == 1)
		{
			State = s5;
		}
		else
		{
			State = s4;
		}
		break;
		
		case s5:
		if(GetBit(temp,0) == 1)
		{
			State = s5;
		}
		else
		{
			State = s6;
		}
		break;
		
		case s6:
		if(GetBit(temp,0) == 1)
		{
			State = s7;
		}
		else
		{
			State = s6;
		}
		break;
		
		case s7:
		if(GetBit(temp,0) == 0)
		{
			State = Init;
		}
		else
		{
			State = s7;
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
		PORTC = 0x03;
		break;
		
		case s1:
		break;
		
		case s2:
		break;

		case s3:
		PORTC = 0x0F;
		break;
		
		case s4:
		break;
		
		case s5:
		PORTC = 0x3F;
		break;
		
		case s6:
		break;
		
		case s7:
		PORTC = 0x00;
		break;
		
		default:
		PORTC = 0x00;
		break;
	} // State actions
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00;
	
	unsigned char tmpB = 0x00; // intermediate variable used for port updates
	/* Replace with your application code */
	PORTC = tmpB;
	State = Init;
	while (1)
	{
		Tick();
	}
}