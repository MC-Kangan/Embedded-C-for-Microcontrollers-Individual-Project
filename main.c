// CONFIG1
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "LEDarray.h"
#include "interrupts.h"
#include "comparator.h"
#include "timers.h"
#include "smartlight.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  
//#define TIME 60 // Normal Mode (1 min = 60 seconds, 1 hour = 60 minutes)
#define TIME 1 // Test Mode (1 min = 1 seconds, 1 hour = 1 minutes)  


// Set the second, minute and hour to 0 initially; the midday is 12 o'clock for winter time and 13 o'clock for summer time
unsigned char second = 0, minute = 0, hour = 0, midday = 12; 

// Set the min_accu (accumulative minute); sun_set (time when LED is on); sun_rise (time when LED is off);
// daylight (daylight time); daylight_pre (daylight time of the previous day)
unsigned int min_accu = 0, sun_set = 0, sun_rise = 0, daylight = 0, daylight_pre = 0; 

void main(void) {
	//call your initialisation functions to set up the hardware modules

     // setup pin for output (connected to LED - RH3)
    LATHbits.LATH3 = 1;   // LATx registers (output latch),set the light to be on initially 
    TRISHbits.TRISH3 = 0; // TRISx registers (data direction), set TRIS value for pin (output)
    
    Interrupts_init();   // Enable Interrupt
    Comp1_init();        // Enable Comparator
    Timer0_init();       // Enable timer0
    LEDarray_init();     // Enable LED array (from lab 2)
    
    
    while (1) {
        if (second == TIME) {minute += 1; min_accu += 1; second = 0;}
        if (minute == TIME) {hour += 1; minute = 0;}
        if (hour == 24) {hour = 0; min_accu = 0; sun_rise = 0; sun_set = 0;} // When time passes 12am/0am min_accu, sun_rise and sun_set is reset 
        
        midday = daylight_saving_time(midday, daylight, daylight_pre);
        one_to_five(hour);
       
        if (sun_rise > 0 && sun_set > 0){
            daylight = sun_set - sun_rise; // Daylight time in minutes
            if (4 * TIME < daylight){ // Typical daylight length should be greater than 4 hours, otherwise, it could be an error
                hour = midday + (daylight/2)/TIME; // Overwrite the current hour (Take the quotient of the division as the hour)
                minute = (daylight/2) % TIME; // Overwrite the current minute (Take the remainder of the division as the minute)
                daylight_pre = daylight; // Store today's daylight to daylight of the previous day
                sun_rise = 0; // Clear the sun_rise time today
                sun_set = 0; // Clear the sun_set time today
            }else{
                daylight = daylight_pre; // If the daylight time is less than 4 hour, let daylight = daylight_pre 
            }
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

