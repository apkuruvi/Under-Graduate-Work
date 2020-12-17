/*	Partner(s) Name & E-mail: Abraham Kuruvila akuru003@ucr.edu 
 *	Lab Section: 21
 *	Assignment: CS 179J Senior Design Project
 *	
 *
 *                     TEST CASES AT THE BOTTOM
 *
 *
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <stdint.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdbool.h> 
#include <string.h> 
#include <math.h> 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <avr/eeprom.h> 
#include <avr/portpins.h> 
#include <usart_ATmega1284.h>
#include <avr/pgmspace.h> 
									//FreeRTOS include files 
#include "FreeRTOS.h" 
#include "task.h" 
#include "croutine.h" 
int dispense = 0;           // user wants to dispense food
int lock_status = 0;        // user wants to lock or unlock
int fan_status = 0;         // user wants to turn on or off the fan
int lock_status1 = 0;             // 0 door is unlocked - used by the microcontroller to know if locked or unlocked
int counter = 0;                  // counters used to keep track of iterations for servo motors
int counter1 = 0;
int counter2 = 0;
unsigned short temp = 0x0000;                //Recicving bluetooth signal from phone via usart

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b)      // Bit-access function
{ 
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) 
{
	return ((x & (0x01 << k)) != 0);
}

																// initializers
enum State3 {Init_fan, Fan_on, Fan_off} state3;
void Fan_Start(
{
	state3 = Init_fan;
}
                             
enum State2 {Init_dispenser, Dispenseopen_sethigh, Dispenseopen_keephigh, Dispenseopen_setlow,Dispenseclose_sethigh,Dispenseclose_setlow,Dispense_wait,Dispense_waitmore} state2;
void Dispenser_Start()
{
	state2 = Init_dispenser;
}

enum State1 {Init_lock, UnLockdoor_sethigh, UnLockdoor_keephigh,UnLockdoor_setlow, Lockdoor_sethigh,Lockdoor_setlow} state1;
void LockDoor_Start()
{
	state1 = Init_lock;
}

enum State {start,waitlock, waitfan, waitdispense} state;
void SelectOption_Start()
{
	 state = start;
}

													// SM
void Fan1()
{
	switch(state3)
	{
		case Init_fan:
		if(fan_status == 1)                   // if user sent command to change fan status
		{
			              // if fan is off, turn it on
			state3 = Fan_on;
		}
		else if(fan_status == 2)
		{
			state3 = Fan_off;             // turn fan off 
		}
		else
		{
			state3 = Init_fan;               // else stay in this state and loop waiting for fan on/off signal
		}
		break;
		
		case Fan_on:
		PORTB =  0xFF;                          // turn fan on- high signal sent to Roger to signal fan on
		state3 = Init_fan;                      // go back to waiting state
		fan_status = 0;                        // microcontroller acknowledges that fan is on
		break;
		
		case Fan_off:								// turn fan off- low signal sent to Roger to signal fan off
		PORTB =  0x00;                           // turn fan off
		state3 = Init_fan;                       // go back to waitng state
		fan_status = 0;                         // microcontroller acknowledges that fan is off
		break;
		
		default:
		state3 = Init_fan;
		break;
	}
}

void Dispenser1()
{
	switch(state2)
	{
		case Init_dispenser:
		if(dispense == 1)                 // if user set request to dispense, go to dispense state
		{
			state2 = Dispenseopen_sethigh;
			counter = 0;
			counter1 = 0;
			counter2 = 0;
		}
		else
		{
			state2 = Init_dispenser;
		}
		break;
		
		case Dispenseopen_sethigh:       // set port a high
		PORTC = 0xFF;
		state2 = Dispenseopen_keephigh;
		break;
		
		case Dispenseopen_keephigh:
		state2 = Dispenseopen_setlow;        // keep high
		break;
		
		case Dispenseopen_setlow:                   // set low and keep low for 18 ms, repeat 10 times
		PORTC = 0x00;                               // this will open the dispenser
		counter++;                             
		if(counter >= 18)
		{
			counter = 0;
			counter2++;
			if(counter2 < 10)           // need to do 10 times or servo won't move all the way
			{
				state2 = Dispenseopen_sethigh;	
			}
			else
			{
				counter2 = 0;
				state2 = Dispense_wait;
			}
		}
		break;

		case Dispense_wait:                    // we opened dispenser, wait 2 seconds for food to come out
		counter1++;
		if(counter1 > 2000)
		{
			counter1 = 0;
			state2 = Dispenseclose_sethigh;               // need to close the dispeneser
		}
		break;
		
		case Dispenseclose_sethigh:                 // set port a high for 1 ms
		PORTC = 0xFF;
		state2 = Dispenseclose_setlow;
		break;
		
		
		case Dispenseclose_setlow:
		PORTC = 0x00;
		counter++;                                // set port a low for 19 ms repeating the high and low setting 10 times
		if(counter >= 19)                         // this will close the dispenser
		{
			counter = 0;
			counter2++;
			if(counter2 < 10)             //need to do 10 times or servo won't move all the way
			{
				state2 = Dispenseclose_sethigh;	
			}
			else
			{
				state2 = Dispense_waitmore;              
				counter2 = 0;
			}
		}
		break;
		
		case Dispense_waitmore:
		counter1++;                          // wait one second before going back to init state
		if(counter1 > 1000)
		{
			dispense = 0;
			counter1 = 0;
			state2 = Init_dispenser;
		}
		break;
		
		default:
		state2 = Init_dispenser;
		break;
	}
}

void LockDoor1()
{
	switch(state1)
	{
		case Init_lock:
		if(lock_status == 2)                          // if user set request to lock door
		{
			                             // if door is unlocked, go to state to lock
			state1 = Lockdoor_sethigh;
			lock_status1 = 1;
		}
		else if (lock_status == 1)
		{
			lock_status1 = 0;
				state1 = UnLockdoor_sethigh;					// else door is locked and go to state to unlock
		}
		else
		{
			PORTA = lock_status1 ? 0xF7 : 0;
			state1 = Init_lock;   
		}                             
		break;
		
		case UnLockdoor_sethigh:                           // set port a high
		PORTA = 0x01;
		state1 = UnLockdoor_keephigh;
		break;
		
		case UnLockdoor_keephigh:                          // need two ms of port a high
		state1 = UnLockdoor_setlow;
		break; 
		
		case UnLockdoor_setlow:
		PORTA = 0x00;                       // set port a low for 18 ms, need to repeat setting high and low 10 times
		counter++;                           // this unlocks the door
		if(counter >= 18)      // setting low for 18 ms
		{
			counter = 0;
			counter1++;
			if(counter1 < 10)             // set high and low 10 times, need to do 10 times or servo won't move all the way
			{
				state1 = UnLockdoor_sethigh;		
			}
			else
			{
				counter1 = 0;
				lock_status = 0;
				state1 = Init_lock;                // go to starting state when done
			}
		}
		break;
		
		case Lockdoor_sethigh:                // set port a high for one second 
		PORTA = 0x01;
		state1 = Lockdoor_setlow;
		break;
		
		case Lockdoor_setlow:
		PORTA = 0x00;          // set low for 19 ms need to do this 10 times
		counter++;
		if(counter >= 19)         // do 19 times of setting low
		{
			counter = 0;
			counter1++;
			if(counter1 < 10)             //need to do 10 times or servo won't move all the way
			{ 
				state1 = Lockdoor_sethigh;	
			}
			else
			{
				state1 = Init_lock;          // go back to starting state
				lock_status = 0;
				counter1 = 0;
			}
		}
		break;
		
		default:
		state1 = Init_lock;
		break;
	}
}
void SelectOption1()
{
	switch(state)
	{
		case start:
		if(USART_HasReceived(0))           // check input recieved from bluetooth
		{
			temp = USART_Receive(0);
			if(temp == '1')                   // unlock door
			{
				lock_status = 1;
				state = waitlock;
			}
			if((temp == '2') || (temp == '6'))
			{
				lock_status = 2;                    // lock door
				state = waitlock;
			}
			if(temp == '3')
			{                       // fan on
				fan_status = 1;
				state = waitfan;
			}
			if(temp == '4')             // fan off
			{
				fan_status = 2;
				state = waitfan;
			}
			if(temp == '5')            // dispense food
			{
				dispense = 1;
				state = waitdispense;
			}
			else
			{
				state = start;
			}
			USART_Flush(0);      //clear usart buffer
		}
		
		break;
	
		case waitlock:
		if(lock_status == 0)  //waits for door to lock/unlock before going back to start to get new inputs from bluetooth
		{
			state = start;
		}
		else
		{
			state = waitlock;
		}
		break; 
		
		case waitfan:        //waits for fan to turn off/on before going back to start to get new inputs from bluetooth
		if(fan_status == 0)
		{
			state = start;
		}
		else
		{
			state = waitfan;
		}
		break;
		
		case waitdispense:   //waits for food to dispense before going back to start to get new inputs from bluetooth
		if(dispense == 0)
		{
			state = start;
		}
		else
		{
			state = waitdispense;
		}
		break;
		
		default:
		state = start;
		break;
	}
}


												// Loopers-setting periods
void SelectOption()
{
	SelectOption_Start();
   for(;;) 
   { 	
	SelectOption1();
	vTaskDelay(200); 
   } 
}

void LockDoor()
{
	LockDoor_Start();
	for(;;)
	{
		LockDoor1();
		vTaskDelay(1);
	}
}

void Dispenser()
{
	Dispenser_Start();
	for(;;)
	{
		Dispenser1();
		vTaskDelay(1);
	}
}

void Fan()
{
	Fan_Start();
	for(;;)
	{
		Fan1();
		vTaskDelay(1);
	}
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(SelectOption, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(LockDoor, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(Dispenser, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(Fan, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}	
 
int main(void) 
{ 
   DDRA = 0xFF; PORTA = 0x00;
   DDRB = 0xFF; PORTB = 0x00;
   DDRC = 0xFF; PORTC = 0x00;
   DDRD = 0xFF; PORTD = 0x00;
  
   initUSART(0);
											 //Start Tasks  
   StartSecPulse(1);
											 //RunSchedular 
   vTaskStartScheduler(); 
   return 0; 
}

// TEST CASES
/*
Because I can only test the servo motor by actually using it, I attached buttons onto PORTC.
When the apporiate button is pressed, the program should send the proper signal to have the
motor move left or right (unlock/lock). There are also buttons for turning the fan on and off
as I just set an output pin high. I used an LED  on PORTD that would light up if fan button was
pressed. It would turn off if the fan button was pressed again.
*/

