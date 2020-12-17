/*	Partner(s) Name & E-mail: Abraham Kuruvila akuru003@ucr.edu    No Partner
 *	Lab Section: 21
 *	Assignment: Lab 1 Part 4 Exercise 1
 *	Exercise Description: Partner Activity 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.7 --- 9/27/2016 19:42:10 PST
*/

#include "rims.h"

/*This code will be shared between state machines.*/
static int i = 0;
unsigned char TimerFlag = 0;
void TimerISR() {
   TimerFlag = 1;
}


enum SM1_States { SM1_s1, SM1_s2, SM1_s3 } SM1_State;

TickFct_State_machine_1() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_s1;
         break;
      case SM1_s1:
         if (A1) {
            SM1_State = SM1_s2;
            i = 1;
         }
         break;
      case SM1_s2:
         if (1) {
            SM1_State = SM1_s3;
         }
         else if (!A1) {
            SM1_State = SM1_s1;
            i = 0;
         }
         break;
      case SM1_s3:
         if (1) {
            SM1_State = SM1_s2;
         }
         else if (!A1) {
            SM1_State = SM1_s1;
            i = 0;
         }
         break;
      default:
         SM1_State = SM1_s1;
      } // Transitions

   switch(SM1_State) { // State actions
      case SM1_s1:
         break;
      case SM1_s2:
         B = 0xA0;
         break;
      case SM1_s3:
         B = 0x00;
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}

enum SM2_States { SM2_s1, SM2_s2, SM2_s3, SM2_s4, SM2_s5 } SM2_State;

TickFct_State_machine_2() {
   static int j = 0;
   switch(SM2_State) { // Transitions
      case -1:
         SM2_State = SM2_s1;
         break;
      case SM2_s1:
         if (1) {
            SM2_State = SM2_s2;
         }
         break;
      case SM2_s2:
         if (j >= 4) {
            SM2_State = SM2_s3;
            j = 0;
         }
         else if (j < 4 && !A1) {
            SM2_State = SM2_s2;
            j++;
         }
         else if (A1) {
            SM2_State = SM2_s5;
         }
         break;
      case SM2_s3:
         if (j < 2 && !A1) {
            SM2_State = SM2_s3;
            j++;
         }
         else if (j >= 2) {
            SM2_State = SM2_s4;
            j = 0;
         }
         else if (A1) {
            SM2_State = SM2_s5;
         }
         break;
      case SM2_s4:
         if (j < 3 && !A1) {
            SM2_State = SM2_s4;
            j++;
         }
         else if (j >= 3) {
            SM2_State = SM2_s2;
            j = 0;
         }
         else if (A1) {
            SM2_State = SM2_s5;
         }
         break;
      case SM2_s5:
         if (!A1) {
            SM2_State = SM2_s2;
         }
         break;
      default:
         SM2_State = SM2_s1;
      } // Transitions

   switch(SM2_State) { // State actions
      case SM2_s1:
         break;
      case SM2_s2:
         B = 0x01;
         break;
      case SM2_s3:
         B = 0x02;
         break;
      case SM2_s4:
         B = 0x04;
         break;
      case SM2_s5:
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}
int main() {
   B = 0; //Init outputs
   TimerSet(1000);
   TimerOn();
   SM1_State = -1;
   SM2_State = -1;
   while(1) {
      TickFct_State_machine_1();
      TickFct_State_machine_2();
      while (!TimerFlag);
      TimerFlag = 0;
   }
}