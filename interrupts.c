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
    INTCONbits.IPEN = 1; // Enable priority levels on interrupts
    INTCONbits.GIEL = 1; // Enable peripheral interrupts (PEIE also ok, see datasheet P209)
    PIE0bits.TMR0IE = 1; // Turn on Timer0 TMR0 interrupt source (From datasheet P222)
    PIE2bits.C1IE = 1;   // From instruction: To make use of the comparator interrupt you need to enable it by setting C1IE in the PIE2 register 
                         // and check the C1IF bit in PIR2 in your ISR.(From datasheet P224)
    INTCONbits.GIE=1; 	 //Turn on interrupts globally (when this is off, all interrupts are deactivated)
}


