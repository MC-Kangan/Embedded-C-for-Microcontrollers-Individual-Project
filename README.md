# Individual project - Energy saving automatic outside light

## Learning outcomes

The principal learning objectives for this project are:

- Implement a working microprocessor based system to achieve a more complex real world task
- Develop your ability to independently plan, organise and structure your code 
- Improve your grasp of the C language and writing your own functions


## Specification
Design and program a device that meets the following requirements:

1. Monitors light level with the LDR and turns on an LED in low light conditions (i.e. night-time) and off in bright conditions (i.e. daytime)
1. Displays the current hour of day on the LED array in binary
1. Turns the light off between approx. 1am and 5am
1. Adjusts for daylight savings time
1. Maintain synchronicity with the sun indefinitely
1. Be fully automatic (requires zero maintenance after installation)

Please use this GitHub repo to manage your software development and submit your individual project code.


## Program Setup Guide

1. The program has two modes, Test and Normal mode. In the Test mode, each hour will be displayed in seconds on the LED array(1h = 1s). 
In the Normal mode, each hour will be displayed in actual hours on the LED array (1h = 3600s).

2. Before running the program, the user has to input the current time into the variable 'hour' and 'minute', the default timing is 0:00. 
After initialising the program, the program is able to synchronise with the sun light automatically every day. Besides, the user also needs
to input the 'midday' variable to implement the adjustment for Daylight Saving Time. The 'midday' variable only takes two values: 12 and 13.
The solar midday is 12 o'clock in winter time and 13 o'clock in summer time. By default, 'midday' is 12.

3. Once the program is correctly setup, the program is able to fulfill all the requirements in the specification.


## Methodology 

### 1. Timing 

[Relevant files: timers.c/timers.h/interrupts.c/interrupts.h/main.c]

The timing is achieved by an ISR (Interrupt Service Routine). The 16-bit Timer0 is set up to achieve overflow in every second by employing the same
methodology in Lab 3 Exercise 2.3. Every second, when the timer overflows, the ISR will be waken and plus 1 second to the 'second' variable. In the Normal
mode, as the 'second' variable increases to 60, this variable will be cleared to 0 and the 'minute' variable will plus one. The 'hour' variable will
increment in the similar way. The timing will be displayed on the LED array in binary.

----------------------------------------------------------------------------------
### 2. Light-sensing

[Relevant files: interrupts.c/interrupts.h/comparator.c/comparator.h/main.c]

The light-sensing is also achieved by an ISR (Interrupt Service Routine) by using the comparator. Interrupt flag will be set on both the postive and
negative edge of the comparator. The interrupt will trigger the ISR to toggle the LED. 

----------------------------------------------------------------------------------
### 3. Energy-saving from 1 to 5 am

[Relevant files: smartlight.c/smartlight.h/main.c]

A function called 'one_to_five' is defined in document 'smartlight.c'. The functions will turn off the LED between 1 am and 5 am, despite the surrounding 
lighting condition. At 5 am, the comparator output is checked. If the surrounding is dark, the LED will turn on and if the surrounding is bright, the LED
will remain off. At summer time, sunrise is possible to occur at 5 am. 

----------------------------------------------------------------------------------
### 4. Adjusting the time by synchronising with the sun light 

[Relevant files: main.c]

Every day at 0 o'clock, variable 'min_accu' will start counting the accumulative minutes from 0. When the LED is toggled, the accumulative timing will be recorded 
in variables 'sun_set' and 'sun_rise', which are initially set to 0. After toggling, if the LED switches from off to on, the 'min_accu' will be recorded to 'sun_set', 
as the LED only turns on when the surrounding is dark; If the LED switches from on to off, the timing will be recorded to 'sun_rise' as the LED only turns off when 
the surrounding becomes bright. 

As soon as 'sun_set' is recorded, the difference between 'sun_set' and 'sun_rise' is computed, which is the length of daylight in minutes. It is assumed 
that sunrise and sunset are symmetrical about the solar midday. Therefore, the timing of the sunset will be overwritten as an adjustment. For example,
if the 'daylight' is 520 mins, according to the above assumption, the solar midday (12 at winter time) should occur 260 mins (520/2) before the 'sun_set' time. 
Thus, the current 'hour' will be overwrote to 16 (12 + 260/60 = 12 + 4) and the current 'minute' will be overwrote to 20 (260 % 60 = 20), hence 16:20. With this
method, every day the timer will be adjusted and synchronised at the sunset time. After the time is synchronised, 'sun_rise' and 'sun_set' will be cleared to 0 and
the 'daylight' will be stored in 'daylight_pre', which is the daylight of the previous day.

