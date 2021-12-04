# fastled-bicycle-light
A led strip light for the front box of my bicycle. Street legal and decorative light effects with FastLED library for a WS2811 led strip.

## Summary
I have a plastic box installed on the handlebar of my bicycle. To increase visibility and add some glow to Critical Mass rides, I added an LED strip around it, starting at the rear left, then wrapping around the left, front and right sides, to end at the rear right. There are two buttons to control the light mode and the brightness. The code can be easily adapted to different strip and section lengths.

### Light Modes
There are 8 light modes:

* 0: White to the front, yellow to the sides, red to the rear. This is used as a street legal bicycle light. It supplements, but does not replace lights fitted to the bicycle!
* 1: Like 0, but rotated 90°, for when the box is installed sideways on the rear rack
* 2: Like 0, but rotated 180°, just to have all 4 possible orientations
* 3: Like 0, but rotated 270°, for when the box is installed sideways on the rear rack (on the other side)
* 4: Warm white all around, can be used as a camping light
* 5: Orange blinking hazard light for emergency situations
* 6: Orange rotating warning light, similar to construction equipment
* 7: A random mix of fancy light animations. Some animations are from the FastLED examples, some are written by myself.

### Brightness
There are 6 brightness levels:

* 0: full brightness
* 1: half
* 2: quarter
* 3: 1/8
* 4: half, but fewer LEDs
* 5: 1/8, but fewer LEDs
In levels 4 and 5, which LEDs are off depends on the light mode: Modes 0-3 keep 4 LEDs in each corner on, all other modes have every 2nd LED on.


## Requirements

### Hardware
* Pretty much any Arduino board (I use the Pro Micro)
* A battery or power bank for power supply
* An LED strip with programmable LEDs (such as Adafruit Neopixel: https://www.adafruit.com/category/183). I use WS2811. The data line is connected to Pin 2 of the Arduino.
* Ideally a power button, so you don't have to disconnect the battery to turn it off
* a push button (ideally waterproof) to change the brightness, connected to pin 4
* another push button to change the light mode, connected to pin 6
* ideally a waterproof case for the Arduino and power supply

### Software
* Arduino IDE: https://www.arduino.cc/en/software
* FastLED library: In the Arduino IDE, go to Tools -> Manage Libraries, and search for FastLED