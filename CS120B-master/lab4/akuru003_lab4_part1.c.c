/* Partner 1 Name & E-mail: Abraham Kuruvila akuru003@ucr.edu
* Partner 2 Name & E-mail: Rogelio Vazquez rvazq002@ucr.edu
* Lab Section: 22
* Assignment: Lab # Exercise #: Lab 4 Exercise 1
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


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs,
	
	unsigned char tmpB = 0x00; // intermediate variable used for port updates
	unsigned char holder = 0x00;
	/* Replace with your application code */
	while (1)
	{
		
		holder = ~PINA;
		holder = holder << 4;
		holder = holder >> 4;
		
		
		if(holder <= 0x02)
		{
			tmpB = ((tmpB & 0x00) | 0x60);
			PORTC = tmpB;
		}
		else if( (~PINA & 0x0F)  <= 0x04)
		{
			PORTC = 0x70;
		}
		else if(holder <= 0x06)
		{
			tmpB = ((tmpB & 0x00) | 0x38);
			PORTC = tmpB;
		}
		else if(holder <= 0x09)
		{
			tmpB = ((tmpB & 0x00) | 0x3C);
			PORTC = tmpB;
		}
		else if(holder <= 0x0C)
		{
			tmpB = ((tmpB & 0x00) | 0x3E);
			PORTC = tmpB;
		}
		else if(holder <= 0x0F)
		{
			tmpB = ((tmpB & 0x00) | 0x3F);
			PORTC = tmpB;
		}
	}
}





