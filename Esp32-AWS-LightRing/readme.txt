This contains the C++ code for the esp32.

This is intended to be used in vscode with the platformio extension.

The program
* Authenticates to aws using certificates. The certificates harded coded in the program are also for SSL encryption/trust purposes.
* Parses JSON in incoming messages to know what lights to turn on
* Uses the FastLED library to control the ws2811b leds (light ring of 16 lights)



