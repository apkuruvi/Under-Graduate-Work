/* Partner 1 Name & E-mail: Abraham Kuruvila akuru003@ucr.edu
* Partner 2 Name & E-mail: Rogelio Vazquez rvazq002@ucr.edu
* Lab Section: 22
* Assignment: Lab # Exercise #: Lab 10 Prelab
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/


/// PINOUTS -STARTING AT d, 1-c3,2 - c2,3-c1, 4 - c0,5-c8,6-c7,7-c6,8 -c5
/////
#include <avr/io.h>
#include <avr/interrupt.h>


volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn()
{
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff()
{
	TCCR1B = 0x00;
}

// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

void TimerISR()
{
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect)
{
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0)
	{
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
		
	}
}

void TimerSet(unsigned long M)
{
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}







// Returns '\0' if no key pressed, else returns char '1', '2', ... '9', 'A', ...
// If multiple keys pressed, returns leftmost-topmost one
// Keypad must be connected to port C
/* Keypad arrangement
 PC4 PC5 PC6 PC7
 col 1 2 3 4
row
PC0 1 1 | 2 | 3 | A
PC1 2 4 | 5 | 6 | B
PC2 3 7 | 8 | 9 | C
PC3 4 * | 0 | # | D
*/
unsigned char GetKeypadKey() {
PORTC = 0xEF; // Enable col 4 with 0, disable others with 1’s
asm("nop"); // add a delay to allow PORTC to stabilize before checking
if (GetBit(PINC,0)==0) { return('1'); }
if (GetBit(PINC,1)==0) { return('4'); }
if (GetBit(PINC,2)==0) { return('7'); }
if (GetBit(PINC,3)==0) { return('*'); }
// Check keys in col 2
PORTC = 0xDF; // Enable col 5 with 0, disable others with 1’s
asm("nop"); // add a delay to allow PORTC to stabilize before checking
if (GetBit(PINC,0)==0) { return('2'); }
if (GetBit(PINC,1)==0) { return('5'); }
if (GetBit(PINC,2)==0) { return('8'); }
if (GetBit(PINC,3)==0) { return('0'); }	
// ... *****FINISH*****
// Check keys in col 3
PORTC = 0xBF; // Enable col 6 with 0, disable others with 1’s
asm("nop"); // add a delay to allow PORTC to stabilize before checking
if (GetBit(PINC,0)==0) { return('3'); }
if (GetBit(PINC,1)==0) { return('6'); }
if (GetBit(PINC,2)==0) { return('9'); }
if (GetBit(PINC,3)==0) { return('#'); }			
// ... *****FINISH*****
// Check keys in col 4
PORTC = 0x7F; // Enable col 6 with 0, disable others with 1’s
asm("nop"); // add a delay to allow PORTC to stabilize before checking
if (GetBit(PINC,0)==0) { return('A'); }
if (GetBit(PINC,1)==0) { return('B'); }
if (GetBit(PINC,2)==0) { return('C'); }
if (GetBit(PINC,3)==0) { return('D'); }
// ... *****FINISH*****
return('\0'); // default value
}
int main(void)
{
unsigned char x;
DDRB = 0xFF; PORTB = 0x00; // PORTB set to output, outputs init 0s
DDRC = 0xF0; PORTC = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
while(1) {
x = GetKeypadKey();
switch (x) {
case '\0': PORTB = 0x1F; break; // All 5 LEDs on
case '1': PORTB = 0x01; break; // hex equivalent
case '2': PORTB = 0x02; break;
case '3': PORTB = 0x03; break; // All 5 LEDs on
case '4': PORTB = 0x04; break; // hex equivalent
case '5': PORTB = 0x05; break;
case '6': PORTB = 0x06; break; // All 5 LEDs on
case '7': PORTB = 0x07; break; // hex equivalent
case '8': PORTB = 0x08; break;
case '9': PORTB = 0x09; break; // All 5 LEDs on
case 'A': PORTB = 0x0A; break; // hex equivalent
case 'B': PORTB = 0x0B; break;
case 'C': PORTB = 0x0C; break; 
// . . . ***** FINISH *****
case 'D': PORTB = 0x0D; break;
case '*': PORTB = 0x0E; break;
case '0': PORTB = 0x00; break;
case '#': PORTB = 0x0F; break;
default: PORTB = 0x1B; break; // Should never occur. Middle LED
//off.
}
}
}





