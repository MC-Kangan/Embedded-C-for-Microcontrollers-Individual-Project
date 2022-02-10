
/* 
 * File:   main.c
 * Author: Kangan Chen
 *
 * Created on February 5, 2022, 12:00 AM
 */


//SEE README FILE FOR ALL THE EXPLANATIONS OF THE PROGRAM

// CONFIG1
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTE = OFF       // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "LEDarray.h"
#include "interrupts.h"
#include "comparator.h"
#include "timers.h"
#include "smartlight.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz 


// Normal Mode and Test Mode

// The program has two modes, Test and Normal mode. In the Test mode, each hour will be displayed in seconds on the LED array(1h = 1s). 
// In the Normal mode, each hour will be displayed in actual hours on the LED array (1h = 3600s).

//#define TIME 60 // Normal Mode (1 min = 60 seconds, 1 hour = 60 minutes)
#define TIME 1 // Test Mode (1 min = 1 seconds, 1 hour = 1 minutes)  


// Program Setup

// Before running the program, the user has to input the current time into the variable 'hour' and 'minute', the default timing is 0:00. 
// Besides, the user also needs to input the 'midday' variable to implement the adjustments for Daylight Saving Time (in the main program)
// The 'midday' variable only takes two values: 12 and 13. The solar midday is 12 o'clock in winter time and 13 o'clock in summer time. By default, 'midday' is 12.

// Set the global variables
unsigned char second = 0, minute = 0, hour = 0; // Set the second, minute and hour to 0 initially;
unsigned int min_accu = 0, sun_set = 0, sun_rise = 0; // Set the min_accu (accumulative minute); sun_set (time when LED is on); sun_rise (time when LED is off);

// Main program
void main(void) {
    
     // Setup pin for output (connected to LED - RH3)
    LATHbits.LATH3 = 1;   // LATx registers (output latch), set the light to be on initially 
    TRISHbits.TRISH3 = 0; // TRISx registers (data direction), set TRIS value for pin (output)
    
    // Call init functions
    Interrupts_init();    // Enable Interrupt (see interrupts.c)
    Comp1_init();         // Enable Comparator (see comparator.c)
    Timer0_init();        // Enable timer0 (see timers.c)
    LEDarray_init();      // Enable LED array (see LEDarray.c)
    
    // Set the local variables
    unsigned char midday = 12; // The midday is 12 o'clock for winter time and 13 o'clock for summer time. By default, midday = 12.
    unsigned int daylight = 0, daylight_pre = 0; // daylight (daylight time); daylight_pre (daylight time of the previous day)
    
    // Endless loop that repeats itself
    while (1) { 
        
        midday = daylight_saving_time(midday, daylight, daylight_pre); // Defined in smartlight.c, used to make adjustments for Daylight Saving Time       
        one_to_five(hour);                                             // Defined in smartlight.c, used to switch off the LED during 1 am to 5 am
       
        // Adjusting the current time by synchronising with sunlight
        if (sun_rise > 0 && sun_set > 0 && sun_set > sun_rise){ // sun_rise and sun_set are only valid when they both > 0 and sun_set > sun_rise (Both variables are initially 0)
            daylight = sun_set - sun_rise;             // Daylight time in minutes (Difference between the sun_set and sun_rise time)
            if (4 * TIME < daylight){                  // Typical daylight length should be greater than 4 hours, otherwise, it could be an error
                hour = midday + (daylight / 2) / TIME; // Overwrite the current hour to synchronise with sunlight (Take the quotient of the division as the hour)
                minute = (daylight / 2) % TIME;        // Overwrite the current minute to synchronise with sunlight (Take the remainder of the division as the minute)
                second = 0;                            // After the overwrite, reset the second to 0
                daylight_pre = daylight;               // Store today's daylight to daylight of the previous day, to keep track the daylight time for 2 consecutive days
                sun_rise = 0;                          // Clear the sun_rise time today after synchronising with sunlight to 0
                sun_set = 0;                           // Clear the sun_set time today after synchronising with sunlight to 0
            }else{
                daylight = daylight_pre;               // If the daylight time is less than 4 hour, assume daylight = daylight_pre 
            }
        }
      
		LEDarray_disp_bin(hour);                       // Display the current hour
    }
    
}

/************************************
 * High priority interrupt service routine
 * Make sure all enabled interrupts are checked and flags cleared
************************************/

void __interrupt(high_priority) HighISR()
{
	
    // ISR 1: Light sensing by using comparator
    if (PIR2bits.C1IF){ // if C1IF ==1                                       // Check the interrupt source for the comparator; When surrounding light is dark, turn on LED; vice versa.
        LATHbits.LATH3 = !LATHbits.LATH3;                                    // Toggle LED (same procedure as lab-1)
        if (LATHbits.LATH3){sun_set = min_accu;} else {sun_rise = min_accu;} // After the toggle, if LED is on, means sunset; if LED is off, means sunrise; Record the accumulative minutes for sunrise and sunset.
        PIR2bits.C1IF = 0; }                                                 // Clear the interrupt flag!
   
    // ISR 2: Timing by using Timer0
    if (PIR0bits.TMR0IF){                                                    // Check the interrupt source for the timer, if overflow occur
        second += 1;                                                         // Increment second when overflow occurs
        if (second == TIME) {minute += 1; min_accu += 1; second = 0;}        // Increment minute and min_accu (accumulative minute). After the increment, reset second to 0
        if (minute == TIME) {hour += 1; minute = 0;}                         // Increment hour. After the increment, reset minute to 0
        if (hour == 24) {hour = 0; min_accu = 0; sun_rise = 0; sun_set = 0;} // When time passes 12am/0am, min_accu, sun_rise and sun_set is reset to 0
        PIR0bits.TMR0IF = 0; }			                                     // Clear the interrupt flag!
    
}

