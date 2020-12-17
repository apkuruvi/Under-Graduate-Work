/* Partner 1 Name & E-mail: Abraham Kuruvila akuru003@ucr.edu
* Partner 2 Name & E-mail: Rogelio Vazquez rvazq002@ucr.edu
* Lab Section: 22
* Assignment: Lab # Exercise #: Project
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/


#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
int counter900 = 0;
volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char tmp = 0x00;
int seed = 0;
unsigned char ground = 0xFF;
unsigned char blue = 0x00;
unsigned char green = 0x00;
unsigned char red = 0x00;
unsigned char violet = 0x00;
int rowStart = 0;
int groundStart = 0x00;
int ar[8][8]; // 1-blue, 2-red, 3-green
int gamewon = 0; // see if game is done
int piecemovement = 0; // see if we're done moving piece;
int piecenum = 0;
int rowsave1, piece1,piece2, piece3, piece4 = 0;
int rowsave1pos = 0;
int rowclearcheck = 1;
int rowclearcheck2 = 1;
int score = 0;
int columnclearcheck = 10;
int columnclearcheck2 = 10;
int temp90 = 100;
int temp91 = -1;
int temp92 = -1;
int temp80 = 0;
int check = 0;
int check2 = 0;
int pause = 0;
int checkcounter = 0;
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

void setGround()
{
	if(groundStart == 0x7F || rowStart == 7)
	{
		groundStart = 0xFE;
		rowStart = 0;
	}
	else
	{
		groundStart = (groundStart << 1) | 0x01;		
		rowStart++;
	}
	blue = red = green = 0x00;
	
	for( int b = 0; b < 8; b++)
	{
		if(ar[rowStart][b] == 1) blue |= 0x80;
		if(ar[rowStart][b] == 2) red |= 0x80;
		if(ar[rowStart][b] == 3) green |= 0x80;
		if(ar[rowStart][b] == 4){ red |= 0x80;  blue |= 0x80; }
		if(ar[rowStart][b] == 5){ red |= 0x80;  green |= 0x80; }	
		if(ar[rowStart][b] == 6){ blue |= 0x80;  green |= 0x80; }	
		if( b < 7 )
		{
			blue = blue >> 1;
			red = red >> 1;
			green = green >> 1;
		}
	}
	for(int i = 7; i >= 0; i--)
	{
		PORTC = 0x88;
		PORTD = 0x88;
		PORTC |= ((groundStart >> i) & 0x01);
		PORTC |= (((blue >> i) << 4) & 0x10);
		PORTD |= ((green >> i) & 0x01);
		PORTD |= (((red >> i) << 4) & 0x10);
		PORTC |= 0x44;
		PORTD |= 0x44;
	}
	PORTC |= 0x22;
	PORTD |= 0x22;
	PORTC = 0x00;
	PORTD = 0x00;
}

void InitADC()
{
	ADMUX=(1<<REFS0);   
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}
void convert(){
	ADCSRA |=(1<<ADSC);//start ADC conversion
	while ( !(ADCSRA & (1<<ADIF)));//wait till ADC conversion
	
}
enum states {start, wait, x_axis, y_axis} state;
unsigned short temp1,movx,movy, temp2;
unsigned char left,right,up,down = 0x00;
void tick()
{
	switch(state)
	{
		case start:
		state = wait;
		break;
		
		case wait:
		temp2 = 0x00;
		temp1 = ADMUX;
		if (ADMUX ==0x40)
		{
			state = x_axis;
		}
		else if (ADMUX ==0x41)
		{
			state = y_axis;
		}
		
		else{state = wait;}
		break;
		
		case x_axis:
		state = wait;
		break;
		
		case y_axis:
		state = wait;
		break;
		
		
	}
	switch(state)
	{
		case start:
		break;
		
		case wait:
		break;
		
		case x_axis:
		convert();
		movx =ADC;
		if (movx > 612)
		{
			temp2= (0x08);
			right++;
			
		}
		else if (movx <412)
		{
			temp2= (0x04);
			left++;
		}
		ADMUX = 0x41;// initialize value of ADMUX to switch over to next pin
		break;
		
		case y_axis:
		convert();
		movy =ADC;
		if (movy > 612)
		{
			temp2= (0x02);
			up++;
		}
		else if (movy <412)
		{
			temp2= (0x01);
			down++;
			
		}
		ADMUX = 0x40;
		break;
	}
	//PORTB = temp2;
}


enum States2 { Init2, s32, s42, s52, s62,s72, s82, s92} State2;

void Tick2() // move left right or down
{
	switch(State2){                                   // Transitions
		case Init2:
		if(piecemovement == 1)
		{
			if(piecenum == 1)
			State2 = s32;
			else if(piecenum == 2)
			{
				State2 = s42;
			}
			else if(piecenum == 3)
			{
				State2 = s52;
			}
			else if(piecenum == 4)
			{
				State2 = s62;
			}
			else if(piecenum == 5)
			{
				State2 = s72;
			}
			else if(piecenum == 6)
			{
				State2 = s82;
			}
			else if(piecenum == 7)
			{
				State2 = s92;
			}
		}
		break;
		
		case s32:
		if(temp2 == 8) // move left
		{
			if(rowsave1pos == 0) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 0 && rowsave1 <= 6)
				{
				    if(piece1 - 1 >= 0)
				    {
				      	if(ar[rowsave1][piece1 - 1] == 0)
    					{
    						ar[rowsave1][piece1] = 0;
    						ar[rowsave1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1][piece1] = 1;
    						ar[rowsave1][piece2] = 2;
    						ar[rowsave1][piece3] = 3;
    						ar[rowsave1][piece4] = 4;
    						setGround();
    						rowsave1pos = 0;
    					}  
				    }
				}
			}
			else if(rowsave1pos == 1) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece1 - 1 >= 0)
			    {
			     	if(rowsave1 >= 1 && rowsave1 <= 5)
    				{
    					if(ar[rowsave1 - 1][piece1 - 1] == 0 && ar[rowsave1][piece2 - 1] == 0 &&
    					 ar[rowsave1 + 1][piece2 - 1] == 0 && ar[rowsave1 + 2][piece2 - 1] == 0)
    					{
    						ar[rowsave1 - 1][piece1] = 0;
    						ar[rowsave1][piece2] = 0;
    						ar[rowsave1 + 1][piece3] = 0;
    						ar[rowsave1 + 2][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1 - 1][piece1] = 1;
    						ar[rowsave1][piece2] = 2;
    						ar[rowsave1 + 1][piece3] = 3;
    						ar[rowsave1 + 2][piece4] = 4;
    						setGround();
    						rowsave1pos = 1;
    					}
    				}   
			    }
			}
			else if(rowsave1pos == 2) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece4 - 1 >= 0)
			    {
			        if(rowsave1 >= 0 && rowsave1 <= 6)
    				{
    					if(ar[rowsave1][piece4 - 1] == 0)
    					{
    						ar[rowsave1][piece1] = 0;
    						ar[rowsave1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1][piece1] = 1;
    						ar[rowsave1][piece2] = 2;
    						ar[rowsave1][piece3] = 3;
    						ar[rowsave1][piece4] = 4;
    						setGround();
    						rowsave1pos = 2;
    					}
    				}
			    }
			}
			else if(rowsave1pos == 3) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece4 - 1 >= 0)
			    {
			      	if(rowsave1 >= 2 && rowsave1 <= 5)
    				{
    					if(ar[rowsave1 - 2][piece4 - 1] == 0 && ar[rowsave1 - 1][piece3 - 1] == 0 &&
    					 ar[rowsave1][piece2 - 1] == 0 && ar[rowsave1 + 1][piece1 - 1] == 0)
    					{
    						ar[rowsave1 + 1][piece1] = 0;
    						ar[rowsave1][piece2] = 0;
    						ar[rowsave1 - 1][piece3] = 0;
    						ar[rowsave1 - 2][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1 + 1][piece1] = 1;
    						ar[rowsave1][piece2] = 1;
    						ar[rowsave1 - 1][piece3] = 3;
    						ar[rowsave1 - 2][piece4] = 4;
    						setGround();
    						rowsave1pos = 3;
    					}
    				}  
			    }
			}
		}
		if(temp2 == 4) // move right
		{
			if(rowsave1pos == 0) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 0 && rowsave1 <= 6)
				{
				    if(piece4 + 1 <= 7)
				    {
				      	if(ar[rowsave1][piece4 + 1] == 0)
    					{
    						ar[rowsave1][piece1] = 0;
    						ar[rowsave1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1][piece1] = 1;
    						ar[rowsave1][piece2] = 2;
    						ar[rowsave1][piece3] = 3;
    						ar[rowsave1][piece4] = 4;
    						setGround();
    						rowsave1pos = 0;
    					}  
				    }
				}
			}
			else if(rowsave1pos == 1) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece4 + 1 <= 7)
			    {
			     	if(rowsave1 >= 1 && rowsave1 <= 5)
    				{
    					if(ar[rowsave1 - 1][piece1 + 1] == 0 && ar[rowsave1][piece2 + 1] == 0 &&
    					 ar[rowsave1 + 1][piece2 + 1] == 0 && ar[rowsave1 + 2][piece2 + 1] == 0)
    					{
    						ar[rowsave1 - 1][piece1] = 0;
    						ar[rowsave1][piece2] = 0;
    						ar[rowsave1 + 1][piece3] = 0;
    						ar[rowsave1 + 2][piece4] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1 - 1][piece1] = 1;
    						ar[rowsave1][piece2] = 2;
    						ar[rowsave1 + 1][piece3] = 3;
    						ar[rowsave1 + 2][piece4] = 4;
    						setGround();
    						rowsave1pos = 1;
    					}
    				}   
			    }
			}
			else if(rowsave1pos == 2) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece1 + 1 <= 7)
			    {
			        if(rowsave1 >= 0 && rowsave1 <= 6)
    				{
    					if(ar[rowsave1][piece1 + 1] == 0)
    					{
    						ar[rowsave1][piece1] = 0;
    						ar[rowsave1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1][piece1] = 1;
    						ar[rowsave1][piece2] = 2;
    						ar[rowsave1][piece3] = 3;
    						ar[rowsave1][piece4] = 4;
    						setGround();
    						rowsave1pos = 2;
    					}
    				}
			    }
			}
			else if(rowsave1pos == 3) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece4 + 1 <= 7)
			    {
			      	if(rowsave1 >= 2 && rowsave1 <= 5)
    				{
    					if(ar[rowsave1 - 2][piece4 + 1] == 0 && ar[rowsave1 - 1][piece3 + 1] == 0 &&
    					 ar[rowsave1][piece2 + 1] == 0 && ar[rowsave1 + 1][piece1 + 1] == 0)
    					{
    						ar[rowsave1 + 1][piece1] = 0;
    						ar[rowsave1][piece2] = 0;
    						ar[rowsave1 - 1][piece3] = 0;
    						ar[rowsave1 - 2][piece4] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1 + 1][piece1] = 1;
    						ar[rowsave1][piece2] = 1;
    						ar[rowsave1 - 1][piece3] = 3;
    						ar[rowsave1 - 2][piece4] = 4;
    						setGround();
    						rowsave1pos = 3;
    					}
    				}  
			    }
			}
		}
		if(temp2 == 2) // move down
		{
	    	if(rowsave1pos == 0)
    		{
    			if(rowsave1 != 7)
    			{
    				if(ar[rowsave1 + 1][piece1] == 0 && ar[rowsave1 + 1][piece2] == 0 && ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1 + 1][piece4] == 0)
    				{
    					ar[rowsave1][piece1] = 0;
    					ar[rowsave1][piece2] = 0;
    					ar[rowsave1][piece3] = 0;
    					ar[rowsave1][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1][piece1] = 1;
    					ar[rowsave1][piece2] = 2;
    					ar[rowsave1][piece3] = 3;
    					ar[rowsave1][piece4] = 4;
    				}
    			}
    		}
    		else if(rowsave1pos == 1)
    		{
    			if(rowsave1 <= 4 && rowsave1 >= 1)
    			{
    				if(ar[rowsave1 + 3][piece2] == 0)
    				{
    					ar[rowsave1 - 1][piece1] = 0;
    					ar[rowsave1][piece2] = 0;
    					ar[rowsave1 + 1][piece3] = 0;
    					ar[rowsave1 + 2][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1 - 1][piece1] = 1;
    					ar[rowsave1][piece2] = 2;
    					ar[rowsave1 + 1][piece3] = 3;
    					ar[rowsave1 + 2][piece4] = 4;
    				}
    			}
    		}
    		else if(rowsave1pos == 2)
    		{
    			if(rowsave1 != 7)
    			{
    				if(ar[rowsave1 + 1][piece1] == 0 && ar[rowsave1 + 1][piece2] == 0 && ar[rowsave1 + 1][piece3] == 0 && 
    				ar[rowsave1 + 1][piece4] == 0)
    				{
    					ar[rowsave1][piece1] = 0;
    					ar[rowsave1][piece2] = 0;
    					ar[rowsave1][piece3] = 0;
    					ar[rowsave1][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1][piece1] = 1;
    					ar[rowsave1][piece2] = 2;
    					ar[rowsave1][piece3] = 3;
    					ar[rowsave1][piece4] = 4;
    				}
    			}
    		}
    		else if(rowsave1pos == 3)
    		{
    			if(rowsave1 <= 5 && rowsave1 >= 2)
    			{
    				if(ar[rowsave1 + 2][piece2] == 0)
    				{
    					ar[rowsave1 + 1][piece1] = 0;
    					ar[rowsave1][piece2] = 0;
    					ar[rowsave1 - 1][piece3] = 0;
    					ar[rowsave1 - 2][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1 + 1][piece1] = 1;
    					ar[rowsave1][piece2] = 2;
    					ar[rowsave1 - 1][piece3] = 3;
    					ar[rowsave1 - 2][piece4] = 4;
    				}
    			}
    		}
		}
		if(piecemovement == 1)
		{
			State2 = s32;
		}
		else
		State2 = Init2;
		break;
		
		case s42:
		if(temp2 == 8) // move left
		{
			if(rowsave1pos == 0) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 1 && rowsave1 <= 7)
				{
				    if(piece1 - 1 >= 0)
				    {
				      	if((ar[rowsave1 - 1][piece1 - 1] == 0) && (ar[rowsave1][piece3 - 1] == 0))
    					{
    						ar[rowsave1 - 1][piece1] = 0;
    						ar[rowsave1 - 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1 - 1][piece1] = 3;
    						ar[rowsave1 - 1][piece2] = 1;
    						ar[rowsave1][piece3] = 5;
    						ar[rowsave1][piece4] = 6;
    						setGround();
    						rowsave1pos = 0;
    					}  
				    }
				}
			}
			else if(rowsave1pos == 1) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece3 - 1 >= 0)
			    {
			     	if(rowsave1 >= 2 && rowsave1 <= 6)
    				{
    					if(ar[rowsave1][piece3 - 1] == 0 && ar[rowsave1 + 1][piece4 - 1] == 0 &&
    					 ar[rowsave1 - 1][piece2 - 1] == 0 && ar[rowsave1 - 2][piece1 - 1] == 0)
    					{
    						ar[rowsave1 - 2][piece1] = 0;
    						ar[rowsave1 - 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1 + 1][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1 - 2][piece1] = 3;
    						ar[rowsave1 - 1][piece2] = 1;
    						ar[rowsave1][piece3] = 5;
    						ar[rowsave1 + 1][piece4] = 6;
    						setGround();
    						rowsave1pos = 1;
    					}
    				}   
			    }
			}
			else if(rowsave1pos == 2) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece4 - 1 >= 0)
			    {
			        if(rowsave1 >= 0 && rowsave1 <= 6)
    				{
    					if((ar[rowsave1][piece4 - 1] == 0) && (ar[rowsave1 + 1][piece2 - 1] == 0))
    					{
    						ar[rowsave1 + 1][piece1] = 0;
    						ar[rowsave1 + 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1 + 1][piece1] = 3;
    						ar[rowsave1 + 1][piece2] = 1;
    						ar[rowsave1][piece3] = 5;
    						ar[rowsave1][piece4] = 6;
    						setGround();
    						rowsave1pos = 2;
    					}
    				}
			    }
			}
			else if(rowsave1pos == 3) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece1 - 1 >= 0)
			    {
			      	if(rowsave1 >= 1 && rowsave1 <= 5)
    				{
    					if(ar[rowsave1][piece3 - 1] == 0 && ar[rowsave1 - 1][piece4 - 1] == 0 &&
    					 ar[rowsave1 + 1][piece2 - 1] == 0 && ar[rowsave1 + 2][piece1 - 1] == 0)
    					{
    						ar[rowsave1 + 2][piece1] = 0;
    						ar[rowsave1 + 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1 - 1][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1 + 2][piece1] = 3;
    						ar[rowsave1 + 1][piece2] = 1;
    						ar[rowsave1][piece3] = 5;
    						ar[rowsave1 - 1][piece4] = 6;
    						setGround();
    						rowsave1pos = 3;
    					}
    				}  
			    }
			}
		}
		if(temp2 == 4) // move right
		{
			if(rowsave1pos == 0) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 1 && rowsave1 <= 6)  // row check
				{
				    if((piece2 + 1 <= 7) && (piece4 + 1 <= 7))  // piece bundary check
				    {
				      	if((ar[rowsave1][piece4 + 1] == 0) && (ar[rowsave1 - 1][piece2 + 1] == 0))   // array check
    					{
    						ar[rowsave1 - 1][piece1] = 0;
    						ar[rowsave1 - 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1 - 1][piece1] = 3;
    						ar[rowsave1 - 1][piece2] = 1;
    						ar[rowsave1][piece3] = 5;
    						ar[rowsave1][piece4] = 6;
    						setGround();
    						rowsave1pos = 0;
    					}  
				    }
				}
			}
			else if(rowsave1pos == 1) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece1 + 1 <= 7)
			    {
			     	if(rowsave1 >= 2 && rowsave1 <= 5)
    				{
    					if(ar[rowsave1 - 2][piece1 + 1] == 0 && ar[rowsave1 - 1][piece2 + 1] == 0 &&
    					 ar[rowsave1][piece3 + 1] == 0 && ar[rowsave1 + 1][piece4 + 1] == 0)
    					{
    						ar[rowsave1 - 2][piece1] = 0;
    						ar[rowsave1 - 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1 + 1][piece4] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1 - 2][piece1] = 3;
    						ar[rowsave1 - 1][piece2] = 1;
    						ar[rowsave1][piece3] = 5;
    						ar[rowsave1 + 1][piece4] = 6;
    						setGround();
    						rowsave1pos = 1;
    					}
    				}   
			    }
			}
			else if(rowsave1pos == 2) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece1 + 1 <= 7)
			    {
			        if(rowsave1 >= 0 && rowsave1 <= 5)
    				{
    					if((ar[rowsave1][piece3 + 1] == 0)  && (ar[rowsave1 + 1][piece1 + 1] == 0))
    					{
    						ar[rowsave1 + 1][piece1] = 0;
    						ar[rowsave1 + 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1 + 1][piece1] = 3;
    						ar[rowsave1 + 1][piece2] = 1;
    						ar[rowsave1][piece3] = 5;
    						ar[rowsave1][piece4] = 6;
    						setGround();
    						rowsave1pos = 2;
    					}
    				}
			    }
			}
			else if(rowsave1pos == 3) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece4 + 1 <= 7)
			    {
			      	if(rowsave1 >= 1 && rowsave1 <= 4)
    				{
    					if(ar[rowsave1 - 1][piece4 + 1] == 0 && ar[rowsave1][piece3 + 1] == 0 &&
    					 ar[rowsave1 + 1][piece2 + 1] == 0 && ar[rowsave1 + 2][piece1 + 1] == 0)
    					{
    						ar[rowsave1 + 2][piece1] = 0;
    						ar[rowsave1 + 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1 - 1][piece4] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1 + 2][piece1] = 3;
    						ar[rowsave1 + 1][piece2] = 1;
    						ar[rowsave1][piece3] = 5;
    						ar[rowsave1 - 1][piece4] = 6;
    						setGround();
    						rowsave1pos = 3;
    					}
    				}  
			    }
			}
		}
		if(temp2 == 2) // move down
		{
	    	if(rowsave1pos == 0)
    		{
    			if(rowsave1 != 7)
    			{
    				if(ar[rowsave1 + 1][piece4] == 0 && ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1][piece2] == 0 && ar[rowsave1][piece1] == 0)
    				{
    					ar[rowsave1 - 1][piece1] = 0;
    					ar[rowsave1 - 1][piece2] = 0;
    					ar[rowsave1][piece3] = 0;
    					ar[rowsave1][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1- 1][piece1] = 3;
    					ar[rowsave1 - 1][piece2] = 1;
    					ar[rowsave1][piece3] = 5;
    					ar[rowsave1][piece4] = 6;
    				}
    			}
    		}
    		else if(rowsave1pos == 1)
    		{
    			if(rowsave1 <= 5 && rowsave1 >= 2)
    			{
    				if((ar[rowsave1 + 2][piece3] == 0) && (ar[rowsave1][piece2] == 0))
    				{
    					ar[rowsave1 - 2][piece1] = 0;
    					ar[rowsave1 - 1][piece2] = 0;
    					ar[rowsave1][piece3] = 0;
    					ar[rowsave1 + 1][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1 - 2][piece1] = 3;
    					ar[rowsave1 - 1][piece2] = 1;
    					ar[rowsave1][piece3] = 5;
    					ar[rowsave1 + 1][piece4] = 6;
    				}
    			}
    		}
    		else if(rowsave1pos == 2)
    		{
    			if(rowsave1 <= 5)
    			{
    				if(ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1 + 1][piece4] == 0 && ar[rowsave1 + 2][piece1] == 0 && 
    				ar[rowsave1 + 2][piece2] == 0)
    				{
    					ar[rowsave1 + 1][piece1] = 0;
    					ar[rowsave1 + 1][piece2] = 0;
    					ar[rowsave1][piece3] = 0;
    					ar[rowsave1][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1 + 1][piece1] = 3;
    					ar[rowsave1 + 1][piece2] = 1;
    					ar[rowsave1][piece3] = 5;
    					ar[rowsave1][piece4] = 6;
    				}
    			}
    		}
    		else if(rowsave1pos == 3)
    		{
    			if(rowsave1 <= 4 && rowsave1 >= 1)
    			{
    				if((ar[rowsave1 + 1][piece3] == 0) && (ar[rowsave1 + 3][piece1] == 0))
    				{
    					ar[rowsave1 + 2][piece1] = 0;
    					ar[rowsave1 + 1][piece2] = 0;
    					ar[rowsave1][piece3] = 0;
    					ar[rowsave1 - 1][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1 + 2][piece1] = 3;
    					ar[rowsave1 + 1][piece2] = 1;
    					ar[rowsave1][piece3] = 5;
    					ar[rowsave1 - 1][piece4] = 6;
    				}
    			}
    		}
		}
		if(piecemovement == 1)
		{
			State2 = s42;
		}
		else
		State2 = Init2;
		break;
		
		case s52:
		if(temp2 == 8) // move left
		{
			if(rowsave1pos == 0) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 0 && rowsave1 <= 6)
				{
				    if((piece1 - 1 >= 0) && (piece3 - 1 >= 0))
				    {
				      	if(ar[rowsave1][piece1 - 1] == 0)
    					{
    						ar[rowsave1 - 1][piece1] = 0;
    						ar[rowsave1 - 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1 - 1][piece1] = 2;
    						ar[rowsave1 - 1][piece2] = 5;
    						ar[rowsave1][piece3] = 1;
    						ar[rowsave1][piece4] = 4;
    						setGround();
    						rowsave1pos = 0;
    					}  
				    }
				}
			}
			else if(rowsave1pos == 1) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if((piece3 - 1 >= 0) && (piece4 - 1 >= 0))
			    {
			     	if(rowsave1 >= 0 && rowsave1 <= 5)
    				{
    					if(ar[rowsave1][piece3 - 1] == 0 && ar[rowsave1 + 1][piece4 - 1] == 0)
    					{
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece1] = 0;
    						ar[rowsave1 + 1][piece2] = 0;
    						ar[rowsave1 + 1][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1][piece1] = 2;
    						ar[rowsave1][piece3] = 1;
    						ar[rowsave1 + 1][piece2] = 5;
    						ar[rowsave1 + 1][piece4] = 4;
    						setGround();
    						rowsave1pos = 1;
    					}
    				}   
			    }
			}
			else if(rowsave1pos == 2) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if((piece4 - 1 >= 0) && (piece2 - 1 >= 0))
			    {
			        if(rowsave1 >= 0 && rowsave1 <= 5)
    				{
    					if((ar[rowsave1][piece4 - 1] == 0) && (ar[rowsave1 + 1][piece2 - 1] == 0))
    					{
    						ar[rowsave1 + 1][piece1] = 0;
    						ar[rowsave1 + 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1 + 1][piece1] = 2;
    						ar[rowsave1 + 1][piece2] = 5;
    						ar[rowsave1][piece3] = 1;
    						ar[rowsave1][piece4] = 4;
    						setGround();
    						rowsave1pos = 2;
    					}
    				}
			    }
			}
			else if(rowsave1pos == 3) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if((piece2 - 1 >= 0) && (piece1 - 1 >= 0))
			    {
			      	if(rowsave1 >= 1 && rowsave1 <= 6)
    				{
    					if(ar[rowsave1 - 1][piece2 - 1] == 0 && ar[rowsave1][piece1 - 1] == 0)
    					{
    						ar[rowsave1][piece1] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1 - 1][piece2] = 0;
    						ar[rowsave1 - 1][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1][piece1] = 2;
    						ar[rowsave1][piece3] = 1;
    						ar[rowsave1 - 1][piece2] = 5;
    						ar[rowsave1 - 1][piece4] = 4;
    						setGround();
    						rowsave1pos = 3;
    					}
    				}  
			    }
			}
		}
		if(temp2 == 4) // move right
		{
			if(rowsave1pos == 0) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 1 && rowsave1 <= 6)
				{
				    if((piece4 + 1 <= 7) && (piece2 + 1 <= 7))
				    {
				      	if((ar[rowsave1][piece4 + 1] == 0) && (ar[rowsave1 - 1][piece2 + 1] == 0))
    					{
    						ar[rowsave1 - 1][piece1] = 0;
    						ar[rowsave1 - 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1 - 1][piece1] = 2;
    						ar[rowsave1 - 1][piece2] = 5;
    						ar[rowsave1][piece3] = 1;
    						ar[rowsave1][piece4] = 4;
    						setGround();
    						rowsave1pos = 0;
    					}  
				    }
				}
			}
			else if(rowsave1pos == 1) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if((piece2 + 1 <= 7) && (piece1 + 1 <= 7))
			    {
			     	if(rowsave1 >= 0 && rowsave1 <= 5)
    				{
    					if(ar[rowsave1][piece1 + 1] == 0 && ar[rowsave1 + 1][piece2 + 1] == 0)
    					{
    						ar[rowsave1 + 1][piece4] = 0;
    						ar[rowsave1 + 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece1] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1 + 1][piece2] = 5;
    						ar[rowsave1 + 1][piece4] = 4;
    						ar[rowsave1][piece3] = 1;
    						ar[rowsave1][piece1] = 2;
    						setGround();
    						rowsave1pos = 1;
    					}
    				}   
			    }
			}
			else if(rowsave1pos == 2) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if((piece3 + 1 <= 7) && (piece1 + 1 <= 7))
			    {
			        if((rowsave1 >= 0 && rowsave1 <= 5))
    				{
    					if((ar[rowsave1][piece3 + 1] == 0) && (ar[rowsave1 + 1][piece1 + 1] == 0))
    					{
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						ar[rowsave1 + 1][piece2] = 0;
    						ar[rowsave1 + 1][piece1] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1 + 1][piece1] = 2;
    						ar[rowsave1 + 1][piece2] = 5;
    						ar[rowsave1][piece3] = 1;
    						ar[rowsave1][piece4] = 4;
    						setGround();
    						rowsave1pos = 2;
    					}
    				}
			    }
			}
			else if(rowsave1pos == 3) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if((piece4 + 1 <= 7) && (piece3 + 1 <= 7))
			    {
			      	if(rowsave1 >= 1 && rowsave1 <= 6)
    				{
    					if(ar[rowsave1][piece3 + 1] == 0 && ar[rowsave1 - 1][piece4 + 1] == 0)
    					{
    						ar[rowsave1 - 1][piece2] = 0;
    						ar[rowsave1 - 1][piece4] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece1] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1 - 1][piece4] = 4;
    						ar[rowsave1 - 1][piece2] = 5;
    						ar[rowsave1][piece3] = 1;
    						ar[rowsave1][piece1] = 2;
    						setGround();
    						rowsave1pos = 3;
    					}
    				}  
			    }
			}
		}
		if(temp2 == 2) // move down
		{
	    	if(rowsave1pos == 0)
    		{
    			if((rowsave1 != 7) && rowsave1 >= 1)
    			{
    				if(ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1 + 1][piece4] == 0)
    				{
    					ar[rowsave1 - 1][piece1] = 0;
    					ar[rowsave1 - 1][piece2] = 0;
    					ar[rowsave1][piece3] = 0;
    					ar[rowsave1][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1 - 1][piece1] = 2;
    					ar[rowsave1 - 1][piece2] = 5;
    					ar[rowsave1][piece3] = 1;
    					ar[rowsave1][piece4] = 4;
    				}
    			}
    		}
    		else if(rowsave1pos == 1)
    		{
    			if(rowsave1 <= 5 && rowsave1 >= 0)
    			{
    				if((ar[rowsave1 + 2][piece3] == 0) && (ar[rowsave1 + 2][piece1] == 0))
    				{
    					ar[rowsave1][piece1] = 0;
    					ar[rowsave1][piece3] = 0;
    					ar[rowsave1 + 1][piece2] = 0;
    					ar[rowsave1 + 1][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1][piece1] = 2;
    					ar[rowsave1][piece3] = 1;
    					ar[rowsave1 + 1][piece2] = 5;
    					ar[rowsave1 + 1][piece4] = 4;
    				}
    			}
    		}
    		else if(rowsave1pos == 2)
    		{
    			if(rowsave1 <= 5 && rowsave1 >= 0)
    			{
    				if(ar[rowsave1 + 2][piece3] == 0 && ar[rowsave1 + 2][piece4] == 0)
    				{
    					ar[rowsave1 + 1][piece1] = 0;
    					ar[rowsave1 + 1][piece2] = 0;
    					ar[rowsave1][piece3] = 0;
    					ar[rowsave1][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1 + 1][piece1] = 2;
    					ar[rowsave1 + 1][piece2] = 5;
    					ar[rowsave1][piece3] = 1;
    					ar[rowsave1][piece4] = 4;
    				}
    			}
    		}
    		else if(rowsave1pos == 3)
    		{
    			if(rowsave1 <= 6 && rowsave1 >= 1)
    			{
    				if((ar[rowsave1 + 1][piece3] == 0) && (ar[rowsave1 + 1][piece1] == 0))
    				{
    					ar[rowsave1][piece1] = 0;
    					ar[rowsave1][piece3] = 0;
    					ar[rowsave1 - 1][piece2] = 0;
    					ar[rowsave1 - 1][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1][piece1] = 2;
    					ar[rowsave1][piece3] = 1;
    					ar[rowsave1 - 1][piece2] = 5;
    					ar[rowsave1 - 1][piece4] = 4;
    				}
    			}
    		}
		}
		if(piecemovement == 1)
		{
			State2 = s52;
		}
		else
		State2 = Init2;
		break;
		
		
		case s62:
		if(temp2 == 8) // move left
		{
			if(rowsave1pos == 0) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 1 && rowsave1 <= 6)
				{
				    if(piece2 - 1 >= 0)
				    {
				      	if((ar[rowsave1 - 1][piece1 - 1] == 0) && (ar[rowsave1][piece2 - 1] == 0))
    					{
    						ar[rowsave1 - 1][piece1] = 0;
    						ar[rowsave1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1 - 1][piece1] = 1;
    						ar[rowsave1][piece2] = 4;
    						ar[rowsave1][piece3] = 5;
    						ar[rowsave1][piece4] = 6;
    						setGround();
    						rowsave1pos = 0;
    					}  
				    }
				}
			}
			else if(rowsave1pos == 1) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece2 - 1 >= 0)
			    {
			     	if(rowsave1 >= 1 && rowsave1 <= 5)
    				{
    					if(ar[rowsave1 - 1][piece2 - 1] == 0 && ar[rowsave1][piece3 - 1] == 0 &&
    					 ar[rowsave1 + 1][piece4 - 1] == 0)
    					{
    						ar[rowsave1][piece1] = 0;
    						ar[rowsave1 - 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1 + 1][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1][piece1] = 1;
    						ar[rowsave1 - 1][piece2] = 4;
    						ar[rowsave1][piece3] = 5;
    						ar[rowsave1 + 1][piece4] = 6;
    						setGround();
    						rowsave1pos = 1;
    					}
    				}   
			    }
			}
			else if(rowsave1pos == 2) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece4 - 1 >= 0)
			    {
			        if(rowsave1 >= 0 && rowsave1 <= 5)
    				{
    					if((ar[rowsave1][piece4 - 1] == 0) && (ar[rowsave1 + 1][piece1 - 1] == 0))
    					{
    						ar[rowsave1 + 1][piece1] = 0;
    						ar[rowsave1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1 + 1][piece1] = 1;
    						ar[rowsave1][piece2] = 4;
    						ar[rowsave1][piece3] = 5;
    						ar[rowsave1][piece4] = 6;
    						setGround();
    						rowsave1pos = 2;
    					}
    				}
			    }
			}
			else if(rowsave1pos == 3) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece1 - 1 >= 0)
			    {
			      	if(rowsave1 >= 1 && rowsave1 <= 5)
    				{
    					if(ar[rowsave1 - 1][piece4 - 1] == 0  &&
    					 ar[rowsave1 + 1][piece2 - 1] == 0 && ar[rowsave1][piece1 - 1] == 0)
    					{
    						ar[rowsave1][piece1] = 0;
    						ar[rowsave1 + 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1 - 1][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1][piece1] = 1;
    						ar[rowsave1 + 1][piece2] = 4;
    						ar[rowsave1][piece3] = 5;
    						ar[rowsave1 - 1][piece4] = 6;
    						setGround();
    						rowsave1pos = 3;
    					}
    				}  
			    }
			}
		}
		if(temp2 == 4) // move right
		{
			if(rowsave1pos == 0) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 1 && rowsave1 <= 6)
				{
				    if(piece4 + 1 <= 7)
				    {
				      	if((ar[rowsave1][piece4 + 1] == 0) && (ar[rowsave1 - 1][piece1 + 1] == 0))
    					{
    						ar[rowsave1 - 1][piece1] = 0;
    						ar[rowsave1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1 - 1][piece1] = 1;
    						ar[rowsave1][piece2] = 4;
    						ar[rowsave1][piece3] = 5;
    						ar[rowsave1][piece4] = 6;
    						setGround();
    						rowsave1pos = 0;
    					}  
				    }
				}
			}
			else if(rowsave1pos == 1) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece1 + 1 <= 7)
			    {
			     	if(rowsave1 >= 1 && rowsave1 <= 5)
    				{
    					if(ar[rowsave1 - 1][piece2 + 1] == 0 && ar[rowsave1][piece1 + 1] == 0 &&
    					 ar[rowsave1 + 1][piece4 + 1] == 0)
    					{
    						ar[rowsave1][piece1] = 0;
    						ar[rowsave1 - 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1 + 1][piece4] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1][piece1] = 1;
    						ar[rowsave1 - 1][piece2] = 4;
    						ar[rowsave1][piece3] = 5;
    						ar[rowsave1 + 1][piece4] = 6;
    						setGround();
    						rowsave1pos = 1;
    					}
    				}   
			    }
			}
			else if(rowsave1pos == 2) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece2 + 1 <= 7)
			    {
			        if(rowsave1 >= 0 && rowsave1 <= 5)
    				{
    					if((ar[rowsave1][piece2 + 1] == 0) && (ar[rowsave1 + 1][piece1 + 1] == 0))
    					{
    						ar[rowsave1 + 1][piece1] = 0;
    						ar[rowsave1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1 + 1][piece1] = 1;
    						ar[rowsave1][piece2] = 4;
    						ar[rowsave1][piece3] = 5;
    						ar[rowsave1][piece4] = 6;
    						setGround();
    						rowsave1pos = 2;
    					}
    				}
			    }
			}
			else if(rowsave1pos == 3) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece4 + 1 <= 7)
			    {
			      	if(rowsave1 >= 1 && rowsave1 <= 5)
    				{
    					if(ar[rowsave1 - 1][piece4 + 1] == 0 && ar[rowsave1][piece3 + 1] == 0 &&
    					 ar[rowsave1 + 1][piece2 + 1] == 0)
    					{
    						ar[rowsave1][piece1] = 0;
    						ar[rowsave1 + 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1 - 1][piece4] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1][piece1] = 1;
    						ar[rowsave1 + 1][piece2] = 4;
    						ar[rowsave1][piece3] = 5;
    						ar[rowsave1 - 1][piece4] = 6;
    						setGround();
    						rowsave1pos = 3;
    					}
    				}  
			    }
			}
		}
		if(temp2 == 2) // move down
		{
	    	if(rowsave1pos == 0)
    		{
    			if(rowsave1 != 7)
    			{
    				if(ar[rowsave1 + 1][piece2] == 0 && ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1 + 1][piece4] == 0)
    				{
    					ar[rowsave1 - 1][piece1] = 0;
    					ar[rowsave1][piece2] = 0;
    					ar[rowsave1][piece3] = 0;
    					ar[rowsave1][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1 - 1][piece1] = 1;
    					ar[rowsave1][piece2] = 4;
    					ar[rowsave1][piece3] = 5;
    					ar[rowsave1][piece4] = 6;
    				}
    			}
    		}
    		else if(rowsave1pos == 1)
    		{
    			if(rowsave1 <= 5 && rowsave1 >= 1)
    			{
    				if((ar[rowsave1 + 2][piece4] == 0) && (ar[rowsave1 + 1][piece1] == 0))
    				{
    					ar[rowsave1][piece1] = 0;
    					ar[rowsave1 - 1][piece2] = 0;
    					ar[rowsave1][piece3] = 0;
    					ar[rowsave1 + 1][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1][piece1] = 1;
    					ar[rowsave1 - 1][piece2] = 4;
    					ar[rowsave1][piece3] = 5;
    					ar[rowsave1 + 1][piece4] = 6;
    				}
    			}
    		}
    		else if(rowsave1pos == 2)
    		{
    			if(rowsave1 + 1 <= 6)
    			{
    				if(ar[rowsave1 + 1][piece2] == 0 && ar[rowsave1 + 1][piece4] == 0 && ar[rowsave1 + 2][piece3] == 0)
    				{
    					ar[rowsave1 + 1][piece1] = 0;
    					ar[rowsave1][piece2] = 0;
    					ar[rowsave1][piece3] = 0;
    					ar[rowsave1][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1 + 1][piece1] = 1;
    					ar[rowsave1][piece2] = 4;
    					ar[rowsave1][piece3] = 5;
    					ar[rowsave1][piece4] = 6;
    				}
    			}
    		}
    		else if(rowsave1pos == 3)
    		{
    			if(rowsave1 <= 5 && rowsave1 >= 1)
    			{
    				if((ar[rowsave1 + 2][piece2] == 0) && (ar[rowsave1 + 1][piece1] == 0))
    				{
    					ar[rowsave1][piece1] = 0;
    					ar[rowsave1 + 1][piece2] = 0;
    					ar[rowsave1][piece3] = 0;
    					ar[rowsave1 - 1][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1][piece1] = 1;
    					ar[rowsave1 + 1][piece2] = 4;
    					ar[rowsave1][piece3] = 5;
    					ar[rowsave1 - 1][piece4] = 6;
    				}
    			}
    		}
		}
		if(piecemovement == 1)
		{
			State2 = s62;
		}
		else
		State2 = Init2;
		break;
		
		
		case s72:
		if(temp2 == 8) // move left
		{
			if(rowsave1pos == 0) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 1 && rowsave1 <= 6)
				{
				    if(piece2 - 1 >= 0)
				    {
				      	if((ar[rowsave1 - 1][piece1 - 1] == 0) && (ar[rowsave1][piece2 - 1] == 0))
    					{
    						ar[rowsave1 - 1][piece1] = 0;
    						ar[rowsave1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1 - 1][piece1] = 3;
    						ar[rowsave1][piece2] = 2;
    						ar[rowsave1][piece3] = 4;
    						ar[rowsave1][piece4] = 6;
    						setGround();
    						rowsave1pos = 0;
    					}  
				    }
				}
			}
			else if(rowsave1pos == 1) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece2 - 1 >= 0)
			    {
			     	if(rowsave1 >= 1 && rowsave1 <= 5)
    				{
    					if(ar[rowsave1 - 1][piece2 - 1] == 0 && ar[rowsave1][piece3 - 1] == 0 &&
    					 ar[rowsave1 + 1][piece4 - 1] == 0)
    					{
    						ar[rowsave1 - 1][piece1] = 0;
    						ar[rowsave1 - 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1 + 1][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1 - 1][piece1] = 3;
    						ar[rowsave1 - 1][piece2] = 2;
    						ar[rowsave1][piece3] = 4;
    						ar[rowsave1 + 1][piece4] = 6;
    						setGround();
    						rowsave1pos = 1;
    					}
    				}   
			    }
			}
			else if(rowsave1pos == 2) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece4 - 1 >= 0)
			    {
			        if(rowsave1 >= 1 && rowsave1 <= 5)
    				{
    					if((ar[rowsave1][piece4 - 1] == 0) && (ar[rowsave1 + 1][piece1 - 1] == 0))
    					{
    						ar[rowsave1 + 1][piece1] = 0;
    						ar[rowsave1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1 + 1][piece1] = 3;
    						ar[rowsave1][piece2] = 2;
    						ar[rowsave1][piece3] = 4;
    						ar[rowsave1][piece4] = 6;
    						setGround();
    						rowsave1pos = 2;
    					}
    				}
			    }
			}
			else if(rowsave1pos == 3) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece1 - 1 >= 0)
			    {
			      	if(rowsave1 >= 1 && rowsave1 <= 5)
    				{
    					if(ar[rowsave1 - 1][piece4 - 1] == 0  &&
    					 ar[rowsave1 + 1][piece1 - 1] == 0 && ar[rowsave1][piece3 - 1] == 0)
    					{
    						ar[rowsave1 + 1][piece1] = 0;
    						ar[rowsave1 + 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1 - 1][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1 + 1][piece1] = 3;
    						ar[rowsave1 + 1][piece2] = 2;
    						ar[rowsave1][piece3] = 4;
    						ar[rowsave1 - 1][piece4] = 6;
    						setGround();
    						rowsave1pos = 3;
    					}
    				}  
			    }
			}
		}
		if(temp2 == 4) // move right
		{
			if(rowsave1pos == 0) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 1 && rowsave1 <= 6)
				{
				    if(piece4 + 1 <= 7)
				    {
				      	if((ar[rowsave1][piece4 + 1] == 0) && (ar[rowsave1 - 1][piece1 + 1] == 0))
    					{
    						ar[rowsave1 - 1][piece1] = 0;
    						ar[rowsave1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1 - 1][piece1] = 3;
    						ar[rowsave1][piece2] = 2;
    						ar[rowsave1][piece3] = 4;
    						ar[rowsave1][piece4] = 6;
    						setGround();
    						rowsave1pos = 0;
    					}  
				    }
				}
			}
			else if(rowsave1pos == 1) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece1 + 1 <= 7)
			    {
			     	if(rowsave1 >= 1 && rowsave1 <= 5)
    				{
    					if(ar[rowsave1 - 1][piece1 + 1] == 0 && ar[rowsave1][piece3 + 1] == 0 &&
    					 ar[rowsave1 + 1][piece4 + 1] == 0)
    					{
    						ar[rowsave1 - 1][piece1] = 0;
    						ar[rowsave1 - 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1 + 1][piece4] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1 - 1][piece1] = 3;
    						ar[rowsave1 - 1][piece2] = 2;
    						ar[rowsave1][piece3] = 4;
    						ar[rowsave1 + 1][piece4] = 6;
    						setGround();
    						rowsave1pos = 1;
    					}
    				}   
			    }
			}
			else if(rowsave1pos == 2) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece2 + 1 <= 7)
			    {
			        if(rowsave1 >= 1 && rowsave1 <= 5)
    				{
    					if((ar[rowsave1][piece2 + 1] == 0) && (ar[rowsave1 + 1][piece1 + 1] == 0))
    					{
    						ar[rowsave1 + 1][piece1] = 0;
    						ar[rowsave1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1 + 1][piece1] = 3;
    						ar[rowsave1][piece2] = 2;
    						ar[rowsave1][piece3] = 4;
    						ar[rowsave1][piece4] = 6;
    						setGround();
    						rowsave1pos = 2;
    					}
    				}
			    }
			}
			else if(rowsave1pos == 3) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece4 + 1 <= 7)
			    {
			      	if(rowsave1 >= 1 && rowsave1 <= 5)
    				{
    					if(ar[rowsave1 - 1][piece4 + 1] == 0 && ar[rowsave1][piece3 + 1] == 0 &&
    					 ar[rowsave1 + 1][piece2 + 1] == 0)
    					{
    						ar[rowsave1 + 1][piece1] = 0;
    						ar[rowsave1 + 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1 - 1][piece4] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1 + 1][piece1] = 3;
    						ar[rowsave1 + 1][piece2] = 2;
    						ar[rowsave1][piece3] = 4;
    						ar[rowsave1 - 1][piece4] = 6;
    						setGround();
    						rowsave1pos = 3;
    					}
    				}  
			    }
			}
		}
		if(temp2 == 2) // move down
		{
	    	if(rowsave1pos == 0)
    		{
    			if(rowsave1 != 7)
    			{
    				if(ar[rowsave1 + 1][piece2] == 0 && ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1 + 1][piece4] == 0)
    				{
    					ar[rowsave1 - 1][piece1] = 0;
    					ar[rowsave1][piece2] = 0;
    					ar[rowsave1][piece3] = 0;
    					ar[rowsave1][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1 - 1][piece1] = 3;
    					ar[rowsave1][piece2] = 2;
    					ar[rowsave1][piece3] = 4;
    					ar[rowsave1][piece4] = 6;
    				}
    			}
    		}
    		else if(rowsave1pos == 1)
    		{
    			if(rowsave1 <= 5 && rowsave1 >= 1)
    			{
    				if((ar[rowsave1 + 2][piece4] == 0) && (ar[rowsave1][piece1] == 0))
    				{
    					ar[rowsave1 - 1][piece1] = 0;
    					ar[rowsave1 - 1][piece2] = 0;
    					ar[rowsave1][piece3] = 0;
    					ar[rowsave1 + 1][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1 - 1][piece1] = 3;
    					ar[rowsave1 - 1][piece2] = 2;
    					ar[rowsave1][piece3] = 4;
    					ar[rowsave1 + 1][piece4] = 6;
    				}
    			}
    		}
    		else if(rowsave1pos == 2)
    		{
    			if(rowsave1 + 1 <= 6)
    			{
    				if(ar[rowsave1 + 1][piece4] == 0 && ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1 + 2][piece1] == 0)
    				{
    					ar[rowsave1 + 1][piece1] = 0;
    					ar[rowsave1][piece2] = 0;
    					ar[rowsave1][piece3] = 0;
    					ar[rowsave1][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1 + 1][piece1] = 3;
    					ar[rowsave1][piece2] = 2;
    					ar[rowsave1][piece3] = 4;
    					ar[rowsave1][piece4] = 6;
    				}
    			}
    		}
    		else if(rowsave1pos == 3)
    		{
    			if(rowsave1 <= 5 && rowsave1 >= 1)
    			{
    				if((ar[rowsave1 + 2][piece2] == 0) && (ar[rowsave1 + 2][piece1] == 0))
    				{
    					ar[rowsave1 + 1][piece1] = 0;
    					ar[rowsave1 + 1][piece2] = 0;
    					ar[rowsave1][piece3] = 0;
    					ar[rowsave1 - 1][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1 + 1][piece1] = 3;
    					ar[rowsave1 + 1][piece2] = 2;
    					ar[rowsave1][piece3] = 4;
    					ar[rowsave1 - 1][piece4] = 6;
    				}
    			}
    		}
		}
		if(piecemovement == 1)
		{
			State2 = s72;
		}
		else
		State2 = Init2;
		break;
		
		case s82:
		if(temp2 == 8) // move left
		{
			if(rowsave1pos == 0) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 1 && rowsave1 <= 6)
				{
				    if(piece2 - 1 >= 0)
				    {
				      	if((ar[rowsave1 - 1][piece1 - 1] == 0) && (ar[rowsave1][piece2 - 1] == 0))
    					{
    						ar[rowsave1 - 1][piece1] = 0;
    						ar[rowsave1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1 - 1][piece1] = 4;
    						ar[rowsave1][piece2] = 1;
    						ar[rowsave1][piece3] = 3;
    						ar[rowsave1][piece4] = 6;
    						setGround();
    						rowsave1pos = 0;
    					}  
				    }
				}
			}
			else if(rowsave1pos == 1) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece2 - 1 >= 0)
			    {
			     	if(rowsave1 >= 1 && rowsave1 <= 5)
    				{
    					if(ar[rowsave1 - 1][piece2 - 1] == 0 && ar[rowsave1][piece3 - 1] == 0 &&
    					 ar[rowsave1 + 1][piece4 - 1] == 0)
    					{
    						ar[rowsave1 + 1][piece1] = 0;
    						ar[rowsave1 - 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1 + 1][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1 + 1][piece1] = 4;
    						ar[rowsave1 - 1][piece2] = 1;
    						ar[rowsave1][piece3] = 3;
    						ar[rowsave1 + 1][piece4] = 6;
    						setGround();
    						rowsave1pos = 1;
    					}
    				}   
			    }
			}
			else if(rowsave1pos == 2) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece4 - 1 >= 0)
			    {
			        if(rowsave1 >= 1 && rowsave1 <= 5)
    				{
    					if((ar[rowsave1][piece4 - 1] == 0) && (ar[rowsave1 + 1][piece1 - 1] == 0))
    					{
    						ar[rowsave1 + 1][piece1] = 0;
    						ar[rowsave1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1 + 1][piece1] = 4;
    						ar[rowsave1][piece2] = 1;
    						ar[rowsave1][piece3] = 3;
    						ar[rowsave1][piece4] = 6;
    						setGround();
    						rowsave1pos = 2;
    					}
    				}
			    }
			}
			else if(rowsave1pos == 3) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece1 - 1 >= 0)
			    {
			      	if(rowsave1 >= 1 && rowsave1 <= 5)
    				{
    					if(ar[rowsave1 - 1][piece1 - 1] == 0  &&
    					 ar[rowsave1 + 1][piece2 - 1] == 0 && ar[rowsave1][piece3 - 1] == 0)
    					{
    						ar[rowsave1 - 1][piece1] = 0;
    						ar[rowsave1 + 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1 - 1][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1 - 1][piece1] = 4;
    						ar[rowsave1 + 1][piece2] = 1;
    						ar[rowsave1][piece3] = 3;
    						ar[rowsave1 - 1][piece4] = 6;
    						setGround();
    						rowsave1pos = 3;
    					}
    				}  
			    }
			}
		}
		if(temp2 == 4) // move right
		{
			if(rowsave1pos == 0) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 1 && rowsave1 <= 6)
				{
				    if(piece4 + 1 <= 7)
				    {
				      	if((ar[rowsave1][piece4 + 1] == 0) && (ar[rowsave1 - 1][piece1 + 1] == 0))
    					{
    						ar[rowsave1 - 1][piece1] = 0;
    						ar[rowsave1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1 - 1][piece1] = 4;
    						ar[rowsave1][piece2] = 1;
    						ar[rowsave1][piece3] = 3;
    						ar[rowsave1][piece4] = 6;
    						setGround();
    						rowsave1pos = 0;
    					}  
				    }
				}
			}
			else if(rowsave1pos == 1) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece1 + 1 <= 7)
			    {
			     	if(rowsave1 >= 1 && rowsave1 <= 5)
    				{
    					if(ar[rowsave1 - 1][piece2 + 1] == 0 && ar[rowsave1][piece3 + 1] == 0 &&
    					 ar[rowsave1 + 1][piece1 + 1] == 0)
    					{
    						ar[rowsave1 + 1][piece1] = 0;
    						ar[rowsave1 - 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1 + 1][piece4] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1 + 1][piece1] = 4;
    						ar[rowsave1 - 1][piece2] = 1;
    						ar[rowsave1][piece3] = 3;
    						ar[rowsave1 + 1][piece4] = 6;
    						setGround();
    						rowsave1pos = 1;
    					}
    				}   
			    }
			}
			else if(rowsave1pos == 2) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece2 + 1 <= 7)
			    {
			        if(rowsave1 >= 1 && rowsave1 <= 5)
    				{
    					if((ar[rowsave1][piece2 + 1] == 0) && (ar[rowsave1 + 1][piece1 + 1] == 0))
    					{
    						ar[rowsave1 + 1][piece1] = 0;
    						ar[rowsave1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1 + 1][piece1] = 4;
    						ar[rowsave1][piece2] = 1;
    						ar[rowsave1][piece3] = 3;
    						ar[rowsave1][piece4] = 6;
    						setGround();
    						rowsave1pos = 2;
    					}
    				}
			    }
			}
			else if(rowsave1pos == 3) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece4 + 1 <= 7)
			    {
			      	if(rowsave1 >= 1 && rowsave1 <= 5)
    				{
    					if(ar[rowsave1 - 1][piece4 + 1] == 0 && ar[rowsave1][piece3 + 1] == 0 &&
    					 ar[rowsave1 + 1][piece2 + 1] == 0)
    					{
    						ar[rowsave1 - 1][piece1] = 0;
    						ar[rowsave1 + 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1 - 1][piece4] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1 - 1][piece1] = 4;
    						ar[rowsave1 + 1][piece2] = 1;
    						ar[rowsave1][piece3] = 3;
    						ar[rowsave1 - 1][piece4] = 6;
    						setGround();
    						rowsave1pos = 3;
    					}
    				}  
			    }
			}
		}
		if(temp2 == 2) // move down
		{
	    	if(rowsave1pos == 0)
    		{
    			if(rowsave1 != 7)
    			{
    				if(ar[rowsave1 + 1][piece2] == 0 && ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1 + 1][piece4] == 0)
    				{
    					ar[rowsave1 - 1][piece1] = 0;
    					ar[rowsave1][piece2] = 0;
    					ar[rowsave1][piece3] = 0;
    					ar[rowsave1][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1 - 1][piece1] = 4;
    					ar[rowsave1][piece2] = 1;
    					ar[rowsave1][piece3] = 3;
    					ar[rowsave1][piece4] = 6;
    				}
    			}
    		}
    		else if(rowsave1pos == 1)
    		{
    			if(rowsave1 <= 5 && rowsave1 >= 1)
    			{
    				if((ar[rowsave1 + 2][piece4] == 0) && (ar[rowsave1 + 2][piece1] == 0))
    				{
    					ar[rowsave1 + 1][piece1] = 0;
    					ar[rowsave1 - 1][piece2] = 0;
    					ar[rowsave1][piece3] = 0;
    					ar[rowsave1 + 1][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1 + 1][piece1] = 4;
    					ar[rowsave1 - 1][piece2] = 1;
    					ar[rowsave1][piece3] = 3;
    					ar[rowsave1 + 1][piece4] = 6;
    				}
    			}
    		}
    		else if(rowsave1pos == 2)
    		{
    			if(rowsave1 + 1 <= 6)
    			{
    				if(ar[rowsave1 + 1][piece2] == 0 && ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1 + 2][piece1] == 0)
    				{
    					ar[rowsave1 + 1][piece1] = 0;
    					ar[rowsave1][piece2] = 0;
    					ar[rowsave1][piece3] = 0;
    					ar[rowsave1][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1 + 1][piece1] = 4;
    					ar[rowsave1][piece2] = 1;
    					ar[rowsave1][piece3] = 3;
    					ar[rowsave1][piece4] = 6;
    				}
    			}
    		}
    		else if(rowsave1pos == 3)
    		{
    			if(rowsave1 <= 5 && rowsave1 >= 1)
    			{
    				if((ar[rowsave1 + 2][piece2] == 0) && (ar[rowsave1][piece1] == 0))
    				{
    					ar[rowsave1 - 1][piece1] = 0;
    					ar[rowsave1 + 1][piece2] = 0;
    					ar[rowsave1][piece3] = 0;
    					ar[rowsave1 - 1][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1 - 1][piece1] = 4;
    					ar[rowsave1 + 1][piece2] = 1;
    					ar[rowsave1][piece3] = 3;
    					ar[rowsave1 - 1][piece4] = 6;
    				}
    			}
    		}
		}
		if(piecemovement == 1)
		{
			State2 = s82;
		}
		else
		State2 = Init2;
		break;
	
		case s92:
		if(temp2 == 8) // move left
		{
			if(rowsave1pos == 0) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 1 && rowsave1 <= 6)
				{
				    if(piece4 - 1 >= 0)
				    {
				      	if((ar[rowsave1][piece4 - 1] == 0) && (ar[rowsave1 - 1][piece2 - 1] == 0))
    					{
    						ar[rowsave1 - 1][piece1] = 0;
    						ar[rowsave1 - 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1 - 1][piece1] = 5;
    						ar[rowsave1 - 1][piece2] = 1;
    						ar[rowsave1][piece3] = 4;
    						ar[rowsave1][piece4] = 2;
    						setGround();
    						rowsave1pos = 0;
    					}  
				    }
				}
			}
			else if(rowsave1pos == 1) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece3 - 1 >= 0)
			    {
			     	if(rowsave1 >= 2 && rowsave1 <= 5)
    				{
    					if(ar[rowsave1][piece3 - 1] == 0 && ar[rowsave1 - 1][piece4 - 1] == 0 &&
    					 ar[rowsave1 + 1][piece2 - 1] == 0 && ar[rowsave1 + 2][piece1 - 1] == 0)
    					{
    						ar[rowsave1 + 2][piece1] = 0;
    						ar[rowsave1 + 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1 - 1][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1 + 2][piece1] = 5;
    						ar[rowsave1 + 1][piece2] = 1;
    						ar[rowsave1][piece3] = 4;
    						ar[rowsave1 - 1][piece4] = 2;
    						setGround();
    						rowsave1pos = 1;
    					}
    				}   
			    }
			}
			else if(rowsave1pos == 2) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece1 - 1 >= 0)
			    {
			        if(rowsave1 >= 1 && rowsave1 <= 5)
    				{
    					if((ar[rowsave1][piece3 - 1] == 0) && (ar[rowsave1 + 1][piece1 - 1] == 0))
    					{
    						ar[rowsave1 + 1][piece1] = 0;
    						ar[rowsave1 + 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1 + 1][piece1] = 5;
    						ar[rowsave1 + 1][piece2] = 1;
    						ar[rowsave1][piece3] = 4;
    						ar[rowsave1][piece4] = 2;
    						setGround();
    						rowsave1pos = 2;
    					}
    				}
			    }
			}
			else if(rowsave1pos == 3) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece1 - 1 >= 0)
			    {
			      	if(rowsave1 >= 2 && rowsave1 <= 5)
    				{
    					if(ar[rowsave1][piece3 - 1] == 0 && ar[rowsave1 + 1][piece4 - 1] == 0 &&
    					 ar[rowsave1 - 1][piece2 - 1] == 0 && ar[rowsave1 - 2][piece1 - 1] == 0)
    					{
    						ar[rowsave1 - 2][piece1] = 0;
    						ar[rowsave1 - 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1 + 1][piece4] = 0;
    						piece1 -= 1;
    						piece2 -= 1;
    						piece3 -= 1;
    						piece4 -= 1;
    						ar[rowsave1 - 2][piece1] = 5;
    						ar[rowsave1 - 1][piece2] = 1;
    						ar[rowsave1][piece3] = 4;
    						ar[rowsave1 + 1][piece4] = 2;
    						setGround();
    						rowsave1pos = 3;
    					}
    				}  
			    }
			}
		}
		if(temp2 == 4) // move right
		{
			if(rowsave1pos == 0) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 1 && rowsave1 <= 6)  // row check
				{
				    if((piece1 + 1 <= 7) && (piece3 + 1 <= 7))  // piece bundary check
				    {
				      	if((ar[rowsave1 - 1][piece1 + 1] == 0) && (ar[rowsave1][piece3 + 1] == 0))   // array check
    					{
    						ar[rowsave1 - 1][piece1] = 0;
    						ar[rowsave1 - 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1 - 1][piece1] = 5;
    						ar[rowsave1 - 1][piece2] = 1;
    						ar[rowsave1][piece3] = 4;
    						ar[rowsave1][piece4] = 2;
    						setGround();
    						rowsave1pos = 0;
    					}  
				    }
				}
			}
			else if(rowsave1pos == 1) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece2 + 1 <= 7)
			    {
			     	if(rowsave1 >= 2 && rowsave1 <= 5)
    				{
    					if(ar[rowsave1 + 2][piece1 + 1] == 0 && ar[rowsave1 + 1][piece2 + 1] == 0 &&
    					 ar[rowsave1][piece3 + 1] == 0 && ar[rowsave1 - 1][piece4 + 1] == 0)
    					{
    						ar[rowsave1 + 2][piece1] = 0;
    						ar[rowsave1 + 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1 - 1][piece4] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1 + 2][piece1] = 5;
    						ar[rowsave1 + 1][piece2] = 1;
    						ar[rowsave1][piece3] = 4;
    						ar[rowsave1 - 1][piece4] = 2;
    						setGround();
    						rowsave1pos = 1;
    					}
    				}   
			    }
			}
			else if(rowsave1pos == 2) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece4 + 1 <= 7)
			    {
			        if(rowsave1 >= 0 && rowsave1 <= 5)
    				{
    					if((ar[rowsave1][piece4 + 1] == 0)  && (ar[rowsave1 + 1][piece2 + 1] == 0))
    					{
    						ar[rowsave1 + 1][piece1] = 0;
    						ar[rowsave1 + 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1][piece4] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1 + 1][piece1] = 5;
    						ar[rowsave1 + 1][piece2] = 1;
    						ar[rowsave1][piece3] = 4;
    						ar[rowsave1][piece4] = 2;
    						setGround();
    						rowsave1pos = 2;
    					}
    				}
			    }
			}
			else if(rowsave1pos == 3) // pos = which direction it's facing, rowsave keep track of which row we in
			{
			    if(piece4 + 1 <= 7)
			    {
			      	if(rowsave1 >= 2 && rowsave1 <= 5)
    				{
    					if(ar[rowsave1 + 1][piece4 + 1] == 0 && ar[rowsave1][piece3 + 1] == 0 &&
    					 ar[rowsave1 - 1][piece2 + 1] == 0 && ar[rowsave1 - 2][piece1 + 1] == 0)
    					{
    						ar[rowsave1 - 2][piece1] = 0;
    						ar[rowsave1 - 1][piece2] = 0;
    						ar[rowsave1][piece3] = 0;
    						ar[rowsave1 + 1][piece4] = 0;
    						piece1 += 1;
    						piece2 += 1;
    						piece3 += 1;
    						piece4 += 1;
    						ar[rowsave1 - 2][piece1] = 5;
    						ar[rowsave1 - 1][piece2] = 1;
    						ar[rowsave1][piece3] = 4;
    						ar[rowsave1 + 1][piece4] = 2;
    						setGround();
    						rowsave1pos = 3;
    					}
    				}  
			    }
			}
		}
		if(temp2 == 2) // move down
		{
	    	if(rowsave1pos == 0)
    		{
    			if(rowsave1 != 7)
    			{
    				if(ar[rowsave1 + 1][piece4] == 0 && ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1][piece2] == 0 && ar[rowsave1][piece1] == 0)
    				{
    					ar[rowsave1 - 1][piece1] = 0;
    					ar[rowsave1 - 1][piece2] = 0;
    					ar[rowsave1][piece3] = 0;
    					ar[rowsave1][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1 - 1][piece1] = 5;
    					ar[rowsave1 - 1][piece2] = 1;
    					ar[rowsave1][piece3] = 4;
    					ar[rowsave1][piece4] = 2;
    				}
    			}
    		}
    		else if(rowsave1pos == 1)
    		{
    			if(rowsave1 <= 4 && rowsave1 >= 1)
    			{
    				if((ar[rowsave1 + 1][piece3] == 0) && (ar[rowsave1 + 3][piece1] == 0))
    				{
    					ar[rowsave1 + 2][piece1] = 0;
    					ar[rowsave1 + 1][piece2] = 0;
    					ar[rowsave1][piece3] = 0;
    					ar[rowsave1 - 1][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1 + 2][piece1] = 5;
    					ar[rowsave1 + 1][piece2] = 1;
    					ar[rowsave1][piece3] = 4;
    					ar[rowsave1 - 1][piece4] = 2;
    				}
    			}
    		}
    		else if(rowsave1pos == 2)
    		{
    			if(rowsave1 <= 5)
    			{
    				if(ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1 + 1][piece4] == 0 && ar[rowsave1 + 2][piece1] == 0 && 
    				ar[rowsave1 + 2][piece2] == 0)
    				{
    					ar[rowsave1 + 1][piece1] = 0;
    					ar[rowsave1 + 1][piece2] = 0;
    					ar[rowsave1][piece3] = 0;
    					ar[rowsave1][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1 + 1][piece1] = 5;
    					ar[rowsave1 + 1][piece2] = 1;
    					ar[rowsave1][piece3] = 4;
    					ar[rowsave1][piece4] = 2;
    				}
    			}
    		}
    		else if(rowsave1pos == 3)
    		{
    			if(rowsave1 <= 4 && rowsave1 >= 1)
    			{
    				if((ar[rowsave1][piece2] == 0) && (ar[rowsave1 + 2][piece3] == 0))
    				{
    					ar[rowsave1 - 2][piece1] = 0;
    					ar[rowsave1 - 1][piece2] = 0;
    					ar[rowsave1][piece3] = 0;
    					ar[rowsave1 + 1][piece4] = 0;
    					rowsave1++;
    					ar[rowsave1 - 2][piece1] = 5;
    					ar[rowsave1 - 1][piece2] = 1;
    					ar[rowsave1][piece3] = 4;
    					ar[rowsave1 + 1][piece4] = 2;
    				}
    			}
    		}
		}
		if(piecemovement == 1)
		{
			State2 = s92;
		}
		else
		State2 = Init2;
		break;
		
	}
	switch(State2)
	{case Init2:	break;
		case s32:
		break;
		default:
		break;
	} // State actions
}

enum States1 { Init1, s31, s41, s51,s61, s71,s81,s91} State1;

void Tick1()  // rotate!
{
	switch(State1)
	{                                   // Transitions
		case Init1:
		if(piecemovement == 1)
		{
			if(piecenum == 1)
			State1 = s31;
			else if(piecenum == 2)
			{
				State1 = s41;
			}
			else if(piecenum == 3)
			{
				State1 = s51;
			}
			else if(piecenum == 4)
			{
				State1 = s61;
			}
			else if(piecenum == 5)
			{
				State1 = s71;
			}
			else if(piecenum == 6)
			{
				State1 = s81;
			}
			else if(piecenum == 7)
			{
				State1 = s91;
			}
		}
		break;
		
		case s31:
		if(GetBit(PINA,2) == 0)
		{
			if(rowsave1pos == 0) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 != 0 && rowsave1 < 6)
				{
					if(ar[rowsave1 - 1][piece1 + 1] == 0 && ar[rowsave1 + 1][piece2] == 0 && ar[rowsave1 + 2][piece2] == 0)
					{
						ar[rowsave1][piece1] = 0;
						ar[rowsave1][piece3] = 0;
						ar[rowsave1][piece4] = 0;
						piece1 += 1;
						piece3 -= 1;
						piece4 -= 2;
						ar[rowsave1 - 1][piece1] = 1;
						ar[rowsave1 + 1][piece3] = 3;
						ar[rowsave1 + 2][piece4] = 4;
						setGround();
						rowsave1pos = 1;
					}
				}
			}
			else if(rowsave1pos == 1) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 != 0 && rowsave1 < 6)
				{
					if(piece2 >= 2)
					{
						if(ar[rowsave1][piece1 - 1] == 0 && ar[rowsave1][piece2 + 1] == 0 && ar[rowsave1][piece1 - 2] == 0)
						{
							ar[rowsave1 - 1][piece1] = 0;
							ar[rowsave1 + 1][piece3] = 0;
							ar[rowsave1 + 2][piece4] = 0;
							piece1 += 1;
							piece3 -= 1;
							piece4 -= 2;
							ar[rowsave1][piece1] = 1;
							ar[rowsave1][piece3] = 3;
							ar[rowsave1][piece4] = 4;
							setGround();
							rowsave1pos = 2;
						}
				}
				}
			}
			else if(rowsave1pos == 2) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 2 && rowsave1 != 7)
				{
					if(ar[rowsave1 - 1][piece2] == 0 && ar[rowsave1 - 2][piece2] == 0 && ar[rowsave1 + 1][piece2] == 0)
					{
						ar[rowsave1][piece1] = 0;
						ar[rowsave1][piece3] = 0;
						ar[rowsave1][piece4] = 0;
						piece1 -= 1;
						piece3 += 1;
						piece4 += 2;
						ar[rowsave1 + 1][piece1] = 1;
						ar[rowsave1 - 1][piece3] = 3;
						ar[rowsave1 - 2][piece4] = 4;
						setGround();
						rowsave1pos = 3;
					}
				}
			}
			else if(rowsave1pos == 3) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 0 && rowsave1 != 7)
				{
					if(piece2 >= 1 && piece2 <= 5)
					{
						if(ar[rowsave1][piece2 - 1] == 0 && ar[rowsave1][piece2 + 1] == 0 && ar[rowsave1][piece2 + 2] == 0)
						{
							ar[rowsave1 + 1][piece1] = 0;
							ar[rowsave1 - 1][piece3] = 0;
							ar[rowsave1 - 2][piece4] = 0;
							piece1 -= 1;
							piece3 += 1;
							piece4 += 2;
							ar[rowsave1][piece1] = 1;
							ar[rowsave1][piece3] = 3;
							ar[rowsave1][piece4] = 4;
							setGround();
							rowsave1pos = 0;
						}
					}
				}
			}
		}
		if(piecemovement == 1)
		{
			State1 = s31;
		}
		else
		State1 = Init1;
		break;
		
		case s41:
		if(GetBit(PINA,2) == 0)
		{
			if(rowsave1pos == 0) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 2 && rowsave1 <= 6)
				{
					if(ar[rowsave1 - 2][piece1 + 2] == 0 && ar[rowsave1 - 1][piece2 + 1] == 0
					&& ar[rowsave1][piece3 - 1] == 0 && ar[rowsave1 + 1][piece4 - 2] == 0)
					{
						ar[rowsave1 - 1][piece1] = 0;
						ar[rowsave1 - 1][piece2] = 0;
						ar[rowsave1][piece3] = 0;
						ar[rowsave1][piece4] = 0;
						piece1 += 2;
						piece2 += 1;
						piece3 -= 1;
						piece4 -= 2;
						ar[rowsave1 - 2][piece1] = 3;
						ar[rowsave1 - 1][piece2] = 1;
						ar[rowsave1][piece3] = 5;
						ar[rowsave1 + 1][piece4] = 6;
						setGround();
						rowsave1pos = 1;
					}
				}
			}
			else if(rowsave1pos == 1) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 2 && rowsave1 <= 6)
				{
					if(piece2 >= 2)
					{
						if(ar[rowsave1 - 1][piece3] == 0 && ar[rowsave1 - 1][piece4 - 1] == 0
						&& ar[rowsave1][piece2] == 0 && ar[rowsave1][piece1] == 0)
						{
							ar[rowsave1][piece3] = 0;
							ar[rowsave1 - 1][piece2] = 0;
							ar[rowsave1 - 2][piece1] = 0;
							ar[rowsave1 + 1][piece4] = 0;
							piece1 += 1;
							piece4 -= 1;
							rowsave1--;
							ar[rowsave1][piece4] = 6;
							ar[rowsave1][piece3] = 5;
							ar[rowsave1 + 1][piece2] = 1;
							ar[rowsave1 + 1][piece1] = 3;
							setGround();
							rowsave1pos = 2;
						}
				}
				}
			}
			else if(rowsave1pos == 2) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 1 && rowsave1 <= 6)
				{
					if(ar[rowsave1][piece3 + 1] == 0 && ar[rowsave1 - 1][piece1 + 1] == 0 
					&& ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1 + 2][piece3] == 0)
					{
						ar[rowsave1][piece3] = 0;
						ar[rowsave1][piece4] = 0;
						ar[rowsave1 + 1][piece2] = 0;
						ar[rowsave1 + 1][piece1] = 0;
						piece1 -= 2;
						piece2 -= 1;
						piece3 += 1;
						piece4 += 2;
						ar[rowsave1][piece3] = 5;
						ar[rowsave1 + 2][piece1] = 3;
						ar[rowsave1 + 1][piece2] = 1;
						ar[rowsave1 - 1][piece4] = 6;
						setGround();
						rowsave1pos = 3;
					}
				}
			}
			else if(rowsave1pos == 3) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 1 && rowsave1 <= 5)
				{
					if(piece3 >=2 && piece3 <=6)
					{
						if(ar[rowsave1][piece2 - 1] == 0 && ar[rowsave1][piece2] == 0 
						&& ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1 + 1][piece3 + 1] == 0)
						{
							ar[rowsave1 - 1][piece4] = 0;
							ar[rowsave1][piece3] = 0;
							ar[rowsave1 + 1][piece2] = 0;
							ar[rowsave1 + 2][piece1] = 0;
							piece1 -= 1;
							piece4 += 1;
							ar[rowsave1 - 1][piece1] = 3;
							ar[rowsave1 - 1][piece2] = 1;
							ar[rowsave1][piece3] = 5;
							ar[rowsave1][piece4] = 6;
							setGround();
							rowsave1pos = 0;
						}
					}
				}
			}
		}
		if(piecemovement == 1)
		{
			State1 = s41;
		}
		else
		State1 = Init1;
		break;
		
		case s51:
		if(GetBit(PINA,2) == 0)
		{
			if(rowsave1pos == 0) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 1 && rowsave1 <= 6)
				{
						ar[rowsave1 - 1][piece1] = 1;
						ar[rowsave1 - 1][piece2] = 2;
						ar[rowsave1][piece3] = 4;
						ar[rowsave1][piece4] = 5;
						rowsave1--;
						piece1 += 1;
						piece4 -= 1;
						setGround();
						rowsave1pos = 1;
				}
			}
			else if(rowsave1pos == 1) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 2 && rowsave1 <= 6)
				{
					if(piece2 >= 3)
					{
						
						piece3 += 1;
						piece2 -= 1;
						ar[rowsave1][piece3] = 1;
						ar[rowsave1][piece4] = 4;
						ar[rowsave1 + 1][piece1] = 2;
						ar[rowsave1 + 1][piece2] = 5;
						setGround();
						rowsave1pos = 2;
						
					}
				}
			}
			else if(rowsave1pos == 2) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 1 && rowsave1 <= 6)
				{
						piece1 -= 1;
						piece4 += 1;
						rowsave1++;
						ar[rowsave1][piece3] = 1;
						ar[rowsave1][piece1] = 2;
						ar[rowsave1 - 1][piece2] = 5;
						ar[rowsave1 - 1][piece4] = 4;
						setGround();
						rowsave1pos = 3;
				}
			}
			else if(rowsave1pos == 3) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 1 && rowsave1 <= 6)
				{
						piece3 -= 1;
						piece2 += 1;
						ar[rowsave1 - 1][piece1] = 2;
						ar[rowsave1 - 1][piece2] = 5;
						ar[rowsave1][piece3] = 1;
						ar[rowsave1][piece4] = 4;
						setGround();
						rowsave1pos = 0;
				}
			}
		}
		if(piecemovement == 1)
		{
			State1 = s51;
		}
		else
		State1 = Init1;
		break;
		
		case s61:
		if(GetBit(PINA,2) == 0)
		{
			if(rowsave1pos == 0) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 1 && rowsave1 <= 6)
				{
					if(ar[rowsave1 + 1][piece3] == 0)
					{
						ar[rowsave1 - 1][piece1] = 0;
						ar[rowsave1][piece2] = 0;
						ar[rowsave1][piece3] = 0;
						ar[rowsave1][piece4] = 0;
						piece1 += 1;
						piece2 += 1;
						piece4 -= 1;
						ar[rowsave1][piece1] = 1;
						ar[rowsave1][piece3] = 5;
						ar[rowsave1 - 1][piece2] = 4;
						ar[rowsave1 + 1][piece4] = 6;
						setGround();
						rowsave1pos = 1;
					}
				}
			}
			else if(rowsave1pos == 1) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 1 && rowsave1 <= 5)
				{
					if(piece4 - 1 >= 0)
					{
						if(ar[rowsave1][piece3 - 1] == 0)
						{
							ar[rowsave1 - 1][piece2] = 0;
							ar[rowsave1][piece1] = 0;
							ar[rowsave1 + 1][piece4] = 0;
							ar[rowsave1][piece3] = 0;
							piece1 -= 1;
							piece2 += 1;
							piece4 -= 1;
							ar[rowsave1 + 1][piece1] = 1;
							ar[rowsave1][piece2] = 4;
							ar[rowsave1][piece3] = 5;
							ar[rowsave1][piece4] = 6;
							setGround();
							rowsave1pos = 2;
						}
					}
				}
			}
			else if(rowsave1pos == 2) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 1 && rowsave1 <= 6)
				{
					if(ar[rowsave1 - 1][piece3] == 0)
					{
						ar[rowsave1 + 1][piece1] = 0;
						ar[rowsave1][piece2] = 0;
						ar[rowsave1][piece3] = 0;
						ar[rowsave1][piece4] = 0;
						piece1 -= 1;
						piece2 -= 1;
						piece4 += 1;
						ar[rowsave1][piece1] = 1;
						ar[rowsave1][piece3] = 5;
						ar[rowsave1 - 1][piece4] = 6;
						ar[rowsave1 + 1][piece2 ] = 4;
						setGround();
						rowsave1pos = 3;
					}
				}
			}
			else if(rowsave1pos == 3) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 1 && rowsave1 <= 6)
				{
					if(piece4 + 1 <= 7)
					{
						if(ar[rowsave1][piece3 + 1] == 0)
						{
							ar[rowsave1][piece1] = 0;
							ar[rowsave1 + 1][piece2] = 0;
							ar[rowsave1][piece3] = 0;
							ar[rowsave1 - 1][piece4] = 0;
							piece1 += 1;
							piece2 -= 1;
							piece4 += 1;
							ar[rowsave1 - 1][piece1] = 1;
							ar[rowsave1][piece2] = 4;
							ar[rowsave1][piece3] = 5;
							ar[rowsave1][piece4] = 6;
							setGround();
							rowsave1pos = 0;
						}
					}
				}
			}
		}
		if(piecemovement == 1)
		{
			State1 = s61;
		}
		else
		State1 = Init1;
		break;
		
		
		case s71:
		if(GetBit(PINA,2) == 0)
		{
			if(rowsave1pos == 0) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 2 && rowsave1 <= 6)
				{
					if((ar[rowsave1 - 1][piece3] == 0) && (ar[rowsave1 - 1][piece4] == 0) && (ar[rowsave1 + 1][piece3] == 0))
					{
						ar[rowsave1 - 1][piece1] = 0;
						ar[rowsave1][piece2] = 0;
						ar[rowsave1][piece3] = 0;
						ar[rowsave1][piece4] = 0;
						piece1 += 2;
						piece2 += 1;
						piece4 -= 1;
						ar[rowsave1 - 1][piece1] = 3;
						ar[rowsave1 - 1][piece2] = 2;
						ar[rowsave1][piece3] = 4;
						ar[rowsave1 + 1][piece4] = 6;
						setGround();
						rowsave1pos = 1;
					}
				}
			}
			else if(rowsave1pos == 1) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 2 && rowsave1 <= 5)
				{
					if(piece3 - 1 > 0)
					{
						if((ar[rowsave1][piece3 - 1] == 0) && (ar[rowsave1][piece1] == 0) 
						&& (ar[rowsave1 + 1][piece1] == 0))
						{
							ar[rowsave1 - 1][piece2] = 0;
							ar[rowsave1 - 1][piece1] = 0;
							ar[rowsave1 + 1][piece4] = 0;
							ar[rowsave1][piece3] = 0;
							piece2 += 1;
							piece4 -= 1;
							ar[rowsave1 + 1][piece1] = 3;
							ar[rowsave1][piece2] = 2;
							ar[rowsave1][piece3] = 4;
							ar[rowsave1][piece4] = 6;
							setGround();
							rowsave1pos = 2;
						}
					}
				}
			}
			else if(rowsave1pos == 2) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 2 && rowsave1 <= 6)
				{
					if((ar[rowsave1 - 1][piece3] == 0) && (ar[rowsave1 + 1][piece3] == 0) 
					&& (ar[rowsave1 + 1][piece3 - 1] == 0))
					{
						ar[rowsave1 + 1][piece1] = 0;
						ar[rowsave1][piece2] = 0;
						ar[rowsave1][piece3] = 0;
						ar[rowsave1][piece4] = 0;
						piece1 -= 2;
						piece2 -= 1;
						piece4 += 1;
						ar[rowsave1 + 1][piece1] = 3;
						ar[rowsave1 + 1][piece2] = 2;
						ar[rowsave1][piece3] = 4;
						ar[rowsave1 - 1][piece4 ] = 6;
						setGround();
						rowsave1pos = 3;
					}
				}
			}
			else if(rowsave1pos == 3) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 2 && rowsave1 <= 6)
				{
					if(piece3 + 1 < 7)
					{
						if((ar[rowsave1][piece3 + 1] == 0) && (ar[rowsave1][piece3 - 1] == 0)
						&& (ar[rowsave1 - 1][piece3 - 1] == 0))
						{
							ar[rowsave1 + 1][piece1] = 0;
							ar[rowsave1 + 1][piece2] = 0;
							ar[rowsave1][piece3] = 0;
							ar[rowsave1 - 1][piece4] = 0;
							piece2 -= 1;
							piece4 += 1;
							ar[rowsave1 - 1][piece1] = 3;
							ar[rowsave1][piece2] = 2;
							ar[rowsave1][piece3] = 4;
							ar[rowsave1][piece4] = 6;
							setGround();
							rowsave1pos = 0;
						}
					}
				}
			}
		}
		if(piecemovement == 1)
		{
			State1 = s71;
		}
		else
		State1 = Init1;
		break;
		
		
		case s81:
		if(GetBit(PINA,2) == 0)
		{
			if(rowsave1pos == 0) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 1 && rowsave1 <= 6)
				{
					if((ar[rowsave1 + 1][piece3] == 0) && (ar[rowsave1 + 1][piece4] == 0) && (ar[rowsave1 - 1][piece3] == 0))
					{
						ar[rowsave1 - 1][piece1] = 0;
						ar[rowsave1][piece2] = 0;
						ar[rowsave1][piece3] = 0;
						ar[rowsave1][piece4] = 0;
						piece2 += 1;
						piece4 -= 1;
						ar[rowsave1 + 1][piece1] = 4;
						ar[rowsave1 - 1][piece2] = 1;
						ar[rowsave1][piece3] = 3;
						ar[rowsave1 + 1][piece4] = 6;
						setGround();
						rowsave1pos = 1;
					}
				}
			}
			else if(rowsave1pos == 1) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 2 && rowsave1 <= 5)
				{
					if(piece3 - 1 > 0)
					{
						if((ar[rowsave1][piece3 + 1] == 0) && (ar[rowsave1][piece3 - 1] == 0) 
						&& (ar[rowsave1 + 1][piece3 - 1] == 0))
						{
							ar[rowsave1 - 1][piece2] = 0;
							ar[rowsave1 + 1][piece1] = 0;
							ar[rowsave1 + 1][piece4] = 0;
							ar[rowsave1][piece3] = 0;
							piece1 -= 2;
							piece2 += 1;
							piece4 -= 1;
							ar[rowsave1 + 1][piece1] = 4;
							ar[rowsave1][piece2] = 1;
							ar[rowsave1][piece3] = 3;
							ar[rowsave1][piece4] = 6;
							setGround();
							rowsave1pos = 2;
						}
					}
				}
			}
			else if(rowsave1pos == 2) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 2 && rowsave1 <= 6)
				{
					if((ar[rowsave1 - 1][piece3] == 0) && (ar[rowsave1 + 1][piece3] == 0) 
					&& (ar[rowsave1 - 1][piece3 - 1] == 0))
					{
						ar[rowsave1 + 1][piece1] = 0;
						ar[rowsave1][piece2] = 0;
						ar[rowsave1][piece3] = 0;
						ar[rowsave1][piece4] = 0;
						piece2 -= 1;
						piece4 += 1;
						ar[rowsave1 - 1][piece1] = 4;
						ar[rowsave1 + 1][piece2] = 1;
						ar[rowsave1][piece3] = 3;
						ar[rowsave1 - 1][piece4 ] = 6;
						setGround();
						rowsave1pos = 3;
					}
				}
			}
			else if(rowsave1pos == 3) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 2 && rowsave1 <= 6)
				{
					if(piece3 + 1 < 7)
					{
						if((ar[rowsave1][piece3 + 1] == 0) && (ar[rowsave1][piece3 - 1] == 0)
						&& (ar[rowsave1 - 1][piece3 + 1] == 0))
						{
							ar[rowsave1 - 1][piece1] = 0;
							ar[rowsave1 + 1][piece2] = 0;
							ar[rowsave1][piece3] = 0;
							ar[rowsave1 - 1][piece4] = 0;
							piece2 -= 1;
							piece1 += 2;
							piece4 += 1;
							ar[rowsave1 - 1][piece1] = 4;
							ar[rowsave1][piece2] = 1;
							ar[rowsave1][piece3] = 3;
							ar[rowsave1][piece4] = 6;
							setGround();
							rowsave1pos = 0;
						}
					}
				}
			}
		}
		if(piecemovement == 1)
		{
			State1 = s81;
		}
		else
		State1 = Init1;
		break;
		
		
		case s91:
		if(GetBit(PINA,2) == 0)
		{
			if(rowsave1pos == 0) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 2 && rowsave1 <= 6)
				{
					if(ar[rowsave1 - 2][piece3] == 0 && ar[rowsave1 - 1][piece3] == 0
					&& ar[rowsave1][piece3 + 1] == 0 && ar[rowsave1 + 1][piece3 + 1] == 0)
					{
						ar[rowsave1 - 1][piece1] = 0;
						ar[rowsave1 - 1][piece2] = 0;
						ar[rowsave1][piece3] = 0;
						ar[rowsave1][piece4] = 0;
						piece1 -= 1;
						piece4 += 1;
						rowsave1--;
						ar[rowsave1 + 2][piece1] = 5;
						ar[rowsave1 + 1][piece2] = 1;
						ar[rowsave1][piece3] = 4;
						ar[rowsave1 - 1][piece4] = 2;
						setGround();
						rowsave1pos = 1;
					}
				}
			}
			else if(rowsave1pos == 1) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 1 && rowsave1 <= 5)
				{
					if((piece2 + 1 <= 7) && (piece3 - 1 >= 0))
					{
						if(ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1][piece3 + 1] == 0
						&& ar[rowsave1][piece3 + 2] == 0 && ar[rowsave1 + 1][piece3 - 1] == 0)
						{
							ar[rowsave1][piece3] = 0;
							ar[rowsave1 + 1][piece2] = 0;
							ar[rowsave1 + 2][piece1] = 0;
							ar[rowsave1 - 1][piece4] = 0;
							piece1 -= 2;
							piece2 -= 1;
							piece3 += 1;
							piece4 += 2;
							ar[rowsave1][piece4] = 2;
							ar[rowsave1][piece3] = 4;
							ar[rowsave1 + 1][piece2] = 1;
							ar[rowsave1 + 1][piece1] = 5;
							setGround();
							rowsave1pos = 2;
						}
					}
				}
			}
			else if(rowsave1pos == 2) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 1 && rowsave1 <= 5)
				{
					if(ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1][piece3 - 1] == 0 
					&& ar[rowsave1 + 2][piece3] == 0 && ar[rowsave1 - 1][piece3 - 1] == 0)
					{
						ar[rowsave1][piece3] = 0;
						ar[rowsave1][piece4] = 0;
						ar[rowsave1 + 1][piece2] = 0;
						ar[rowsave1 + 1][piece1] = 0;
						piece1 += 1;
						piece4 -= 1;
						rowsave1++;
						ar[rowsave1][piece3] = 4;
						ar[rowsave1 - 2][piece1] = 5;
						ar[rowsave1 - 1][piece2] = 1;
						ar[rowsave1 + 1][piece4] = 2;
						setGround();
						rowsave1pos = 3;
					}
				}
			}
			else if(rowsave1pos == 3) // pos = which direction it's facing, rowsave keep track of which row we in
			{
				if(rowsave1 >= 2 && rowsave1 <= 5)
				{
					if(piece2 - 1 >= 0 && piece3 + 1 <= 7)
					{
						if(ar[rowsave1][piece3 - 1] == 0 && ar[rowsave1][piece3 - 2] == 0 
						&& ar[rowsave1 - 1][piece3] == 0 && ar[rowsave1 - 1][piece3 + 1] == 0)
						{
							ar[rowsave1 + 1][piece4] = 0;
							ar[rowsave1][piece3] = 0;
							ar[rowsave1 - 1][piece2] = 0;
							ar[rowsave1 - 2][piece1] = 0;
							piece1 += 2;
							piece2 += 1;
							piece3 -= 1;
							piece4 -= 2;
							ar[rowsave1 - 1][piece1] = 5;
							ar[rowsave1 - 1][piece2] = 1;
							ar[rowsave1][piece3] = 4;
							ar[rowsave1][piece4] = 2;
							setGround();
							rowsave1pos = 0;
						}
					}
				}
			}
		}
		if(piecemovement == 1)
		{
			State1 = s91;
		}
		else
		State1 = Init1;
		break;
		
	}
	switch(State1)
	{case Init1:	break;
		case s31:
		break;
		case s41:
		break;	
		case s51:
		break;
		case s61:
		break;
		default:
		break;
	} // State actions
}

enum States { Init, s0, s1,s3, sover, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14,s15, s16, s17,s18, s19, s20, s21} State;
void Tick()   // generateing and moving piece down
{
	switch(State){                                   // Transitions
		case Init:
		for(int i = 0; i < 8; i++)
		{
			if(ar[0][i] != 0) gamewon = 1;
		}
		if(gamewon == 1) State = sover;
		else State = s0;
		break;
		
		case s0:
		for(int i = 0; i < 8; i++)
		{
			if(ar[0][i] != 0) gamewon = 1;
		}
		temp80 = rand() % 7;
		if(temp80 == 0)
		{
			piecenum = 0;
			State = s1;
		}
		else if(temp80 == 1)
		{
			piecenum = 1;
			State = s4;
		}
		else if(temp80 == 2)
		{
			piecenum = 2;
			State = s7;
		}
		else if(temp80 == 3)
		{
			piecenum = 3;
			State = s10;
		}
		else if(temp80 == 4)
		{
			piecenum = 4;
			State = s13;
		}
		else if(temp80 == 5)
		{
			piecenum = 5;
			State = s16;
		}
		else if(temp80 == 6)
		{
			piecenum = 6;
			State = s19;
		}
		counter900 = 0;
		if(gamewon == 1) State = sover;
		break;
		
		case s1:
		piecemovement = 1;
		ar[0][2] = 1;
		ar[0][3] = 2;
		ar[0][4] = 3;
		ar[0][5] = 4;
		rowsave1 = 0;
		counter900 = 2;
		rowsave1pos = 0;
		piece1 = 2;
		piece2 = 3;
		piece3 = 4;
		piece4 = 5;
		piecenum++;
		State = s3;
		break;
	
		case s3:
		if(rowsave1pos == 0)
		{
			if((rowsave1 != 7) && (piecemovement == 1))
			{
				if(ar[rowsave1 + 1][piece1] == 0 && ar[rowsave1 + 1][piece2] == 0 && ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1 + 1][piece4] == 0)
				{
					ar[rowsave1][piece1] = 0;
					ar[rowsave1][piece2] = 0;
					ar[rowsave1][piece3] = 0;
					ar[rowsave1][piece4] = 0;
					rowsave1++;
					ar[rowsave1][piece1] = 1;
					ar[rowsave1][piece2] = 2;
					ar[rowsave1][piece3] = 3;
					ar[rowsave1][piece4] = 4;
					State = s3;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		}
		else if(rowsave1pos == 1)
		{
			if(rowsave1 <= 4 && rowsave1 >= 1 && (piecemovement == 1))
			{
				if(ar[rowsave1 + 3][piece2] == 0)
				{
					ar[rowsave1 - 1][piece1] = 0;
					ar[rowsave1][piece2] = 0;
					ar[rowsave1 + 1][piece3] = 0;
					ar[rowsave1 + 2][piece4] = 0;
					rowsave1++;
					ar[rowsave1 - 1][piece1] = 1;
					ar[rowsave1][piece2] = 2;
					ar[rowsave1 + 1][piece3] = 3;
					ar[rowsave1 + 2][piece4] = 4;
					State = s3;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		}
		else if(rowsave1pos == 2)
		{
			if(rowsave1 != 7 && (piecemovement == 1))
			{
				if(ar[rowsave1 + 1][piece1] == 0 && ar[rowsave1 + 1][piece2] == 0 && ar[rowsave1 + 1][piece3] == 0 && 
				ar[rowsave1 + 1][piece4] == 0)
				{
					ar[rowsave1][piece1] = 0;
					ar[rowsave1][piece2] = 0;
					ar[rowsave1][piece3] = 0;
					ar[rowsave1][piece4] = 0;
					rowsave1++;
					ar[rowsave1][piece1] = 1;
					ar[rowsave1][piece2] = 2;
					ar[rowsave1][piece3] = 3;
					ar[rowsave1][piece4] = 4;
					State = s3;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		}
		else if(rowsave1pos == 3)
		{
			if(rowsave1 <= 5 && rowsave1 >= 2 && (piecemovement == 1))
			{
				if(ar[rowsave1 + 2][piece2] == 0)
				{
					ar[rowsave1 + 1][piece1] = 0;
					ar[rowsave1][piece2] = 0;
					ar[rowsave1 - 1][piece3] = 0;
					ar[rowsave1 - 2][piece4] = 0;
					rowsave1++;
					ar[rowsave1 + 1][piece1] = 1;
					ar[rowsave1][piece2] = 2;
					ar[rowsave1 - 1][piece3] = 3;
					ar[rowsave1 - 2][piece4] = 4;
					State = s3;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		}
		break;
		
		case sover:
		for(int i = 0; i < 8; i++)
		{
			for(int j = 0; j < 8; j++)
			{
				ar[i][j] = 0;
			}
		}
		ar[0][0] = 1;
		ar[0][7] = 2;
		ar[7][0] = 3;
		ar[7][7] = 6;
		break;
		default:
		State = Init;
		break;
		
	    case s4:
		piecemovement = 1;
		ar[0][4] = 5;
		ar[0][5] = 6;
		rowsave1 = 0;
		rowsave1pos = 0;
		piece3 = 4;
		piece4 = 5;
		piecenum++;
		if((ar[1][4] != 0) || (ar[1][5] != 0)) gamewon = 1;
		if(gamewon == 1)
		{
			State = sover;
		}
		else
		State = s5;
		break;
		
		case s5:
		ar[0][4] = 0;
		ar[0][5] = 0;
		ar[0][2] = 3;
		ar[0][3] = 1;
		ar[1][4] = 5;
		ar[1][5] = 6;
		counter900 = 2;
		rowsave1 = 1;
		rowsave1pos = 0;
		piece1 = 2;
		piece2 = 3;
		piece3 = 4;
		piece4 = 5;
		State = s6;
		break;
		
		
		case s6:
		if(rowsave1pos == 0)
		{
			if((rowsave1 != 7) && (piecemovement == 1))
			{
				if(ar[rowsave1][piece1] == 0 && ar[rowsave1][piece2] == 0 && ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1 + 1][piece4] == 0)
				{
					ar[rowsave1 - 1][piece1] = 0;
					ar[rowsave1 - 1][piece2] = 0;
					ar[rowsave1][piece3] = 0;
					ar[rowsave1][piece4] = 0;
					rowsave1++;
					ar[rowsave1 - 1][piece1] = 3;
					ar[rowsave1 - 1][piece2] = 1;
					ar[rowsave1][piece3] = 5;
					ar[rowsave1][piece4] = 6;
					State = s6;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		}
		else if(rowsave1pos == 1)
		{
			if(rowsave1 <= 5 && rowsave1 >= 2 && (piecemovement == 1))
			{
				if((ar[rowsave1 + 2][piece3] == 0) && (ar[rowsave1][piece2] == 0))
				{
					ar[rowsave1 - 2][piece1] = 0;
					ar[rowsave1 - 1][piece2] = 0;
					ar[rowsave1][piece3] = 0;
					ar[rowsave1 + 1][piece4] = 0;
					rowsave1++;
					ar[rowsave1 - 2][piece1] = 3;
					ar[rowsave1 - 1][piece2] = 1;
					ar[rowsave1][piece3] = 5;
					ar[rowsave1 + 1][piece4] = 6;
					State = s6;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		}
		else if(rowsave1pos == 2)
		{
			if(rowsave1 <= 5 && (piecemovement == 1))
			{
				if(ar[rowsave1 + 1][piece4] == 0 && ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1 + 2][piece2] == 0 && 
				ar[rowsave1 + 2][piece1] == 0)
				{
					ar[rowsave1 + 1][piece1] = 0;
					ar[rowsave1 + 1][piece2] = 0;
					ar[rowsave1][piece3] = 0;
					ar[rowsave1][piece4] = 0;
					rowsave1++;
					ar[rowsave1 + 1][piece1] = 3;
					ar[rowsave1 + 1][piece2] = 1;
					ar[rowsave1][piece3] = 5;
					ar[rowsave1][piece4] = 6;
					State = s6;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		}
		else if(rowsave1pos == 3)
		{
			if(rowsave1 <= 4 && rowsave1 >= 1 && (piecemovement == 1))
			{
				if((ar[rowsave1 + 3][piece1] == 0) && (ar[rowsave1 + 1][piece3] == 0))
				{
					ar[rowsave1 + 2][piece1] = 0;
					ar[rowsave1 + 1][piece2] = 0;
					ar[rowsave1][piece3] = 0;
					ar[rowsave1 - 1][piece4] = 0;
					rowsave1++;
					ar[rowsave1 + 2][piece1] = 3;
					ar[rowsave1 + 1][piece2] = 1;
					ar[rowsave1][piece3] = 5;
					ar[rowsave1 - 1][piece4] = 6;
					State = s6;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		}
		break;
		
		case s7:
		piecemovement = 1;
		ar[0][3] = 1;
		ar[0][4] = 4;
		rowsave1 = 0;
		rowsave1pos = 0;
		piece3 = 3;
		piece4 = 4;
		piecenum++;
		if((ar[1][3] != 0) || (ar[1][4] != 0)) gamewon = 1;
		if(gamewon == 1)
		{
			State = sover;
		}
		else
		State = s8;
		break;
		
		case s8:
		ar[0][3] = 2;
		ar[0][4] = 5;
		ar[1][3] = 1;
		counter900 = 2;
		ar[1][4] = 4;
		rowsave1 = 1;
		rowsave1pos = 0;
		piece1 = 3;
		piece2 = 4;
		piece3 = 3;
		piece4 = 4;
		State = s9;
		break;
		
		case s9:
		if(rowsave1pos == 0)
		{
			if((rowsave1 <= 6) && (piecemovement == 1))
			{
				if(ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1 + 1][piece4] == 0)
				{
					ar[rowsave1 - 1][piece1] = 0;
					ar[rowsave1 - 1][piece2] = 0;
					ar[rowsave1][piece3] = 0;
					ar[rowsave1][piece4] = 0;
					rowsave1++;
					ar[rowsave1 - 1][piece1] = 2;
					ar[rowsave1 - 1][piece2] = 5;
					ar[rowsave1][piece3] = 1;
					ar[rowsave1][piece4] = 4;
					State = s9;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		}
		else if(rowsave1pos == 1)
		{
			if((rowsave1 <= 5) && (piecemovement == 1))
			{
				if(ar[rowsave1 + 2][piece4] == 0 && ar[rowsave1 + 2][piece2] == 0)
				{
					ar[rowsave1][piece3] = 0;
					ar[rowsave1][piece1] = 0;
					ar[rowsave1 + 1][piece2] = 0;
					ar[rowsave1 + 1][piece4] = 0;
					rowsave1++;
					ar[rowsave1][piece3] = 1;
					ar[rowsave1][piece1] = 2;
					ar[rowsave1 + 1][piece2] = 5;
					ar[rowsave1 + 1][piece4] = 4;
					State = s9;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		}
		else if(rowsave1pos == 2)
		{
			if((rowsave1 <= 5) && (piecemovement == 1))
			{
				if(ar[rowsave1 + 2][piece2] == 0 && ar[rowsave1 + 2][piece1] == 0)
				{
					ar[rowsave1][piece3] = 0;
					ar[rowsave1][piece4] = 0;
					ar[rowsave1 + 1][piece2] = 0;
					ar[rowsave1 + 1][piece1] = 0;
					rowsave1++;
					ar[rowsave1 + 1][piece2] = 5;
					ar[rowsave1 + 1][piece1] = 2;
					ar[rowsave1][piece3] = 1;
					ar[rowsave1][piece4] = 4;
					State = s9;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		}
		else if(rowsave1pos == 3)
		{
			if((rowsave1 <= 6) && (piecemovement == 1))
			{
				if(ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1 + 1][piece1] == 0)
				{
					ar[rowsave1 - 1][piece2] = 0;
					ar[rowsave1 - 1][piece4] = 0;
					ar[rowsave1][piece3] = 0;
					ar[rowsave1][piece1] = 0;
					rowsave1++;
					ar[rowsave1 - 1][piece4] = 4;
					ar[rowsave1 - 1][piece2] = 5;
					ar[rowsave1][piece3] = 1;
					ar[rowsave1][piece1] = 2;
					State = s9;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		}
		break;
		
		
		
		case s10:
		piecemovement = 1;
		ar[0][2] = 4;
		ar[0][3] = 5;
		ar[0][4] = 6;
		rowsave1 = 0;
		rowsave1pos = 0;
		piece2 = 2;
		piece3 = 3;
		piece4 = 4;
		piecenum++;
		if((ar[1][3] != 0) || (ar[1][4] != 0) || (ar[1][2] != 0)) gamewon = 1;
		if(gamewon == 1)
		{
			State = sover;
		}
		else
		State = s11;
		break;
		
		case s11:
		ar[0][2] = 0;
		ar[0][3] = 1;
		ar[0][4] = 0;
		ar[1][2] = 4;
		ar[1][3] = 5;
		ar[1][4] = 6;
		rowsave1 = 1;
		counter900 = 2
		rowsave1pos = 0;
		piece1 = 3;
		piece2 = 2;
		piece3 = 3;
		piece4 = 4;
		State = s12;
		break;
		
		
		
		case s12:
		if(rowsave1pos == 0)
		{
			if((rowsave1 <= 6) && (piecemovement == 1) && (rowsave1 >=1))
			{
				if(ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1 + 1][piece4] == 0 && ar[rowsave1 + 1][piece2] == 0)
				{
					ar[rowsave1 - 1][piece1] = 0;
					ar[rowsave1][piece2] = 0;
					ar[rowsave1][piece3] = 0;
					ar[rowsave1][piece4] = 0;
					rowsave1++;
					ar[rowsave1 - 1][piece1] = 1;
					ar[rowsave1][piece2] = 4;
					ar[rowsave1][piece3] = 5;
					ar[rowsave1][piece4] = 6;
					State = s12;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		}
		else if(rowsave1pos == 1)
		{
			if((rowsave1 <= 5) && (piecemovement == 1) && (rowsave1 >= 1))
			{
				if(ar[rowsave1 + 2][piece4] == 0 && ar[rowsave1 + 1][piece1] == 0)
				{
					ar[rowsave1 - 1][piece2] = 0;
					ar[rowsave1][piece3] = 0;
					ar[rowsave1][piece1] = 0;
					ar[rowsave1 + 1][piece4] = 0;
					rowsave1++;
					ar[rowsave1 - 1][piece2] = 4;
					ar[rowsave1][piece3] = 5;
					ar[rowsave1][piece1] = 1;
					ar[rowsave1 + 1][piece4] = 6;
					State = s12;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		}
		else if(rowsave1pos == 2)
		{
			if((rowsave1 <= 5) && (piecemovement == 1) && (rowsave1 >= 1))
			{
				if(ar[rowsave1 + 1][piece4] == 0 && ar[rowsave1 + 1][piece2] == 0 && ar[rowsave1 + 2][piece3] == 0)
				{
					ar[rowsave1][piece3] = 0;
					ar[rowsave1][piece4] = 0;
					ar[rowsave1][piece2] = 0;
					ar[rowsave1 + 1][piece1] = 0;
					rowsave1++;
					ar[rowsave1 + 1][piece1] = 1;
					ar[rowsave1][piece2] = 4;
					ar[rowsave1][piece3] = 5;
					ar[rowsave1][piece4] = 6;
					State = s12;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		}
		else if(rowsave1pos == 3)
		{
			if((rowsave1 <= 5) && (piecemovement == 1) && (rowsave1 >= 1))
			{
				if(ar[rowsave1 + 2][piece3] == 0 && ar[rowsave1 + 1][piece1] == 0) 
				{
					ar[rowsave1 - 1][piece2] = 0;
					ar[rowsave1 + 1][piece4] = 0;
					ar[rowsave1][piece3] = 0;
					ar[rowsave1][piece1] = 0;
					rowsave1++;
					ar[rowsave1 + 1][piece2] = 4;
					ar[rowsave1 - 1][piece4] = 6;
					ar[rowsave1][piece3] = 5;
					ar[rowsave1][piece1] = 1;
					State = s12;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		
		}
		break;
		
		
		
		case s13:
		piecemovement = 1;
		ar[0][2] = 2;
		ar[0][3] = 4;
		ar[0][4] = 6;
		rowsave1 = 0;
		rowsave1pos = 0;
		piece2 = 2;
		piece3 = 3;
		piece4 = 4;
		piecenum++;
		if((ar[1][3] != 0) || (ar[1][4] != 0) || (ar[1][2] != 0)) gamewon = 1;
		if(gamewon == 1)
		{
			State = sover;
		}
		else
		State = s14;
		break;
		
		case s14:
		ar[0][2] = 3;
		ar[0][3] = 0;
		counter900 = 2
		ar[0][4] = 0;
		ar[1][2] = 2;
		ar[1][3] = 4;
		ar[1][4] = 6;
		rowsave1 = 1;
		rowsave1pos = 0;
		piece1 = 2;
		piece2 = 2;
		piece3 = 3;
		piece4 = 4;
		State = s15;
		break;
		
		
		case s15:
		if(rowsave1pos == 0)
		{
			if((rowsave1 <= 6) && (piecemovement == 1) && (rowsave1 >=1))
			{
				if(ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1 + 1][piece4] == 0 && ar[rowsave1 + 1][piece2] == 0)
				{
					ar[rowsave1 - 1][piece1] = 0;
					ar[rowsave1][piece2] = 0;
					ar[rowsave1][piece3] = 0;
					ar[rowsave1][piece4] = 0;
					rowsave1++;
					ar[rowsave1 - 1][piece1] = 3;
					ar[rowsave1][piece2] = 2;
					ar[rowsave1][piece3] = 4;
					ar[rowsave1][piece4] = 6;
					State = s15;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		}
		else if(rowsave1pos == 1)
		{
			if((rowsave1 <= 5) && (piecemovement == 1) && (rowsave1 >= 1))
			{
				if(ar[rowsave1 + 2][piece4] == 0 && ar[rowsave1][piece1] == 0)
				{
					ar[rowsave1 - 1][piece2] = 0;
					ar[rowsave1][piece3] = 0;
					ar[rowsave1 - 1][piece1] = 0;
					ar[rowsave1 + 1][piece4] = 0;
					rowsave1++;
					ar[rowsave1 - 1][piece2] = 2;
					ar[rowsave1][piece3] = 4;
					ar[rowsave1 - 1][piece1] = 3;
					ar[rowsave1 + 1][piece4] = 6;
					State = s15;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		}
		else if(rowsave1pos == 2)
		{
			if((rowsave1 <= 5) && (piecemovement == 1) && (rowsave1 >= 1))
			{
				if(ar[rowsave1 + 1][piece4] == 0 && ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1 + 2][piece1] == 0)
				{
					ar[rowsave1 + 1][piece1] = 0;
					ar[rowsave1][piece4] = 0;
					ar[rowsave1][piece2] = 0;
					ar[rowsave1][piece3] = 0;
					rowsave1++;
					ar[rowsave1 + 1][piece1] = 3;
					ar[rowsave1][piece2] = 2;
					ar[rowsave1][piece3] = 4;
					ar[rowsave1][piece4] = 6;
					State = s15;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		}
		else if(rowsave1pos == 3)
		{
			if((rowsave1 <= 5) && (piecemovement == 1) && (rowsave1 >= 2))
			{
				if(ar[rowsave1 + 2][piece2] == 0 && ar[rowsave1 + 2][piece1] == 0) 
				{
					ar[rowsave1 + 1][piece2] = 0;
					ar[rowsave1 + 1][piece1] = 0;
					ar[rowsave1][piece3] = 0;
					ar[rowsave1 - 1][piece4] = 0;
					rowsave1++;
					ar[rowsave1 + 1][piece2] = 2;
					ar[rowsave1 - 1][piece4] = 6;
					ar[rowsave1][piece3] = 4;
					ar[rowsave1 + 1][piece1] = 3;
					State = s15;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		
		}
		break;
		
		
		
		case s16:
		piecemovement = 1;
		ar[0][2] = 1;
		ar[0][3] = 3;
		ar[0][4] = 6;
		rowsave1 = 0;
		rowsave1pos = 0;
		piece2 = 2;
		piece3 = 3;
		piece4 = 4;
		piecenum++;
		if((ar[1][3] != 0) || (ar[1][4] != 0) || (ar[1][2] != 0)) gamewon = 1;
		if(gamewon == 1)
		{
			State = sover;
		}
		else
		State = s17;
		break;
		
		case s17:
		ar[0][2] = 0;
		ar[0][3] = 0;
		ar[0][4] = 4;
		ar[1][2] = 1;
		ar[1][3] = 3;
		ar[1][4] = 6;
		rowsave1 = 1;
		counter900 = 2;
		rowsave1pos = 0;
		piece1 = 4;
		piece2 = 2;
		piece3 = 3;
		piece4 = 4;
		State = s18;
		break;
		
		
		case s18:
		if(rowsave1pos == 0)
		{
			if((rowsave1 <= 6) && (piecemovement == 1) && (rowsave1 >=1))
			{
				if(ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1 + 1][piece4] == 0 && ar[rowsave1 + 1][piece2] == 0)
				{
					ar[rowsave1 - 1][piece1] = 0;
					ar[rowsave1][piece2] = 0;
					ar[rowsave1][piece3] = 0;
					ar[rowsave1][piece4] = 0;
					rowsave1++;
					ar[rowsave1 - 1][piece1] = 4;
					ar[rowsave1][piece2] = 1;
					ar[rowsave1][piece3] = 3;
					ar[rowsave1][piece4] = 6;
					State = s18;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		}
		else if(rowsave1pos == 1)
		{
			if((rowsave1 <= 5) && (piecemovement == 1) && (rowsave1 >= 1))
			{
				if(ar[rowsave1 + 2][piece4] == 0 && ar[rowsave1 + 2][piece1] == 0)
				{
					ar[rowsave1 - 1][piece2] = 0;
					ar[rowsave1][piece3] = 0;
					ar[rowsave1 + 1][piece1] = 0;
					ar[rowsave1 + 1][piece4] = 0;
					rowsave1++;
					ar[rowsave1 - 1][piece2] = 1;
					ar[rowsave1][piece3] = 3;
					ar[rowsave1 + 1][piece1] = 4;
					ar[rowsave1 + 1][piece4] = 6;
					State = s18;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		}
		else if(rowsave1pos == 2)
		{
			if((rowsave1 <= 5) && (piecemovement == 1) && (rowsave1 >= 1))
			{
				if(ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1 + 1][piece2] == 0 && ar[rowsave1 + 2][piece1] == 0)
				{
					ar[rowsave1 + 1][piece1] = 0;
					ar[rowsave1][piece4] = 0;
					ar[rowsave1][piece2] = 0;
					ar[rowsave1][piece3] = 0;
					rowsave1++;
					ar[rowsave1 + 1][piece1] = 4;
					ar[rowsave1][piece2] = 1;
					ar[rowsave1][piece3] = 3;
					ar[rowsave1][piece4] = 6;
					State = s18;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		}
		else if(rowsave1pos == 3)
		{
			if((rowsave1 <= 5) && (piecemovement == 1) && (rowsave1 >= 1))
			{
				if(ar[rowsave1][piece1] == 0 && ar[rowsave1 + 2][piece2] == 0) 
				{
					ar[rowsave1 + 1][piece2] = 0;
					ar[rowsave1 - 1][piece1] = 0;
					ar[rowsave1][piece3] = 0;
					ar[rowsave1 - 1][piece4] = 0;
					rowsave1++;
					ar[rowsave1 + 1][piece2] = 1;
					ar[rowsave1 - 1][piece4] = 6;
					ar[rowsave1][piece3] = 3;
					ar[rowsave1 - 1][piece1] = 4;
					State = s18;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		
		}
		break;
		
		
		case s19:
		piecemovement = 1;
		ar[0][2] = 2;
		ar[0][3] = 4;
		rowsave1 = 0;
		rowsave1pos = 0;
		piece3 = 3;
		piece4 = 2;
		piecenum++;
		if((ar[1][2] != 0) || (ar[1][3] != 0)) gamewon = 1;
		if(gamewon == 1)
		{
			State = sover;
		}
		else
		State = s20;
		break;
		
		case s20:
		ar[0][4] = 1;
		ar[0][5] = 5;
		ar[0][2] = 0;
		ar[0][3] = 0;
		ar[1][2] = 2;
		ar[1][3] = 4;
		counter900 = 2;
		rowsave1 = 1;
		rowsave1pos = 0;
		piece1 = 5;
		piece2 = 4;
		piece3 = 3;
		piece4 = 2;
		State = s21;
		break;
		
		
		case s21:
		if(rowsave1pos == 0)
		{
			if((rowsave1 != 7) && (piecemovement == 1))
			{
				if(ar[rowsave1][piece1] == 0 && ar[rowsave1][piece2] == 0 && ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1 + 1][piece4] == 0)
				{
					ar[rowsave1 - 1][piece1] = 0;
					ar[rowsave1 - 1][piece2] = 0;
					ar[rowsave1][piece3] = 0;
					ar[rowsave1][piece4] = 0;
					rowsave1++;
					ar[rowsave1 - 1][piece1] = 5;
					ar[rowsave1 - 1][piece2] = 1;
					ar[rowsave1][piece3] = 4;
					ar[rowsave1][piece4] = 2;
					State = s21;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		}
		else if(rowsave1pos == 1)
		{
			if(rowsave1 <= 4 && rowsave1 >= 1 && (piecemovement == 1))
			{
				if((ar[rowsave1 + 1][piece3] == 0) && (ar[rowsave1 + 3][piece1] == 0))
				{
					ar[rowsave1 + 2][piece1] = 0;
					ar[rowsave1 + 1][piece2] = 0;
					ar[rowsave1][piece3] = 0;
					ar[rowsave1 - 1][piece4] = 0;
					rowsave1++;
					ar[rowsave1 + 2][piece1] = 5;
					ar[rowsave1 + 1][piece2] = 1;
					ar[rowsave1][piece3] = 4;
					ar[rowsave1 - 1][piece4] = 2;
					State = s21;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		}
		else if(rowsave1pos == 2)
		{
			if(rowsave1 <= 5 &&  rowsave1 >= 1 && (piecemovement == 1))
			{
				if(ar[rowsave1 + 1][piece4] == 0 && ar[rowsave1 + 1][piece3] == 0 && ar[rowsave1 + 2][piece2] == 0 && 
				ar[rowsave1 + 2][piece1] == 0)
				{
					ar[rowsave1 + 1][piece1] = 0;
					ar[rowsave1 + 1][piece2] = 0;
					ar[rowsave1][piece3] = 0;
					ar[rowsave1][piece4] = 0;
					rowsave1++;
					ar[rowsave1 + 1][piece1] = 5;
					ar[rowsave1 + 1][piece2] = 1;
					ar[rowsave1][piece3] = 4;
					ar[rowsave1][piece4] = 2;
					State = s21;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		}
		else if(rowsave1pos == 3)
		{
			if(rowsave1 <= 5 && rowsave1 >= 2 && (piecemovement == 1))
			{
				if((ar[rowsave1 + 2][piece4] == 0) && (ar[rowsave1][piece2] == 0))
				{
					ar[rowsave1 - 2][piece1] = 0;
					ar[rowsave1 - 1][piece2] = 0;
					ar[rowsave1][piece3] = 0;
					ar[rowsave1 + 1][piece4] = 0;
					rowsave1++;
					ar[rowsave1 - 2][piece1] = 5;
					ar[rowsave1 - 1][piece2] = 1;
					ar[rowsave1][piece3] = 4;
					ar[rowsave1 + 1][piece4] = 2;
					State = s21;
				}
				else
				{
					piecemovement = 0;
					State = s0;
				}
			}
			else
			{
				piecemovement = 0;
				State = s0;
			}
		}
		break;

		
}
	switch(State)
	{case Init:	break;
		case s0:
		break;
		case s1:
		break;
		case s3:
		break;
		case sover:
		break;
		default:
		break;
	} // State actions
}


enum Statesdone { Initdone, s0done, s1done} Statedone;
void Tick3() // does 8 horiz and 4 vertical removal and move downs
{
    switch(Statedone)
    {
        case Initdone:
        rowclearcheck = 10;
        rowclearcheck2 = 10;
        columnclearcheck = 10;
        if(piecemovement == 0)
        {
            rowclearcheck = 10;
            rowclearcheck = 10;
            columnclearcheck = 10;
            for(int i = 0; i < 8 ; i++)
        	{
        	    rowclearcheck2 = 10;
        		for(int j = 0; j < 8; j++)
        		{
        			if(ar[i][j] == 0)
        			rowclearcheck2 = -1;
        		}
        		if(rowclearcheck2 == 10)
        		rowclearcheck = i;
        	}
        	for(int i = 0; i <= 4 ; i++)
        	{
        		for(int j = 0; j < 8; j++)
        		{
        			if((ar[i][j] == 1) && (ar[i + 1][j] == 1) && (ar[i + 2][j] == 1) && (ar[i + 3][j] == 1))
        			{
        			    columnclearcheck = i + 3;
        			    columnclearcheck2 = j;
        			}
        			if((ar[i][j] == 2) && (ar[i + 1][j] == 2) && (ar[i + 2][j] == 2) && (ar[i + 3][j] == 2))
        			{
        			    columnclearcheck = i + 3;
        			    columnclearcheck2 = j;
        			}
        			if((ar[i][j] == 3) && (ar[i + 1][j] == 3) && (ar[i + 2][j] == 3) && (ar[i + 3][j] == 3))
        			{
        			    columnclearcheck = i + 3;
        			    columnclearcheck2 = j;
        			}
        			if((ar[i][j] == 4) && (ar[i + 1][j] == 4) && (ar[i + 2][j] == 4) && (ar[i + 3][j] == 4))
        			{
        			    columnclearcheck = i + 3;
        			    columnclearcheck2 = j;
        			}
        			if((ar[i][j] == 5) && (ar[i + 1][j] == 5) && (ar[i + 2][j] == 5) && (ar[i + 3][j] == 5))
        			{
        			    columnclearcheck = i + 3;
        			    columnclearcheck2 = j;
        			}
        			if((ar[i][j] == 6) && (ar[i + 1][j] == 6) && (ar[i + 2][j] == 6) && (ar[i + 3][j] == 6))
        			{
        			    columnclearcheck = i + 3;
        			    columnclearcheck2 = j;
        			}
        		}
        	}  
        }
    	if(rowclearcheck != 10)
    	Statedone = s0done;
    	else if(columnclearcheck != 10)
    	Statedone = s1done;
    	else
    	Statedone = Initdone;
    	break;
    	
    	case s0done:
        for(int i = rowclearcheck; i >= 1 ; i--)
    	{
    		for(int j = 7; j >= 0; j--)
    		{
    			ar[i][j] = ar[i - 1][j];
    		}
    	}
    	for(int i = 0; i < 7; i++)
    	{
    		
    	}
    	for(int j = 0; j < 8; j++)
		{
			ar[0][j] = 0;
		}
		score++;
		check = 1;
		check2 = 1;
    	Statedone = Initdone;
    	break;
    	
    	case s1done:
    	piecemovement = 0;
    	ar[columnclearcheck][columnclearcheck2] = 0;
    	ar[columnclearcheck - 1][columnclearcheck2] = 0;
    	ar[columnclearcheck - 2][columnclearcheck2] = 0;
    	ar[columnclearcheck - 3][columnclearcheck2] = 0;
        for(int i = columnclearcheck; i >= 0 ; i--)
    	{
    	    if((i - 4) >= 0)
    		ar[i][columnclearcheck2] = ar[i - 4][columnclearcheck2];
    	}
    	score++;
    	check = 1;
    	check2 = 1;
    	Statedone = Initdone;
    	break;
    }
}


enum Statesdone4 { Initdone4} Statedone4;
void Tick4()   // LCD score
{
    switch(Statedone4)
    {
        case Initdone4:
        LCD_ClearScreen(); 
        if(score == 0) { LCD_DisplayString(1, "Score: 0        ");}
        if(score == 1) { LCD_DisplayString(1, "Score: 1        ");}
        if(score == 2) { LCD_DisplayString(1, "Score: 2        ");}
        if(score == 3) { LCD_DisplayString(1, "Score: 3        ");}
        if(score == 4) { LCD_DisplayString(1, "Score: 4        ");}
        if(score == 5) { LCD_DisplayString(1, "Score: 5        ");}
        if(score == 6) { LCD_DisplayString(1, "Score: 6        ");}
        if(score == 7) { LCD_DisplayString(1, "Score: 7        ");}
        if(score == 8) { LCD_DisplayString(1, "Score: 8        ");}
        if(score == 9) { LCD_DisplayString(1, "Score: 9        ");}
        if(score == 10) { LCD_DisplayString(1, "Score: 10        ");}
        if(score == 11) { LCD_DisplayString(1, "Score: 11        ");}
        if(score == 12) { LCD_DisplayString(1, "Score: 12        ");}
        if(score == 13) { LCD_DisplayString(1, "Score: 13        ");}
        if(score == 14) { LCD_DisplayString(1, "Score: 14        ");}
        if(score == 15) { LCD_DisplayString(1, "Score: 15        ");}
        if(score == 16) { LCD_DisplayString(1, "Score: 16        ");}
        if(score == 17) { LCD_DisplayString(1, "Score: 17        ");}
        if(score == 18) { LCD_DisplayString(1, "Score: 18        ");}
        if(score == 19) { LCD_DisplayString(1, "Score: 19        ");}
        if(score == 20) { LCD_DisplayString(1, "Score: 20        ");}
        if(score == 21) { LCD_DisplayString(1, "Score: 21        ");}
        if(score == 22) { LCD_DisplayString(1, "Score: 22        ");}
        if(score == 23) { LCD_DisplayString(1, "Score: 23        ");}
        if(score == 24) { LCD_DisplayString(1, "Score: 24        ");}
        if(score == 25) { LCD_DisplayString(1, "Score: 25        ");}
        if(score == 26) { LCD_DisplayString(1, "Score: 26        ");}
        if(score == 27) { LCD_DisplayString(1, "Score: 27        ");}
        if(score == 28) { LCD_DisplayString(1, "Score: 28        ");}
        if(score == 29) { LCD_DisplayString(1, "Score: 29        ");}
        if(score == 30) { LCD_DisplayString(1, "Score: 30        ");}
        if(score == 31) { LCD_DisplayString(1, "Score: 31        ");}
        if(score == 32) { LCD_DisplayString(1, "Score: 32        ");}
        if(score == 33) { LCD_DisplayString(1, "Score: 33        ");}
        if(score == 34) { LCD_DisplayString(1, "Score: 34        ");}
        if(score == 35) { LCD_DisplayString(1, "Score: 35        ");}
        if(score == 36) { LCD_DisplayString(1, "Score: 36        ");}
        if(score == 37) { LCD_DisplayString(1, "Score: 37        ");}
        if(score == 38) { LCD_DisplayString(1, "Score: 38        ");}
        if(score == 39) { LCD_DisplayString(1, "Score: 39        ");}
        if(score == 40) { LCD_DisplayString(1, "Score: 40        ");}
        if(score == 41) { LCD_DisplayString(1, "Score: 41        ");}
        if(score == 42) { LCD_DisplayString(1, "Score: 42        ");}
        if(score == 43) { LCD_DisplayString(1, "Score: 43        ");}
        if(score == 44) { LCD_DisplayString(1, "Score: 44        ");}
        if(score == 45) { LCD_DisplayString(1, "Score: 45        ");}
        if(score == 46) { LCD_DisplayString(1, "Score: 46        ");}
        if(score == 47) { LCD_DisplayString(1, "Score: 47        ");}
        if(score == 48) { LCD_DisplayString(1, "Score: 48        ");}
        if(score == 49) { LCD_DisplayString(1, "Score: 49        ");}
        if(score == 50) { LCD_DisplayString(1, "Score: 50        ");}
    	Statedone4 = Statedone4;
    	break;
    }
}



enum Statesdone5 { Initdone5, s100, s101, s102, s103, s120, s121, s122, s123,s124, s125,s126, s104, s105, s106, s107, s108, s109} Statedone5;
void Tick5()   
{
    switch(Statedone5)
    {
        case Initdone5:
        if(score % 4 == 0 && score != 0 && check == 1)
        {
        	for(int i = 0; i < 8; i++)
			{
				for(int j = 0; j < 8; j++)
				{
					if(ar[i][j] != 0)
					{
						if(temp90 > i)
						{
							temp90 = i;
						}
						if(temp91 < i)
						{
							temp91 = i;
						}
					}
				}
			}
			if(temp90 != temp91)
			{
				for(int i = 0; i < 8; i++)
				{
					temp92 = ar[temp90][i];
					ar[temp90][i] = ar[temp91][i];
					ar[temp91][i] = temp92;
				}
			}
        }
        Statedone5 = Initdone5;
        check = 0;
        if(score % 7 == 0 && score != 0 && check2 == 1)
        {
        	Statedone5 = s100;
        	pause = 1;
        }
    	break;
    	
    	case s100:
    	checkcounter = 1;
    	LCD_ClearScreen();
    	LCD_DisplayString(1, "Please enter a number");
    	Statedone5 = s101;
    	break;
    	case s101:
    	LCD_ClearScreen();
    	LCD_DisplayString(1, "Please enter a number");
    	Statedone5 = s102;
    	break;
    	case s102:
    	LCD_ClearScreen();
    	LCD_DisplayString(1, "Please enter a number");
    	Statedone5 = s103;
    	break;
    	case s103:
    	LCD_ClearScreen();
    	LCD_DisplayString(1, "Please enter a number");
    	Statedone5 = s120;
    	break;
    	case s120:
    	LCD_ClearScreen();
    	LCD_DisplayString(1, "Please enter a number");
    	Statedone5 = s121;
    	break;
    	case s121:
    	LCD_ClearScreen();
    	LCD_DisplayString(1, "Please enter a number");
    	Statedone5 = s122;
    	break;
    	case s122:
    	LCD_ClearScreen();
    	LCD_DisplayString(1, "Please enter a number");
    	Statedone5 = s123;
    	break;
    	case s123:
    	LCD_ClearScreen();
    	LCD_DisplayString(1, "Please enter a number");
    	Statedone5 = s124;
    	break;
    	case s124:
    	LCD_ClearScreen();
    	LCD_DisplayString(1, "Please enter a number");
    	Statedone5 = s125;
    	break;
    	case s125:
    	LCD_ClearScreen();
    	LCD_DisplayString(1, "Please enter a number");
    	Statedone5 = s104;
    	break;
    	case s104:
    	LCD_ClearScreen();
    	LCD_DisplayString(1, "Please enter a number");
    	Statedone5 = s105;
    	break;
    	case s105:
    	LCD_ClearScreen();
    	LCD_DisplayString(1, "Please enter a number");
    	Statedone5 = s106;
    	break;
    	case s106:
    	LCD_ClearScreen();
    	LCD_DisplayString(1, "1");
    	Statedone5 = s107;
    	break;
    	
    	case s107:
    	if(GetBit(PINA,2) == 0)
    	{
    		Statedone5 = s108;
    	}
    	else if(temp2 == 1)
    	{
    		if(score % checkcounter == 0)
    		{
    			for(int i = 0; i < 8; i++)
				{
					for(int j = 0; j < 8; j++)
					{
						ar[i][j] = 0;
					}
				}
				LCD_ClearScreen();
				LCD_DisplayString(1, "Picked Right!");
    		}
    		else
    		{
    			LCD_ClearScreen();
    			LCD_DisplayString(1, "Failed!");
    		}
    		Statedone5 = Initdone5;
    		check2 = 0;
    		pause = 0;
    	}
    	else
    	Statedone5 = s107;
    	break;
    	
    	case s108:
    	if(GetBit(PINA,2) == 0)
    	{
    		checkcounter++;
    		if(checkcounter == 2) { LCD_DisplayString(1, "2");}
    		if(checkcounter == 3) { LCD_DisplayString(1, "3");}
    		if(checkcounter == 4) { LCD_DisplayString(1, "4");}
    		if(checkcounter == 5) { LCD_DisplayString(1, "5");}
    		if(checkcounter == 6) { LCD_DisplayString(1, "6");}
    		if(checkcounter == 7) { LCD_DisplayString(1, "7");}
    		if(checkcounter == 8) { LCD_DisplayString(1, "8");}
    		if(checkcounter == 9) { LCD_DisplayString(1, "9");}
    		Statedone5 = s109;
    	}
    	else
    	{
    		Statedone5 = s108;
    	}
    	break;
    	
    	
    	case s109:
    	if(GetBit(PINA,2) == 0)
    	{
    		Statedone5 = s109;
    	}
    	else
    	{
    	    Statedone5 = s107;
    	}
    	break;
    	
    	
    }
}


//tick is to read joystick
// Tick is to generate and move piece down
// Tick1 is to rotate piece
//Tick2 is to move piece left right, up down
//Tick3 is to clear bottom rows and vertical and shift down
// Tick4 is to move rows down after clear vertical
int main(void)
{
	DDRA = 0xF0; PORTA = 0x0F;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	// intermediate variable used for port updates
	/* Replace with your application code */
	LCD_init();
	LCD_ClearScreen();
	TimerSet(1);
	TimerOn();
	state = start;
	State = Init;
	State1 = Init1;
    State2 = Init2;
    Statedone = Initdone;
    Statedone4 = Initdone4;
    Statedone5 = Initdone5;
    srand(seed);
	InitADC(); // down 2 up 1 left 8, right 4
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			ar[i][j] = 0;
		}
	}
	setGround();
	int counter = 0;
	int counter1 = 0;
	int counter2 = 0;
	int counter3 = 0;
	int counter4 = 0;
	int counter5 = 0;
	int counter6 = 0;
	while (1)
	{	
		counter++;
		counter1++;
		counter2++;
		counter3++;
		counter4++;
		counter5++;
		counter6++;
		if(temp2 == 2 || temp2 == 1 || temp2 == 8 || temp2 == 4)
		{
			seed += 1;
			srand(seed);
		}
		if(GetBit(PINA,3) == 0)
		{
			seed = 5;
			srand(seed);
			counter = 0;
			counter1 = 0;
			counter2 = 0;
			counter3 = 0;
			counter4 = 0;
			counter5 = 0;
			counter6 = 0;
			for(int i = 0; i < 8; i++)
			{
				for(int j = 0; j < 8; j++)
				{
					ar[i][j] = 0;
				}
			}
			tmp = 0x00;
			ground = 0xFF;
			blue = 0x00;
			green = 0x00;
			red = 0x00;
			violet = 0x00;
			rowStart = 0;
			groundStart = 0x00;
			ar[8][8]; // 1-blue, 2-red, 3-green
			gamewon = 0; // see if game is done
			piecemovement = 0; // see if we're done moving piece;
			piecenum = 0;
			rowsave1 = 0;
			piece1 = 0;
			piece2 = 0;
			piece3 = 0;
			piece4 = 0;
			rowsave1pos = 0;
			rowclearcheck = 1;
			rowclearcheck2 = 1;
			score = 0;
			columnclearcheck = 10;
			columnclearcheck2 = 10;
			temp90 = 100;
			temp91 = -1;
			temp92 = -1;
			check = 0;
			check2 = 0;
			pause = 0;
			checkcounter = 0;
			state = start;
			State = Init;
			State1 = Init1;
		    State2 = Init2;
		    Statedone = Initdone;
		    Statedone4 = Initdone4;
		    Statedone5 = Initdone5;
		}
		if(counter1 >= 5) // joystick read - tick(0)
		{
			seed += 1;
			srand(seed);
			counter1 = 0;
			tick();            // joystick read
			setGround(); 
		}
		if(counter2 >= 25 && pause == 0 && counter900 >= 2)  // rotate pieces Tick1()
		{
			seed += 1;
			srand(seed);
			counter2 = 0;
			Tick1();         // rotate piece
			setGround();
		}
		if(counter3 >= 25 && pause == 0 && counter900 >= 2) // move pieces Tick2()
		{
			seed += 1;
			srand(seed);
			counter3 = 0;
			Tick2();  //   moving pieces
			setGround();
		}
		if(counter4 >= 150 && pause == 0) // remove rows Tick3()
		{
			seed += 1;
			srand(seed);
			counter4 = 0;
		    Tick3();    
			setGround();
		}
		if(counter5 >= 200 && pause == 0) // Lcd score
		{
			seed += 1;
			srand(seed);
			counter5 = 0;
		    Tick4();    
			setGround();
		}
		if(counter6 >= 150) // Lcd
		{
			seed += 1;
			srand(seed);
			counter6 = 0;
		    Tick5();    
			setGround();
		}
		if(counter >= 700 && pause == 0)  // generate pieces Tick() 
		{
			seed += 1;
			srand(seed);
		    counter = 0;
			Tick();			 // move piece down and/or generate piece
		}
		setGround();
		while(!TimerFlag);
		TimerFlag = 0;
	}
}
