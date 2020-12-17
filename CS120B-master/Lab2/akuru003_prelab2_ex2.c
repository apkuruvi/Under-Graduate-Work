/* Partner 1 Name & E-mail: Abraham Kuruvila akuru003@ucr.edu
* Partner 2 Name & E-mail: Rogelio Vazquez rvazq002@ucr.edu
* Lab Section: 22
* Assignment: Lab # Exercise #: Prelab 2 Exercise 2
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

// main for exercise 2

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs,
	
	unsigned char tmpB = 0x00; // intermediate variable used for port updates
	unsigned char holder = 0x00;
    /* Replace with your application code */
    while (1) 
    {
        holder = PINA << 4;
        holder = holder >> 4;
	if(holder <= 0x02)
		{
			tmpB = SetBit(tmpB,6,1);
			tmpB = SetBit(tmpB,5,1);
			tmpB = SetBit(tmpB,4,0);
			tmpB = SetBit(tmpB,3,0);
			tmpB = SetBit(tmpB,2,0);
			tmpB = SetBit(tmpB,1,0);
			tmpB = SetBit(tmpB,0,0);
			PORTC = tmpB;
		}
		else if(holder <= 0x04)
		{
			tmpB = SetBit(tmpB,6,1);
			tmpB = SetBit(tmpB,5,1);
			tmpB = SetBit(tmpB,4,1);
			tmpB = SetBit(tmpB,3,0);
			tmpB = SetBit(tmpB,2,0);
			tmpB = SetBit(tmpB,1,0);
			tmpB = SetBit(tmpB,0,0);
			PORTC = tmpB;
		}
		else if(holder <= 0x06)
		{
			tmpB = SetBit(tmpB,6,0);
			tmpB = SetBit(tmpB,5,1);
			tmpB = SetBit(tmpB,4,1);
			tmpB = SetBit(tmpB,3,1);
			tmpB = SetBit(tmpB,2,0);
			tmpB = SetBit(tmpB,1,0);
			tmpB = SetBit(tmpB,0,0);
			PORTC = tmpB;
		}
		else if(holder <= 0x09)
		{
			tmpB = SetBit(tmpB,6,0);
			tmpB = SetBit(tmpB,5,1);
			tmpB = SetBit(tmpB,4,1);
			tmpB = SetBit(tmpB,3,1);
			tmpB = SetBit(tmpB,2,1);
			tmpB = SetBit(tmpB,1,0);
			tmpB = SetBit(tmpB,0,0);
			PORTC = tmpB;
		}
		else if(holder <= 0x0C)
		{
			tmpB = SetBit(tmpB,6,0);
			tmpB = SetBit(tmpB,5,1);
			tmpB = SetBit(tmpB,4,1);
			tmpB = SetBit(tmpB,3,1);
			tmpB = SetBit(tmpB,2,1);
			tmpB = SetBit(tmpB,1,1);
			tmpB = SetBit(tmpB,0,0);
			PORTC = tmpB;
		}
		else if(holder <= 0x0F)
		{
			tmpB = SetBit(tmpB,6,0);
			tmpB = SetBit(tmpB,5,1);
			tmpB = SetBit(tmpB,4,1);
			tmpB = SetBit(tmpB,3,1);
			tmpB = SetBit(tmpB,2,1);
			tmpB = SetBit(tmpB,1,1);
			tmpB = SetBit(tmpB,0,1);
			PORTC = tmpB;
		}
    }
}




