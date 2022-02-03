// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "LEDarray.h"
#include "interrupts.h"
#include "comparator.h"
#include "timers.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

unsigned int second = 0; // Global variable 


void main(void) {
	//call your initialisation functions to set up the hardware modules

     // setup pin for output (connected to LED2 - RH3)
    LATHbits.LATH3 = 1;   // LATx registers (output latch),set the light to be on initially 
    TRISHbits.TRISH3 = 0; // TRISx registers (data direction), set TRIS value for pin (output)
    
    Interrupts_init(); // Enable Interrupt
    Comp1_init();// Enable Comparator
    Timer0_init();// Enable timer0
    LEDarray_init();// Enable LED array (from lab 2)
    
    while (1) {
		LEDarray_disp_bin(second); // Current timer value
    }
}


/************************************
 * High priority interrupt service routine
 * Make sure all enabled interrupts are checked and flags cleared
************************************/
void __interrupt(high_priority) HighISR()
{
	//add your ISR code here i.e. check the flag, do something (i.e. toggle an LED), clear the flag...	
    if (PIR2bits.C1IF){ // if C1IF ==1      //check the interrupt source for the comparator; When surrounding light is dark, turn on LED; vice versa.
        LATHbits.LATH3 = !LATHbits.LATH3;   //toggle LED (same procedure as lab-1)
        PIR2bits.C1IF = 0; }			        //clear the interrupt flag!
    if (PIR0bits.TMR0IF){ // if TMR0IF ==1      //check the interrupt source for the comparator; When surrounding light is dark, turn on LED; vice versa.
        second += 1;
        PIR0bits.TMR0IF = 0; }			        //clear the interrupt flag!
    
    }