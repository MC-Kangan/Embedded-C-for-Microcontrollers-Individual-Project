/* 
 * File:   interrupts.h
 * Author: Kangan Chen
 *
 * Created on February 5, 2022, 12:00 AM
 */

#ifndef _interrupts_H
#define _interrupts_H

#include <xc.h>

#define _XTAL_FREQ 64000000

void Interrupts_init(void);
void __interrupt(high_priority) HighISR();

#endif
