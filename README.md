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

## Functionality

1. The program has two mode, test and normal mode. In the Test mode, each hour will be displayed in seconds on the LED array(1h = 1s). 
In the Normal mode, each hour will be displayed in actual hours on the LED array (1h = 3600s).

2. Before running the program, the user has to input the current time into the variable 'hour' and 'minute', the default timing is 0:00. 
After initialising the program, the program is able to synchronise with the sun light automatically every day. Besides, the user also needs
to input the 'midday' variable to implement the adjustment for Daylight Saving Time. The 'midday' variable only takes two values: 12 and 13.
The solar midday is 12 o'clock in winter time and 13 o'clock in summer time. By default, 'midday' is 12.

3. This program is able to turn on the LED when the surrounding environment is dark and turn off the LED when the surrounding environment is 
bright

By default, midday = 12 (winter time). 

## Methodology 






## Test

Test 1 (Requirement 1 & 2 & 3):

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
Test 2.1 (Requirement 5 & 6)
Condition 1: Clock too fast
Assume at winter time, ideally, if sun rises at 8, midday is 12 and sun sets at 16.
If the clock is too fast, it might detects sun rise at 8 and sun set at 18.

- Uncover at 8
- Cover at 18

Time will be overwrote to 12 + (18 - 8)/2 = 17
LED will show 17, 18, 19, 20, 21 ...

----------------------------------------------------------------------------------
Test 2.2 (Requirement 5 & 6)
Condition 2: Clock too slow
Assume at winter time, ideally, if sun rises at 8, midday is 12 and sun sets at 16.
If the clock is too slow, it might detects sun rise at 8 and sun set at 14.

- Uncover at 8
- Cover at 14

Time will be overwrote to 12 + (14 - 8)/2 = 15
LED will show 15, 16, 17, 18, 19 ...

----------------------------------------------------------------------------------
Test 3.1 (Requirement 4)

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
Test 3.2 (Requirement 4)

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
Test 4 (Additional Features)

Due to error, if the daylight time is less than 4 hours, the daylight time will be assumed to be the same as the previous day
and time correction will not happen.

- Uncover at 8
- Cover at 10

LED will show 10, 11, 12, 13, 14 ... (Unaffected)








