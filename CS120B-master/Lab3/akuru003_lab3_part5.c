/* Partner 1 Name & E-mail: Abraham Kuruvila akuru003@ucr.edu
* Partner 2 Name & E-mail: Rogelio Vazquez rvazq002@ucr.edu
* Lab Section: 22
* Assignment: Lab # Exercise #: Lab 3 Exercise 5
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

enum States { Start, s0, s1, s2} State;

unsigned char a[4] = {0x04, 0x01, 0x02, 0x01};
unsigned char a1[4] = {0x00, 0x00, 0x00, 0x00};
unsigned char counter = 0x00;

void Tick()
{
	unsigned char temp = PINA;
	unsigned char start1 = 0x00;
	unsigned char one = 0x01;
	switch(State)
	{
		case Start:
		if((PINA == 0x04) || (PINA == 0x02) || (PINA == 0x01) || (PINA == 0x80))
		{
			State = s0;
		}
		else
		{
			State = Start;
		}
		break;
		
		case s0:
		if((PINA == 0x04) || (PINA == 0x02) || (PINA == 0x01))
		{
			a1[counter] = PINA;
			State = s1;
			counter = counter + 1;
		}
		else if (PINA == 0x80)
		{
			State = s2;
			PORTB = start1;
		}
		break;
		
		case s1:
		if((PINA == 0x04) || (PINA == 0x02) || (PINA == 0x01))
		{
			State = s1;
		}
		else
		{
			if(counter == 0x04)
			{
				if((a[0] == a[0]) && (a[1] == a[1]) && (a[2] == a[2]) && (a[3] == a[3]))
				{
					if(PORTB == 0x01)
					{
						PORTB = start1;
					}
					else
					{
						PORTB = one;
					}
				}
				counter = 0x00;
			}
			State = Start;
		}
		break;
		
		case s2:
		if(PINA == 0x80)
		{
			State = s2;
		}
		else
		{
			State = Start;
		}
		break;
		
		
		
		default:
		State = Start;
		break;
	} // Transitions

	switch(State)
	{   // State actions
		case Start:
		PORTC = Start;
		break;
		
		case s0:
		PORTC = s0;
		break;
		
		case s1:
		PORTC = s1;
		break;
		
		case s2:
		PORTC = s2;
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
	PORTC = Start;
	State = Start;
	while (1)
	{
		Tick();
	}
}
