# Testing Procedure

Assume the device turns on at September (Summer DST time), 00:00am.

## Day 1 - Testing Day
The first day is the testing day to identify the season.
Assume sunrise at 06:30 sunset at 20:30:

- Block the LDR before the LED array displays 6 (night)
- Takes the hand off at 6
- Block the LDR at 20

So device know it's summer time, and solar midday is 13pm.

Now the testing day ends and LED RD7 should turns off so that it is ready to do the following tests.


## Day 2 - Synchronisation Test (clock is too fast)
Assume:

Ideally when sunrise at 8am, sunset should be 18pm, so the solar midday is 13pm.

Let's see what happens if the device detects sunrise at 8am and sunset at 20pm, where the solar midday is 14pm:

- Hand off at 8
- Block at 20

>>> LED stops at 23 for 2 seconds, and then go to the next day.(LED after block: 20, 21, 22, 23, 23, 0)
--------------------------------------------------------------------------------------

- Hand off at 8
- Block at 20
Time that will overwrite to = (20 - 8)/2 + 13 = 19

>>> (LED after block: 19, 20, 21, 22, 23, 0)



## Day 3 - Synchronisation Test (clock is too slow)
Let's see what happens if the device detects sunrise at 8am and sunset at 16pm, where the solar midday is 12pm:

- Hand off at 8;
- Block at 16;

>>> LED jumps to 1 for the next day. (LED after block: 16, 17, 18, 19, 20, 21, 22, 23, 1)
--------------------------------------------------------------------------------------

- Hand off at 8;
- Block at 16;
Time that will overwrite to = (16 - 8)/2 + 13 = 17

>>> (LED after block: 17, 18, 19, 20, 21, 22, 23, 24, 1)
--------------------------------------------------------------------------------------


## Day 4 & 5 - DST Adjustment Test (from summer time to winter time)
If sunset later than 17:40 for two consecutive days, it means winter is comming:

- Hand off at 8 (solar midday = 13)
- Block at 18
- Repeat again

>>> LED stops at 1 for 2 seconds (01:59 ~ 01:00).

Now it's winter and the solar midday is 12pm.


## Day 6 & 7 - DST Adjustment Test (from winter time to summer time)
If sunrise earlier than 6:00 for two consecutive days, it means summer is comming:

- Hand off when displaying 5
- Block at 19 (solar midday = 12)
- Repeat again

>>> LED jumps from 0 to 2 (00:59 ~ 02:00).


## *** Disturbance Test
This can be done at any time, the device will only respond to the correct sunrise and sunset.

For example if we make a incorrect light signal at Day 2:

- Hand off at 8
- Block at 12 (Disturbance)
- Hand off at 16 (Disturbance)
- Block at 20

>>> LED stops at 23 for 2 seconds, as expected.

The disturbance can occur either before sunrise, after sunset or during the day time.

