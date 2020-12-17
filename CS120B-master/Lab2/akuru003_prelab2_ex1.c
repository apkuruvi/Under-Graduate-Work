/* Partner 1 Name & E-mail: Abraham Kuruvila akuru003@ucr.edu
* Partner 2 Name & E-mail: Rogelio Vazquez rvazq002@ucr.edu
* Lab Section: 22
* Assignment: Lab # Exercise #: Prelab2 Exercise 1
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/


// main function for exercise one 


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as inputs,
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs,
	
	unsigned char tmpB = 0x00; // intermediate variable used for port updates
    /* Replace with your application code */
    while (1) 
    {
		tmpB = 0x00;
		for(unsigned short i = 0; i < 0x08; i = i + 0x01)
		{
			if(GetBit(PINA,i))
			{
				tmpB = tmpB + 1;
				PORTC = tmpB;
			}
			if(GetBit(PINB,i))
			{
				tmpB = tmpB + 1;
				PORTC = tmpB;
			}
		}
		PORTC = tmpB;
		
		
		
    }
}







