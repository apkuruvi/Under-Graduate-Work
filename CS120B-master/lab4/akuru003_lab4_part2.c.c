/* Partner 1 Name & E-mail: Abraham Kuruvila akuru003@ucr.edu
* Partner 2 Name & E-mail: Rogelio Vazquez rvazq002@ucr.edu
* Lab Section: 22
* Assignment: Lab # Exercise #: Lab 4 Exercise 2
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

enum States { Init, s0, s1, s2, s3, s4, s5, s6} State;

void Tick()
{
	//unsigned char temp1 = 0x00;
	unsigned char temp = ~PINA;
	unsigned char t1 = PINC;
	unsigned char t = PINC;
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
		if(GetBit(temp,1) == 1)
		{
			State = s2;
		}
		else
		{
			State = s3;
		}
		break;
		
		
		case s1:
		if((GetBit(temp, 0) == 1) && (GetBit(temp, 1) == 1))
		{
			State = s2;
		}
		if(GetBit(temp,0) == 1)
		{
			State = s2;
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
			State = Init;
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
		break;
		
		case s0:
		if(t < 0x09)
		{
			t = t + 1;
			PORTC = t;
		}
		break;
		
		case s1:
		if(t1 > 0x00)
		{
			t = t - 1;
			PORTC = t;
		}
		break;
		
		case s2:
		PORTC = 0x00;
		break;

		case s3:
		break;
		
		case s4:
		break;
		
		case s5:
		break;
		
		case s6:
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