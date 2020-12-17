/* Partner 1 Name & E-mail: Abraham Kuruvila akuru003@ucr.edu
* Partner 2 Name & E-mail: Rogelio Vazquez rvazq002@ucr.edu
* Lab Section: 22
* Assignment: Lab # Exercise #: Lab 2 Exercise 5
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>

int main(void)
{
	DDRD = 0x00; PORTD = 0xFF;
	DDRB = 0XFE; PORTB = 0x01; 

        unsigned char tmp = 0x00;
	unsigned short tmp2 = 0x0000;
	unsigned char tmp3 = 0x00;
	
    while(1)
    {
		 //TODO:: Please write your application code 
		tmp = 0x00;
		tmp2 = 0x0000;
		tmp3 = 0x01;
		tmp = PINB & 0x01;  // get BO val in tmp
		tmp2 = PIND;
		tmp2 = tmp2 << 1;
		tmp2 = tmp2 | tmp;
		
		if(tmp2 >= 0x0046)
		{
			tmp3 = tmp  | 0x02;
		}
		else if((tmp2 > 0x005) && (tmp2 < 0x0046))
		{
			
			tmp3 = tmp | 0x04;
		}
		PORTB = tmp3;
		
    }
}