/* Partner 1 Name & E-mail: Abraham Kuruvila akuru003@ucr.edu
* Partner 2 Name & E-mail: Rogelio Vazquez rvazq002@ucr.edu
* Lab Section: 22
* Assignment: Lab # Exercise #: Lab 1 Exercise 2
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char counter = 0x00;
    /* Replace with your application code */
    while (1) 
    {
		if((PINA & 0x01) == 0x01)
		{
			
		}
		else
		{
		    	counter = counter + 1;
		}
		if((PINA & 0x02) == 0x02)
		{
			
		}
		else
		{
		    	counter = counter + 1;
		}
		if((PINA & 0x04) == 0x04)
		{
			
		}
		else
		{
		    	counter = counter + 1;
		}
		if((PINA & 0x08) == 0x08)
		{
			
		}
		else
		{
		    	counter = counter + 1;
		}
		PORTC = counter;
		counter = 0x00;
    }
	return 0;
}

