#include <xc.h>
#include "smartlight.h"

unsigned char daylight_saving_time (unsigned char midday, unsigned int daylight, unsigned int daylight_pre){
    if (midday == 12){ // Means winter time 
        // At winter, if two consecutive days have a daylight time more than 11 hours, it becomes summer time
        if (daylight >= 11*TIME && daylight_pre >= 11*TIME){
            midday = 13; 
        }
    }
    else if (midday == 13){ // Means summer time
        // At summer, if two consecutive days have a daylight time less than 11 hours, it becomes winter time
        if (daylight < 11*TIME && daylight_pre < 11*TIME){
            midday = 12;
        }
    }
    return midday;
}    

void one_to_five(unsigned char hour){
    if ((hour >= 1) && (hour < 5)){LATHbits.LATH3 = 0;} // During 1 am to 5 am, turn off LED
    else if (hour == 5){ 
        if (CMOUTbits.MC1OUT == 1){LATHbits.LATH3 = 1;} // At 5 am, if the surrounding environment is still dark, turn on LED (Check the comparator output pin)
        else {LATHbits.LATH3 = 0;} // At 5 am, if the sun rises, turn off LED 
    }
}