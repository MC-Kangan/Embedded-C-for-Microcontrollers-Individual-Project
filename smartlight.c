#include <xc.h>
#include "smartlight.h"

/************************************
/ Function: daylight_saving_time
/ Function used to adjust the daylight saving time of the program. The fuction will check the daylight length of two consecutive days 
/ and determine whether it is winter time (midday = 12) or summer time (midday = 13)

/ Arguments: midday, daylight, daylight_pre
/ Return: midday 
************************************/

unsigned char daylight_saving_time (unsigned char midday, unsigned int daylight, unsigned int daylight_pre){
    if (midday == 12){ // Means winter time 
        // At winter, if two consecutive days have a daylight time more than 11 hours, it becomes summer time
        if (daylight >= 11*TIME && daylight_pre >= 11*TIME){
            midday = 13; 
        }
    }
    else if (midday == 13){ // Means summer time
        // At summer, if two consecutive days have a daylight time less than 11 hours (but greater than 0), it becomes winter time
        // The daylight needs to be greater than 0 to be valid.
        if (0 < daylight && daylight < 11 * TIME && 0 < daylight_pre && daylight_pre < 11 * TIME){
            midday = 12;
        }
    }
    return midday; // Return the midday
}    

/************************************
/ Function: one_to_five
/ Function used to switch off the LED between 1 am to 5 am, despite the surrounding lighting conditions.

/ Arguments: hour
************************************/

void one_to_five(unsigned char hour){
    if ((hour >= 1) && (hour < 5)){LATHbits.LATH3 = 0;} // During 1 am to 5 am, turn off LED
    else if (hour == 5){ 
        if (CMOUTbits.MC1OUT == 1){LATHbits.LATH3 = 1;} // At 5 am, if the surrounding environment is still dark, turn on LED (Check the comparator output pin)
        else {LATHbits.LATH3 = 0;}                      // At 5 am, if the sun rises, turn off LED 
    }
}