# Nine Digit Sperry Neon Display Clock

Here's a cute little clock I designed that uses a Sperry neon display -- sometimes known as a Panaplex display. Please note this is not a fully completed project. Although I fabbed out boards and got the display working, I never got around to finishing the firmware.

![A small circuit board with a Panaplex display plugged in](https://github.com/schlae/SperryClock/blob/main/NineDigit.jpg)

The particular display in this clock is a Sperry SP-425-09, but the basic design will probably work with other Panaplex displays as well.

The brains of the operation is an ATMega328P with a 16MHz crystal. You can load it with the Arduino bootloader and write programs for it using the Arduino IDE, or you can program it directly using avr-gcc, AVR Libc, and avrdude.

[Here is the schematic.](https://github.com/schlae/SperryClock/blob/main/NineDigit.pdf)

The basic design uses 9 transistors to multiplex the segment signals to each of the digits in the display. Typical Panaplex displays must be multiplexed otherwise the segment cathode material will start to plate out and darken the display, so avoid operating the display with just a single digit turned on continuously.

The [firmware](https://github.com/schlae/SperryClock/blob/main/NineDigit.ino) is incomplete but does include all the code required to drive the multiplexed display.

Still to be done:

* Driver for the DS3231
* Button handling
* Clock logic 

## Fabrication and Assembly Notes
This is a two-layer board, outer dimensions of 115mm x 50mm. 

The Sperry display should be installed in a socket. I've specified a part from EDAC but you might be able to find a cheaper one. The pin spacing is 0.156" (3.96mm).
