#include <xc.h>
#include "interrupts.h"

/************************************
 * Function to turn on interrupts and set if priority is used
 * Note you also need to enable peripheral interrupts in the INTCON register to use CM1IE.
************************************/

// PIE enables individual interrupts (e.g. set PIE2bits.C1IE = 1)
// PIR are the flags that tells you which interrupt was triggered (e.g. check PIR2bits.C1IF = 1)

void Interrupts_init(void)
{
	// turn on global interrupts, peripheral interrupts and the interrupt source 
	// It's a good idea to turn on global interrupts last, once all other interrupt configuration is done.
    //PIE2bits.C1IE = 1; // From instruction: To make use of the comparator interrupt you need to enable it by setting C1IE in the PIE2 register 
                       // and check the C1IF bit in PIR2 in your ISR.(From datasheet P224)
    PIE0bits.TMR0IE = 1; // Turn on TMR0 interrupt source (From datasheet P222)
    INTCONbits.GIEL = 1;// Enable peripheral interrupts (PEIE also ok, see datasheet P209)
    INTCONbits.GIE=1; 	//turn on interrupts globally (when this is off, all interrupts are deactivated)
}


/************************************
 * High priority interrupt service routine
 * Make sure all enabled interrupts are checked and flags cleared
************************************/
void __interrupt(high_priority) HighISR()
{
	//add your ISR code here i.e. check the flag, do something (i.e. toggle an LED), clear the flag...	
    if (PIR0bits.TMR0IF){ // if TMR0IF ==1    //check the interrupt source
        LATHbits.LATH3 = !LATHbits.LATH3; //toggle LED (same procedure as lab-1)
        //TMR0H = 65535;            // Re-write initial values
        //TMR0L = 3035; 
        PIR0bits.TMR0IF = 0; 						//clear the interrupt flag!
    }
}

