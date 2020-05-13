Edith Flores
OS I/O CapLock

Files:
led.c


--------led-----------------
led is a  kernel  module  that  turns  the  CAPS-lock  LED  on  and  off. The following operations can be supported:
•Readingfrom/dev/ledshould return to the user a small ASCII picture of a light bulb.
•Writing “on” to/dev/ledshould turn the CAPS-lock led on.
•Writing “off” to/dev/ledshould turn the CAPS-lock led off.
•Writing “reset” to/dev/ledshould return the CAPS-lock led to the correct state.

To run:
