/*	Partner(s) Name & E-mail: Abraham Kuruvila akuru003@ucr.edu    No Partner
 *	Lab Section: 21
 *	Assignment: Lab 1 Part 1 Exercise 1
 *	Exercise Description: Bouncing Leds
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.7 --- 9/26/2016 10:9:10 PST
*/

#include "rims.h"

/*Define user variables and functions for this state machine here.*/
unsigned char SM1_Clk;
void TimerISR() {
   SM1_Clk = 1;
}

enum SM1_States { SM1_start, SM1_s2, SM1_s3, SM1_s4, SM1_s5, SM1_s6, SM1_s7, SM1_s8, SM1_s9 } SM1_State;

TickFct_State_machine_1() {
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_start;
         break;
         case SM1_start: 
         if (1) {
            SM1_State = SM1_s2;
         }
         break;
      case SM1_s2: 
         if (!A0) {
            SM1_State = SM1_s3;
         }
         else if (A0) {
            SM1_State = SM1_s9;
         }
         break;
      case SM1_s3: 
         if (!A0) {
            SM1_State = SM1_s4;
         }
         else if (A0) {
            SM1_State = SM1_s2;
         }
         break;
      case SM1_s4: 
         if (!A0) {
            SM1_State = SM1_s5;
         }
         else if (A0) {
            SM1_State = SM1_s3;
         }
         break;
      case SM1_s5: 
         if (!A0) {
            SM1_State = SM1_s6;
         }
         else if (A0) {
            SM1_State = SM1_s4;
         }
         break;
      case SM1_s6: 
         if (!A0) {
            SM1_State = SM1_s7;
         }
         else if (A0) {
            SM1_State = SM1_s5;
         }
         break;
      case SM1_s7: 
         if (!A0) {
            SM1_State = SM1_s8;
         }
         else if (A0) {
            SM1_State = SM1_s6;
         }
         break;
      case SM1_s8: 
         if (!A0) {
            SM1_State = SM1_s9;
         }
         else if (A0) {
            SM1_State = SM1_s7;
         }
         break;
      case SM1_s9: 
         if (!A0) {
            SM1_State = SM1_s2;
         }
         else if (A0) {
            SM1_State = SM1_s8;
         }
         break;
      default:
         SM1_State = SM1_start;
   } // Transitions

   switch(SM1_State) { // State actions
      case SM1_start:
         B = 0;
         break;
      case SM1_s2:
         B = 0;
         B0 = 1;
         break;
      case SM1_s3:
         B = 0;
         B1 = 1;
         break;
      case SM1_s4:
         B = 0;
         B2 = 1;
         break;
      case SM1_s5:
         B = 0;
         B3 = 1;
         break;
      case SM1_s6:
         B = 0;
         B4 = 1;
         break;
      case SM1_s7:
         B = 0;
         B5 = 1;
         break;
      case SM1_s8:
         B = 0;
         B6 = 1;
         break;
      case SM1_s9:
         B = 0;
         B7 = 1;
         break;
      default: // ADD default behaviour below
      break;
   } // State actions

}

int main() {

   const unsigned int periodState_machine_1 = 1000; // 1000 ms default
   TimerSet(periodState_machine_1);
   TimerOn();
   
   SM1_State = -1; // Initial state
   B = 0; // Init outputs

   while(1) {
      TickFct_State_machine_1();
      while(!SM1_Clk);
      SM1_Clk = 0;
   } // while (1)
} // Main