----------------------------------------------------------------------------------
### 5. Adjusting for Daylight Saving Time

[Relevant files: smartlight.c/smartlight.h/main.c]

A function called 'daylight_saving_time' is defined in document 'smartlight.c'. It is assumed that the solar midday is 13 pm in summer and 12 pm in winter. This 
function will check the daylight time for two consecutive days. If both days have a daylight time greater and equal to 11 hours, the program will set 'midday' to 13
(summer time). If both days have a daylight time less than 11 hours, the program will set 'midday' to 12 (winter time). Be default, midday = 12.

----------------------------------------------------------------------------------
### 6. Disturbance prevention from unexpected light sources and obstacles

[Relevant files: main.c]

If there are unexpected light sources and obstacles (may block light), the program may record wrong 'sun_rise' and 'sun_set' time, which will lead to a mistake in 'daylight'
calculation and sunlight synchronising process. The program sets two IF statements to limit the effects of these disturbances. Firstly, since the 'min_accu' only starts 
counting the accumulative minutes from 0 am in the morning, the 'sun_rise' time should always be shorter than 'sun_set' time. If 'sun_rise' > 'sun_set', the time correction 
will not be executed. Secondly, if the 'daylight' is too short (less than 4 hours), it is likely that a wrong signal is recorded. Thus, the time correction will not be 
executed and the 'daylight' will be set to be the same as the previous day. 
     

## Test

### Test 1 (Requirement 1 & 2 & 3):

Anytime except (1 to 5 am):
- Cover LDR (LED ON)
- Uncover LDR (LED OFF)

Between 1 to 5 am: 
- Cover LDR (LED OFF)
- Uncover LDR (LED OFF)

At 5 am (At summer times, sun rise could be before 5 am.): 
- Cover LDR (LED ON)
- Uncover LDR (LED OFF)

----------------------------------------------------------------------------------
### Test 2.1 (Requirement 5 & 6)

Condition 1: Clock too fast

Assume at winter time, ideally, if sun rises at 8, midday is 12 and sun sets at 16.
If the clock is too fast, it might detects sun rise at 8 and sun set at 18.

- Uncover at 8
- Cover at 18

Time will be overwrote to 12 + (18 - 8)/2 = 17

LED will show 17, 18, 19, 20, 21 ...

----------------------------------------------------------------------------------
### Test 2.2 (Requirement 5 & 6)

Condition 2: Clock too slow

Assume at winter time, ideally, if sun rises at 8, midday is 12 and sun sets at 16.
If the clock is too slow, it might detects sun rise at 8 and sun set at 14.

- Uncover at 8
- Cover at 14

Time will be overwrote to 12 + (14 - 8)/2 = 15

LED will show 15, 16, 17, 18, 19 ...

----------------------------------------------------------------------------------
### Test 3.1 (Requirement 4)

Condition 1: From winter time (midday = 12) to summer time (midday = 13)

1st Day:
- Uncover at 6
- Cover at 18
Time will be overwrote to 12 + (18 - 6)/2 = 18

LED will show 18, 19, 20, 21, 22 ...

2nd Day:
- Uncover at 6
- Cover at 22
Time will be overwrote to 13 + (22 - 6)/2 = 21

LED will show 21, 22, 23, 0, 1 ...

Since both two days have a daylight time >= 11h, the midday on the 2nd day will be shifted to 13 due to the transition to summer time.

----------------------------------------------------------------------------------
### Test 3.2 (Requirement 4)

Condition 2: From summer time (midday = 13) to winter time (midday = 12) 

1st Day:
- Uncover at 8
- Cover at 16
Time will be overwrote to 13 + (16 - 8)/2 = 17

LED will show 17, 18, 19, 20, 21 ...

2nd Day:
- Uncover at 8
- Cover at 18
Time will be overwrote to 12 + (18 - 8)/2 = 17

LED will show 17, 18, 19, 20, 21 ...

Since both two days have a daylight time < 11h, the midday on the 2nd day will be shifted to 12 due to the transition to winter time.

----------------------------------------------------------------------------------
### Test 4 (Additional Features)

Due to errors, if the daylight time is less than 4 hours, the daylight time will be assumed to be the same as the previous day
and time correction will not happen.

- Uncover at 8
- Cover at 10

LED will show 10, 11, 12, 13, 14 ... (Unaffected)








