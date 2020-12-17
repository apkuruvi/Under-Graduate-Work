/* Partner 1 Name & E-mail: Abraham Kuruvila akuru003@ucr.edu
* Partner 2 Name & E-mail: Rogelio Vazquez rvazq002@ucr.edu
* Lab Section: 22
* Assignment: Lab # Exercise #: Lab 1 Exercise 1
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	unsigned char tmp = 0x00;
    while(1)
    {
        //TODO:: Please write your application code 
		if(PINA  == 0x01)
		{
			 tmp = 0x01;
		}
		else
		{
			tmp = 0x00;
		}
		PORTB = tmp;
    }
	return 0;
}