/* Partner 1 Name & E-mail: Abraham Kuruvila akuru003@ucr.edu
* Partner 2 Name & E-mail: Rogelio Vazquez rvazq002@ucr.edu
* Lab Section: 22
* Assignment: Lab # Exercise #: Lab 2 Exercise 4
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
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs,
	
	unsigned char tmpB = 0x00; // intermediate variable used for port updates
	unsigned char tmpC = 0x00;
	unsigned char holder = ((PINA << 4) >> 4);
    /* Replace with your application code */
    while (1) 
    {
		holder = PINA;
		tmpB = SetBit(tmpB,0,GetBit(holder,4));
		tmpB = SetBit(tmpB,1,GetBit(holder,5));
		tmpB = SetBit(tmpB,2,GetBit(holder,6));
		tmpB = SetBit(tmpB,3,GetBit(holder,7));
		tmpC = SetBit(tmpC,4,GetBit(holder,0));
		tmpC = SetBit(tmpC,5,GetBit(holder,1));
		tmpC = SetBit(tmpC,6,GetBit(holder,2));
		tmpC = SetBit(tmpC,7,GetBit(holder,3));
		PORTB = tmpB;
	    PORTC = tmpC;
    }
}






