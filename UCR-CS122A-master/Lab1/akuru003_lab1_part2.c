/*	Partner(s) Name & E-mail: Abraham Kuruvila akuru003@ucr.edu    No Partner
 *	Lab Section: 21
 *	Assignment: Lab 1 Part 2 Exercise 1
 *	Exercise Description: Concurrent SM's
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.7 --- 9/27/2016 18:18:23 PST
*/

#include "rims.h"

/*This code will be shared between state machines.*/
unsigned char TimerFlag = 0;
void TimerISR() {
   TimerFlag = 1;
}


enum SM1_States { SM1_start, SM1_s2, SM1_s3, SM1_s4, SM1_s5, SM1_s6, SM1_s7, SM1_s8, SM1_s9 } SM1_State;

TickFct_State_machine_1() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_start;
         break;
      case SM1_start:
         if (!A7 && !A6) {
            SM1_State = SM1_s2;
         }
         break;
      case SM1_s2:
         if (!A0 && !A7 && !A6) {
            SM1_State = SM1_s3;
         }
         else if (A0 && !A7 && !A6) {
            SM1_State = SM1_s9;
         }
         else if (!(!A7 && !A6)) {
            SM1_State = SM1_start;
         }
         break;
      case SM1_s3:
         if (!A0 && !A7 && !A6) {
            SM1_State = SM1_s4;
         }
         else if (A0 && !A7 && !A6) {
            SM1_State = SM1_s2;
         }
         else if (!(!A7 && !A6)) {
            SM1_State = SM1_start;
         }
         break;
      case SM1_s4:
         if (!A0 && !A7 && !A6) {
            SM1_State = SM1_s5;
         }
         else if (A0 && !A7 && !A6) {
            SM1_State = SM1_s3;
         }
         else if (!(!A7 && !A6)) {
            SM1_State = SM1_start;
         }
         break;
      case SM1_s5:
         if (!A0 && !A7 && !A6) {
            SM1_State = SM1_s6;
         }
         else if (A0 && !A7 && !A6) {
            SM1_State = SM1_s4;
         }
         else if (!(!A7 && !A6)) {
            SM1_State = SM1_start;
         }
         break;
      case SM1_s6:
         if (!A0 && !A7 && !A6) {
            SM1_State = SM1_s7;
         }
         else if (A0 && !A7 && !A6) {
            SM1_State = SM1_s5;
         }
         else if (!(!A7 && !A6)) {
            SM1_State = SM1_start;
         }
         break;
      case SM1_s7:
         if (!A0 && !A7 && !A6) {
            SM1_State = SM1_s8;
         }
         else if (A0 && !A7 && !A6) {
            SM1_State = SM1_s6;
         }
         else if (!(!A7 && !A6)) {
            SM1_State = SM1_start;
         }
         break;
      case SM1_s8:
         if (!A0 && !A7 && !A6) {
            SM1_State = SM1_s9;
         }
         else if (A0 && !A7 && !A6) {
            SM1_State = SM1_s7;
         }
         else if (!(!A7 && !A6)) {
            SM1_State = SM1_start;
         }
         break;
      case SM1_s9:
         if (!A0 && !A7 && !A6) {
            SM1_State = SM1_s2;
         }
         else if (A0 && !A7 && !A6) {
            SM1_State = SM1_s8;
         }
         else if (!(!A7 && !A6)) {
            SM1_State = SM1_start;
         }
         break;
      default:
         SM1_State = SM1_start;
      } // Transitions

   switch(SM1_State) { // State actions
      case SM1_start:
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

enum SM2_States { SM2_s1, SM2_s2 } SM2_State;

TickFct_State_machine_2() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
   switch(SM2_State) { // Transitions
      case -1:
         SM2_State = SM2_s1;
         break;
      case SM2_s1:
         if (A7 && A6) {
            SM2_State = SM2_s2;
         }
         break;
      case SM2_s2:
         if (!(A7 && A6)) {
            SM2_State = SM2_s1;
         }
         break;
      default:
         SM2_State = SM2_s1;
      } // Transitions

   switch(SM2_State) { // State actions
      case SM2_s1:
         break;
      case SM2_s2:
         B = 0xFF;
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}

enum SM3_States { SM3_s1, SM3_s2, SM3_s3 } SM3_State;

TickFct_State_machine_3() {
   short a[10] = {10,15,20,25,30,35,40,45,50,60};
static int i = 0;
   switch(SM3_State) { // Transitions
      case -1:
         SM3_State = SM3_s1;
         break;
      case SM3_s1:
         if (!A7 && A6 && A0) {
            SM3_State = SM3_s3;
         }
         else if (!A7 && A6 && !A0) {
            SM3_State = SM3_s2;
            i = 0;
         }
         break;
      case SM3_s2:
         if (!A0 && !A7 && A6 && i < 9) {
            SM3_State = SM3_s2;
            i++;
         }
         else if (!A0 && !A7 && A6 && i >= 9) {
            SM3_State = SM3_s2;
            i = 0;
         }
         else if (!A7 && A6 && A0) {
            SM3_State = SM3_s3;
         }
         else if (!(!A7 && A6 && !A0)) {
            SM3_State = SM3_s1;
            i = 0;
         }
         break;
      case SM3_s3:
         if (A0 && !A7 && A6 && i > 0) {
            SM3_State = SM3_s3;
            i--;
         }
         else if (A0 && !A7 && A6 && i  <= 0) {
            SM3_State = SM3_s3;
            i = 9;
         }
         else if (!A7 && A6 && !A0) {
            SM3_State = SM3_s2;
         }
         else if (!(!A7 && A6 && A0)) {
            SM3_State = SM3_s1;
            i = 0;
         }
         break;
      default:
         SM3_State = SM3_s1;
      } // Transitions

   switch(SM3_State) { // State actions
      case SM3_s1:
         i = 0;
         break;
      case SM3_s2:
         B = a[i];
         break;
      case SM3_s3:
         B = a[i];
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}

enum SM4_States { SM4_s1, SM4_s2, SM4_s3 } SM4_State;

TickFct_State_machine_4() {
   short ar[10] = {7,14,21,28,35,42,49,56,63,70};
static int j = 0;
   switch(SM4_State) { // Transitions
      case -1:
         SM4_State = SM4_s1;
         break;
      case SM4_s1:
         if (A7 && !A6 && !A0) {
            SM4_State = SM4_s2;
            j = 0;
         }
         else if (A7 && !A6 && A0) {
            SM4_State = SM4_s3;
         }
         break;
      case SM4_s2:
         if (A7 && !A6 && A0) {
            SM4_State = SM4_s3;
         }
         else if (!A0 && A7 && !A6 && j < 9) {
            SM4_State = SM4_s2;
            j++;
         }
         else if (!A0 && A7 && !A6 && j >= 9) {
            SM4_State = SM4_s2;
            j = 0;
         }
         else if (!(A7 && !A6 && !A0)) {
            SM4_State = SM4_s1;
            j = 0;
         }
         break;
      case SM4_s3:
         if (A7 && !A6 && !A0) {
            SM4_State = SM4_s2;
         }
         else if (A0 && A7 && !A6 && j > 0) {
            SM4_State = SM4_s3;
            j--;
         }
         else if (A0 && A7 && !A6 && j  <= 0) {
            SM4_State = SM4_s3;
            j = 9;
         }
         else if (!(A7 && !A6 && A0)) {
            SM4_State = SM4_s1;
            j = 0;
         }
         break;
      default:
         SM4_State = SM4_s1;
      } // Transitions

   switch(SM4_State) { // State actions
      case SM4_s1:
         j = 0;
         break;
      case SM4_s2:
         B = ar[j];
         break;
      case SM4_s3:
         B = ar[j];
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
   SM3_State = -1;
   SM4_State = -1;
   while(1) {
      TickFct_State_machine_1();
      TickFct_State_machine_2();
      TickFct_State_machine_3();
      TickFct_State_machine_4();
      while (!TimerFlag);
      TimerFlag = 0;
   }
}