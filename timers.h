/* 
 * File:   timers.h
 * Author: Kangan Chen
 *
 * Created on February 5, 2022, 12:00 AM
 */

#ifndef _timers_H
#define _timers_H

#include <xc.h>

#define _XTAL_FREQ 64000000

void Timer0_init(void);
unsigned int get16bitTMR0val(void);

#endif
