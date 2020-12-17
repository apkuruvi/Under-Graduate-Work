/* Partner 1 Name & E-mail: Abraham Kuruvila akuru003@ucr.edu
* Partner 2 Name & E-mail: Rogelio Vazquez rvazq002@ucr.edu
* Lab Section: 22
* Assignment: Lab # Exercise #: Lab 1 Exercise 4
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ?  (x | (0x01 << k))  :  (x & ~(0x01 << k)) );
	//   Set bit to 1           Set bit to 0
}

unsigned char GetBit(unsigned short x, unsigned char k) {
	return ((x & (0x0001 << k)) != 0);
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x00; PORTA = 0xFF;
	DDRC = 0x00; PORTA = 0xFF;
	
	DDRD = 0xFF; PORTD = 0x00;
	
	unsigned char tmp = 0x00;
	unsigned short totalWeight = 0x00;
	
	
    while(1)
    {
        //TODO:: Please write your application code 
		totalWeight = PINA + PINB + PINC;
		
		//clear temp
		tmp = 0x00;
		
		//A, B, & C Exceeds weight
		if (totalWeight > 0x00BC){
			tmp = tmp | 0x01;
			PORTD = tmp;
		}
		//refresh
		else {
			tmp = tmp & 0xFE;
			PORTD = tmp;
		}
		
		//Port A & C Difference
		if ( PINA - PINC >  0x50) {
			tmp = tmp | 0x02;
			PORTD = tmp;
		}
		else if (PINC - PINA > 0x50){
			tmp = tmp | 0x02;
			PORTD = tmp;
		}
		else {
			tmp = tmp & 0xFD;
			PORTD = tmp;
		}
		
		//approx weight
		
			tmp = SetBit(tmp, 2, GetBit(totalWeight, 0));
			tmp = SetBit(tmp, 3, GetBit(totalWeight, 1));
			tmp = SetBit(tmp, 4, GetBit(totalWeight, 2));
			tmp = SetBit(tmp, 5, GetBit(totalWeight, 3));
			tmp = SetBit(tmp, 6, GetBit(totalWeight, 4));
			tmp = SetBit(tmp, 7, GetBit(totalWeight, 5));
			
			PORTD = tmp;
		
    }
}