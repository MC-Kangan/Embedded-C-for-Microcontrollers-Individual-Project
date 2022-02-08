#include <xc.h>
#include "LEDarray.h"
#include <math.h>

/************************************
/ LEDarray_init
/ Function used to initialise pins to drive the LEDarray
************************************/
void LEDarray_init(void)
{   //set up TRIS registers for pins connected to LED array
    TRISGbits.TRISG0 = 0; //pin G0 set to output
    TRISGbits.TRISG1 = 0; //pin G1 set to output
    TRISAbits.TRISA2 = 0; //pin A2 set to output
    TRISFbits.TRISF6 = 0; //pin F6 set to output
    TRISAbits.TRISA4 = 0; //pin A4 set to output
    TRISAbits.TRISA5 = 0; //pin A5 set to output
    TRISFbits.TRISF0 = 0; //pin F0 set to output
    TRISBbits.TRISB0 = 0; //pin B0 set to output
    TRISBbits.TRISB1 = 0; //pin B1 set to output
    
	//set initial output LAT values (they may have random values when powered on)
    LATGbits.LATG0 = 0; // output of pin G0 set low
    LATGbits.LATG1 = 0; // output of pin G1 set low
    LATAbits.LATA2 = 0; // output of pin A2 set low
    LATFbits.LATF6 = 0; // output of pin F6 set low
    LATAbits.LATA4 = 0; // output of pin A4 set low
    LATAbits.LATA5 = 0; // output of pin A5 set low
    LATFbits.LATF0 = 0; // output of pin F0 set low
    LATBbits.LATB0 = 0; // output of pin B0 set low
    LATBbits.LATB1 = 0; // output of pin B1 set low
    
   
}

/************************************
/ LEDarray_disp_bin
/ Function used to display a number on the LED array in binary
************************************/
void LEDarray_disp_bin(unsigned int number)
{
	//some code to turn on/off the pins connected to the LED array
	//if statements and bit masks can be used to determine if a particular pin should be on/off
	//see Readme.md for examples
    
    
    // If the individual bit in number = the individual bit in the binary mask, return true, otherwise false.
    // Once the LED is on, do not turn it off, therefore, no else statement
    if (number & 0b00000001) { LATGbits.LATG0=1;} else { LATGbits.LATG0=0;} // 0b is the prefix for binary, number = 1
    if (number & 0b00000010) { LATGbits.LATG1=1;} else { LATGbits.LATG1=0;} // 0b is the prefix for binary, number = 2
    if (number & 0b00000100) { LATAbits.LATA2=1;} else { LATAbits.LATA2=0;} // 0b is the prefix for binary, number = 4
    if (number & 0b00001000) { LATFbits.LATF6=1;} else { LATFbits.LATF6=0;} // 0b is the prefix for binary, number = 8
    if (number & 0b00010000) { LATAbits.LATA4=1;} else { LATAbits.LATA4=0;} // 0b is the prefix for binary, number = 16
    if (number & 0b00100000) { LATAbits.LATA5=1;} else { LATAbits.LATA5=0;} // 0b is the prefix for binary, number = 32
    if (number & 0b01000000) { LATFbits.LATF0=1;} else { LATFbits.LATF0=0;} // 0b is the prefix for binary, number = 64
    if (number & 0b10000000) { LATBbits.LATB0=1;} else { LATBbits.LATB0=0;} // 0b is the prefix for binary, number = 128
    //if (number & 0b00000000) { LATBbits.LATB1=1;} else { LATBbits.LATB1=0;} // This condition will never meet. anything AND 00000000 will produce 0, which will always return false

    
}

/************************************
/ Function LEDarray_disp_dec
/ Used to display a number on the LEDs
/ where each LED is a value of 10
************************************/
void LEDarray_disp_dec(unsigned int number)
{
	unsigned int disp_val;
	disp_val = 0;
	//some code to manipulate the variable number into the correct
	//format and store in disp_val for display on the LED array
    if (number > 9)  { disp_val = 1;} // if count>9 display number 1
    if (number > 19) { disp_val = 2;} // if count>19 display number 2
    if (number > 29) { disp_val = 4;} // if count>19 display number 4
    if (number > 39) { disp_val = 8;} // if count>19 display number 8
    if (number > 49) { disp_val = 16;} // if count>19 display number 16
    if (number > 59) { disp_val = 32;} // if count>19 display number 32
    if (number > 69) { disp_val = 64;} // if count>19 display number 64
    if (number > 79) { disp_val = 128;} // if count>19 display number 128

	LEDarray_disp_bin(disp_val); 	//display value on LED array, feed in the disp_value to LEDarray_disp_bin defined above
}


/************************************
/ LEDarray_disp_PPM
/ Function used to display a level on the LED array with peak hold
/ cur_val is the current level from the most recent sample, and max is the peak value for the last second
/ these input values need to calculated else where in your code
************************************/
void LEDarray_disp_PPM(unsigned int cur_val, unsigned int max)
{
    // some code to format the variable cur_val and max, store in disp_val for display on the LED array
	// hint: one method is to manipulate the variables separately and then combine them using the bitwise OR operator
    
	unsigned int disp_val;
	unsigned int corr;
    disp_val = 0; // Initialize disp_val
    corr = 20; // Correction factor, adjust this number according to thee room environment. When room is bright, increase corr, vice versa.
    
    // Display cur_val on LED
    // If disp_val = 1, show 1 LED; disp_val = 1 + 2 = 3, show 2 LED; etc.
    if (cur_val > 9+corr) { disp_val += 1;} // if cur_val> 9+corr display number +1
    if (cur_val > 19+corr) { disp_val += 2;} // if cur_val> 19+corr display number +2
    if (cur_val > 29+corr) { disp_val += 4;} // if cur_val> 29+corr display number +4
    if (cur_val > 39+corr) { disp_val += 8;} // if cur_val> 39+corr display number +8
    if (cur_val > 49+corr) { disp_val += 16;} // if cur_val> 49+corr display number +16
    if (cur_val > 59+corr) { disp_val += 32;} // if cur_val> 59+corr display number +32
    if (cur_val > 69+corr) { disp_val += 64;} // if cur_val> 69+corr display number +64
    if (cur_val > 79+corr) { disp_val += 128;} // if cur_val> 79+corr display number +128
    
    // If cur_val > = max, display cur_val on LED
    if (cur_val >= max) { 
        LEDarray_disp_bin(disp_val);} 
    else { // Show individual LED of the maximum, this is done by adding a number (1 or 2 or 4 etc) directly on the disp_val
        // When the max value lands in the region defined, add specific number 
        if (max > 0+corr && max <=9+corr){ disp_val += 1;} 
        if (max > 9+corr && max <=19+corr) { disp_val += 2;} 
        if (max > 19+corr && max <=29+corr) { disp_val += 4;} 
        if (max > 29+corr && max <=39+corr) { disp_val += 8;} 
        if (max > 39+corr && max <=49+corr) { disp_val += 16;} 
        if (max > 49+corr && max <=59+corr) { disp_val += 32;} 
        if (max > 59+corr && max <=69+corr) { disp_val += 64;} 
        if (max > 69+corr && max <=79+corr) { disp_val += 128;} 
        LEDarray_disp_bin(disp_val);
    }
  
}

