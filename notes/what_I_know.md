[facts]
* 4 AA batteries in series for 6ish volts.
* D1 led seems to blink intermentinly with no reason and DS1
* pressing lock button repeatidly has no effect it seems to know its state. based on the spring actuatd by the lock.
* there are three main boards.
** Board 1 is encase it holds front panel touch panel 2x7 cable interface to Board 2.
** Board 2 seems to be the logic section of the lock, PCB: Main Lever P/N: 5042441-B.  It has a 4 pin dip switch, sw1 button.  U1 brain micro, y1 crystal.  the motor driver for the lock.  As well as the power supply for all three boards.  It also has a daughterboard slot
** Board 3 is the zwave wireless addon board.  it has a zwave module on boar as well as a spi flash chip and i2c rtc.  and a 14 pin connector to the main board.

*left battery terminal is ground right is voltage 
* 5v idles at 24mamp sleeps at lower than my tool can measure 
* locking the door takes around 145 mamps 
* throwing the lock switch open or closed seems to trigger an interupt on the microcontroller waking it form sleep
* the lock button on front certainly trigger interupt
* blinking all leds draws around 50 mamps
* 3 volts it doesn't try to turn motor.  continues to run electronics and complains about low bat via leds
* 4.2 door still works
* stepping voltage back up doesnt make it work.  makes me think its only doing adc check on battey voltage every so often or on boot? or never again after it fails?
* if battery is less than 4.2 at start it wont work.  it does turn all the way down to 3.5 volts before adc check tells it to stop trying.
