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
//#define TIME 60 // Normal Mode
#define TIME 1 // Test Mode  


unsigned int second = 0, sun_set = 0, sun_rise = 0, min_accu = 0, daylight = 0, daylight_pre = 0; // Global variable - second, sun_set (when LED is on), sun_rise (when LED is off), min_accu (accumulative minute)
unsigned char minute = 0, hour = 0; summer = 1; midday = 12; // Set the minute, max minute is 60, thus use unsigned char to save memory

void main(void) {
	//call your initialisation functions to set up the hardware modules

     // setup pin for output (connected to LED - RH3)
    LATHbits.LATH3 = 1;   // LATx registers (output latch),set the light to be on initially 
    TRISHbits.TRISH3 = 0; // TRISx registers (data direction), set TRIS value for pin (output)
    
    Interrupts_init(); // Enable Interrupt
    Comp1_init();// Enable Comparator
    Timer0_init();// Enable timer0
    LEDarray_init();// Enable LED array (from lab 2)
    
    while (1) {
        if (second == TIME) {minute += 1; min_accu += 1; second = 0;}
        if (minute == TIME) {hour += 1; minute = 0;}
        if (hour == 24) {hour = 0; min_accu = 0; sun_rise = 0; sun_set = 0;} // When time passes 12am/0am min_accu, sun_rise and sun_set is reset 
        
        if (summer == 0){ // Summer = 0 means winter; 
            if (daylight >= 11*TIME && daylight_pre >= 11*TIME){// At winter, if two consecutive days have a daylight time more than 11 hours, it becomes summer time
                summer = 1;
                midday = 13; 
            }
        }
        if (summer == 1){ //Summer = 1 means summer.
            if (daylight < 11*TIME && daylight_pre < 11*TIME){// At summer, if two consecutive days have a daylight time less than 11 hours, it becomes winter time
                summer = 0;
                midday = 12;
            }
        }    
                
        if ((hour >= 1) && (hour < 5)){LATHbits.LATH3 = 0;} // During 1 am to 5 am, turn off LED
        else if (hour == 5){ 
            if (CMOUTbits.MC1OUT == 1){LATHbits.LATH3 = 1;} // At 5 am, if the surrounding environment is still dark, turn on LED (Check the comparator output pin)
            else {LATHbits.LATH3 = 0;} // At 5 am, if the sun rises, turn off LED
        }
        
        
        if (sun_rise > 0 && sun_set > 0){
            daylight = sun_set - sun_rise; // Daylight time in minutes
            hour = midday + (daylight/2)/TIME; // Overwrite the current hour
            minute = (daylight/2) % TIME; // Overwrite the current minute
            daylight_pre = daylight;
            sun_rise = 0; // Clear the sun_rise time today
            sun_set = 0; // Clear the sun_rise time today
        }
        
		LEDarray_disp_bin(hour); // Current timer value
    }
    
}


/************************************
 * High priority interrupt service routine
 * Make sure all enabled interrupts are checked and flags cleared
************************************/
void __interrupt(high_priority) HighISR()
{
	//add your ISR code here i.e. check the flag, do something (i.e. toggle an LED), clear the flag...	
    if (PIR2bits.C1IF){ // if C1IF ==1        //check the interrupt source for the comparator; When surrounding light is dark, turn on LED; vice versa.
        LATHbits.LATH3 = !LATHbits.LATH3;     //toggle LED (same procedure as lab-1)
        if (LATHbits.LATH3){sun_set = min_accu;} else {sun_rise = min_accu;}    // After the toggle, if LED is on, means sun set; if LED is off, means sun rise
        PIR2bits.C1IF = 0; }                  //clear the interrupt flag!
   
    if (PIR0bits.TMR0IF){ // if TMR0IF ==1    //check the interrupt source for the timer, if overflow occur
        second += 1;
        PIR0bits.TMR0IF = 0; }			      //clear the interrupt flag!
    
    }