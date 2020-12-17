/*	Partner(s) Name & E-mail: Abraham Kuruvila akuru003@ucr.edu    No Partner
 *	Lab Section: 21
 *	Assignment: Lab 1 Part 1 Exercise 2
 *	Exercise Description: Pattern Sequencer
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.7 --- 9/27/2016 17:20:23 PST
*/

#include "rims.h"

short ar[8] = { 1,1,1,1,1,1,1,1};
short ar1[8] = {0,0,0,0,0,0,0,0};
short ar2[8] = {1,1,1,1,1,1,1,0};
short ar3[8] = {1,1,1,1,1,1,0,1};
short ar4[8] = {1,1,1,1,1,0,1,1};
short ar5[8] = {1,1,1,1,0,1,1,1};
short ar6[8] = {1,1,1,0,1,1,1,1};
short ar7[8] = {1,1,0,1,1,1,1,1};
short ar8[8] = {1,0,1,1,1,1,1,1};
short ar9[8] = {0,1,1,1,1,1,1,1};
unsigned char SM1_Clk;
void TimerISR() {
   SM1_Clk = 1;
}

enum SM1_States { SM1_s1, SM1_s2, SM1_s3, SM1_s4, SM1_s5, SM1_s6, SM1_s7, SM1_s8, SM1_s9, SM1_s10, SM1_s11 } SM1_State;

TickFct_State_machine_1() {
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_s1;
         break;
         case SM1_s1: 
         if (1) {
            SM1_State = SM1_s2;
         }
         break;
      case SM1_s2: 
         if (!A0) {
            SM1_State = SM1_s3;
         }
         else if (A0) {
            SM1_State = SM1_s11;
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
            SM1_State = SM1_s10;
         }
         else if (A0) {
            SM1_State = SM1_s8;
         }
         break;
      case SM1_s10: 
         if (!A0) {
            SM1_State = SM1_s11;
         }
         else if (A0) {
            SM1_State = SM1_s9;
         }
         break;
      case SM1_s11: 
         if (!A0) {
            SM1_State = SM1_s2;
         }
         else if (A0) {
            SM1_State = SM1_s10;
         }
         break;
      default:
         SM1_State = SM1_s1;
   } // Transitions

   switch(SM1_State) { // State actions
      case SM1_s1:
         break;
      case SM1_s2:
         B0 = ar[0];
         B1 = ar[1];
         B2 = ar[2];
         B3 = ar[3];
         B4 = ar[4];
         B5 = ar[5];
         B6 = ar[6];
         B7 = ar[7];
         break;
      case SM1_s3:
         B0 = ar1[0];
         B1 = ar1[1];
         B2 = ar1[2];
         B3 = ar1[3];
         B4 = ar1[4];
         B5 = ar1[5];
         B6 = ar1[6];
         B7 = ar1[7];
         break;
      case SM1_s4:
         B0 = ar2[0];
         B1 = ar2[1];
         B2 = ar2[2];
         B3 = ar2[3];
         B4 = ar2[4];
         B5 = ar2[5];
         B6 = ar2[6];
         B7 = ar2[7];
         break;
      case SM1_s5:
         B0 = ar3[0];
         B1 = ar3[1];
         B2 = ar3[2];
         B3 = ar3[3];
         B4 = ar3[4];
         B5 = ar3[5];
         B6 = ar3[6];
         B7 = ar3[7];
         break;
      case SM1_s6:
         B0 = ar4[0];
         B1 = ar4[1];
         B2 = ar4[2];
         B3 = ar4[3];
         B4 = ar4[4];
         B5 = ar4[5];
         B6 = ar4[6];
         B7 = ar4[7];
         break;
      case SM1_s7:
         B0 = ar5[0];
         B1 = ar5[1];
         B2 = ar5[2];
         B3 = ar5[3];
         B4 = ar5[4];
         B5 = ar5[5];
         B6 = ar5[6];
         B7 = ar5[7];
         break;
      case SM1_s8:
         B0 = ar6[0];
         B1 = ar6[1];
         B2 = ar6[2];
         B3 = ar6[3];
         B4 = ar6[4];
         B5 = ar6[5];
         B6 = ar6[6];
         B7 = ar6[7];
         break;
      case SM1_s9:
         B0 = ar7[0];
         B1 = ar7[1];
         B2 = ar7[2];
         B3 = ar7[3];
         B4 = ar7[4];
         B5 = ar7[5];
         B6 = ar7[6];
         B7 = ar7[7];
         break;
      case SM1_s10:
         B0 = ar8[0];
         B1 = ar8[1];
         B2 = ar8[2];
         B3 = ar8[3];
         B4 = ar8[4];
         B5 = ar8[5];
         B6 = ar8[6];
         B7 = ar8[7];
         break;
      case SM1_s11:
         B0 = ar9[0];
         B1 = ar9[1];
         B2 = ar9[2];
         B3 = ar9[3];
         B4 = ar9[4];
         B5 = ar9[5];
         B6 = ar9[6];
         B7 = ar9[7];
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