/*
#include <stdint.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdbool.h> 
#include <string.h> 
#include <math.h> 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <avr/eeprom.h> 
#include <avr/portpins.h> 
#include <usart_ATmega1284.h>
#include <avr/pgmspace.h> 
//FreeRTOS include files 
#include "FreeRTOS.h" 
#include "task.h" 
#include "croutine.h" 
int dispense = 0;           // user wants to dispense food
int lock_status = 0;        // user wants to lock or unlock
int fan_status = 0;         // user wants to turn on or off the fan
int lock_status1 = 0;             // 0 door is unlocked - used by the microcontroller to know if locked or unlocked
int fan_status1 = 0;              // fan is off or on used by the microcontroller only
int counter = 0;                  // counters used to keep track of iterations for servo motors
int counter1 = 0;
int counter2 = 0;
// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

// initializers
enum State3 {Init_fan, Fan_on, Fan_off} state3;
void Fan_Start(){
	state3 = Init_fan;
}
                               ,L4,L5
enum State2 {Init_dispenser, Dispenseopen_sethigh, Dispenseopen_keephigh, Dispenseopen_setlow,Dispenseclose_sethigh,Dispenseclose_setlow,Dispense_wait,Dispense_waitmore} state2;
void Dispenser_Start(){
	state2 = Init_dispenser;
}

enum State1 {Init_lock, UnLockdoor_sethigh, UnLockdoor_keephigh,UnLockdoor_setlow, Lockdoor_sethigh,Lockdoor_setlow} state1;
void LockDoor_Start(){
	state1 = Init_lock;
}

enum State {start, selection1,selection2,selection3} state;
void SelectOption_Start(){
	 state = start;
}

// SM
void Fan1()
{
	switch(state3)
	{
		case Init_fan:
		if(fan_status == 1)                   // if user sent command to change fan status
		{
			if(fan_status1 == 0)              // if fan is off, turn it on
			{
				state3 = Fan_on;
			}
			else
			{
				state3 = Fan_off;             // turn fan off 
			}
			fan_status = 0;
		}
		break;
		
		case Fan_on:
		PORTD =  0xFF;                          // turn fan on
		state3 = Init_fan;                      // go back to waiting state
		fan_status1 = 1;                        // microcontroller acknowledges that fan is on
		break;
		
		case Fan_off:
		PORTD =  0x00;                           // turn fan off
		state3 = Init_fan;                       // go back to waitng state
		fan_status1 = 0;                         // microcontroller acknowledges that fan is off
		break;
		
		default:
		state3 = Init_fan;
		break;
	}
}

void Dispenser1()
{
	switch(state2)
	{
		case Init_dispenser:
		if(dispense == 1)                 // if user set request to dispense, go to dispense state
		{
			state2 = Dispenseopen_sethigh;
			counter = 0;
			counter1 = 0;
			counter2 = 0;
			dispense = 0;
		}
		else
		{
			state2 = Init_dispenser;
		}
		break;
		
		case Dispenseopen_sethigh:       // set port a high
		PORTA = 0x02;
		state2 = Dispense_keephigh;
		break;
		
		case Dispenseopen_keephigh:
		state2 = Dispense_setlow;        // keep high
		break;
		
		case Dispenseopen_setlow:                   // set low and keep low for 18 ms, repeat 10 times
		PORTA = 0x00;
		counter++;
		if(counter >= 18)
		{
			counter = 0;
			counter2++;
			if(counter2 < 10)
			state2 = Dispenseopen_sethigh;
			else
			{
				counter2 = 0;
				state2 = Dispense_wait;
			}
		}
		break;

		case Dispense_wait:                    // we opened dispenser, wait 2 seconds for food to come out
		counter1++;
		if(counter1 > 2000)
		{
			counter1 = 0;
			state2 = Dispenseclose_sethigh;               // need to close the dispeneser
		}
		break;
		
		case Dispenseclose_sethigh:                 // set port a high for 1 ms
		PORTA = 0x02;
		state2 = Dispenseclose_setlow;
		break;
		
		
		case Dispenseclose_setlow:
		PORTA = 0x00;
		counter++;                                // set port a low for 19 ms repeating the high and low setting 10 times
		if(counter >= 19)
		{
			counter = 0;
			counter2++;
			if(counter2 < 10)
			state2 = Dispenseclose_sethigh;
			else
			{
				state2 = Dispense_waitmore;              
				counter2 = 0;
			}
		}
		break;
		
		case Dispense_waitmore:
		counter1++;                          // wait one second before going back to init state
		if(counter1 > 1000)
		{
			counter1 = 0;
			state2 = Init_dispenser;
		}
		break;
		
		default:
		state2 = Init_dispenser;
		break;
	}
}

void LockDoor1()
{
	switch(state1)
	{
		case Init_lock:
		if(lock_status == 1)                            // if user set request to lock door
		{
			if(lock_status1 == 0)                      // if door is unlocked, go to state to lock
			{
				state1 = Lockdoor_sethigh;
			}
			else
			{
				state1 = UnLockdoor_sethigh;					// else door is locked and go to state to unlock
			}
			lock_status = 0;
		}
		else
		state1 = Init_lock;                                
		break;
		
		case UnLockdoor_sethigh:                           // set port a high
		PORTA = 0x01;
		state1 = UnLockdoor_keephigh;
		break;
		
		case UnLockdoor_keephigh:                          // need two ms of port a high
		state1 = UnLockdoor_setlow;
		break; 
		
		case UnLockdoor_setlow:
		PORTA = 0x00;                       // set port a low for 18 ms, need to repeat setting high and low 10 times
		counter++;
		if(counter >= 18)      // setting low for 18 ms
		{
			counter = 0;
			counter1++;
			if(counter1 < 10)                 // set high and low 10 times
			state1 = UnLockdoor_sethigh;
			else
			{
				counter1 = 0;
				lock_status1 = 0;
				state1 = Init_lock;                // go to starting state when done
			}
		}
		break;
		
		case Lockdoor_sethigh:                // set port a high for one second 
		PORTA = 0x01;
		state1 = Lockdoor_setlow;
		break;
		
		case Lockdoor_setlow:
		PORTA = 0x00;          // set low for 19 ms need to do this 10 times
		counter++;
		if(counter >= 19)         // do 19 times of setting low
		{
			counter = 0;
			counter1++;
			if(counter1 < 10)
			state1 = Lockdoor_sethigh;
			else
			{
				state1 = Init_lock;          // go back to starting state
				lock_status1 = 1;
				counter1 = 0;
			}
		}
		break;
		
		default:
		state1 = Init_lock;
		break;
	}
}
void SelectOption1()
{
	switch(state)
	{
		case start:
		if(GetBit(PINC,0) == 0)                    // first button on pinc pressed
		{
			lock_status = 1;                      
			state = selection1;                            
		}
		if(GetBit(PINC,1) == 0)                   // second button on pinc pressed
		{
			state = selection2;                           
			dispense = 1;                          
		}
		if(GetBit(PINC,2) == 0)                     // third button on pinc pressed
		{
			fan_status = 1;                         
			state = selection3;                             
		}
		break;
	
		case selection1:
		if(GetBit(PINC,0) == 1)                    // wait for first button to be depressed
		{
			state = start;
		}
		break;
		
		case selection2:
		if(GetBit(PINC,1) == 1)					// wait for second button to be depressed
		{
			state = start;
		}
		break;
		
		case selection3:
		if(GetBit(PINC,2) == 1)					// wait for third button to be depressed
		{
			state = start;
		}
		break;
		
		default:
		state = start;
		break;
	}
}


// Loopers
void SelectOption()
{
	SelectOption_Start();
   for(;;) 
   { 	
	SelectOption1();
	vTaskDelay(200); 
   } 
}

void LockDoor()
{
	LockDoor_Start();
	for(;;)
	{
		LockDoor1();
		vTaskDelay(1);
	}
}

void Dispenser()
{
	Dispenser_Start();
	for(;;)
	{
		Dispenser1();
		vTaskDelay(1);
	}
}

void Fan()
{
	Fan_Start();
	for(;;)
	{
		Fan1();
		vTaskDelay(1);
	}
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(SelectOption, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(LockDoor, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(Dispenser, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(Fan, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}	
 
int main(void) 
{ 
   DDRA = 0xFF; PORTA = 0x00;
   DDRB = 0xFF; PORTB = 0x00;
   DDRC = 0x00; PORTC = 0xFF;
   DDRD = 0xFF; PORTD = 0x00;
  
   StartSecPulse(1);
   vTaskStartScheduler(); 
   return 0; 
}
*/