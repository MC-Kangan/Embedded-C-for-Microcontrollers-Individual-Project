/* 
 * File:   smartlight.h
 * Author: Kangan Chen
 *
 * Created on February 5, 2022, 12:00 AM
 */

#ifndef SMARTLIGHT_H
#define	SMARTLIGHT_H

#include <xc.h>
#define _XTAL_FREQ 64000000
//#define TIME 60 // Normal Mode (1 min = 60 seconds, 1 hour = 60 minutes)
#define TIME 1 // Test Mode (1 min = 1 seconds, 1 hour = 1 minutes) 

unsigned char daylight_saving_time (unsigned char midday, unsigned int daylight, unsigned int daylight_pre);
void one_to_five(unsigned char hour);


#endif




