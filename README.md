# Fruit-Warrior-Game
GaTech ECE 2035 Final Project Spring 2021

### Hardware Overview
The hardware configuration differs from the assignment schematic in several ways, and listed below are all the necessary connections:
* The microprocessor is an mbed LPC 1768 board, and the code is dependent on the device (mostly in the DMA and other hardware-specific portions of code)
* An lcd (uLCD-144-G2) must have its serial connected to pins 13 and 14, and reset to pin 25.
* Two push buttons in an open-drain configuration must be connected to pins 22 and 23. No pull up is required as the microcontroller is configured to provide that.
* A speaker, connected as appropriate to pin 18. Note that pin 18 provides analog volages between 0 and 3.3 volts, so a capacitor is needed to be placed in series to have proper audio.
* An SD card in SPI configuration connected to pins 5, 6, and 7 with the CS provided by pin 8.
* A joystick, correctly connected so that its output range is between 0 and 3.3 volts, connected to pins 19 and 20.

### Software Dependencies
This game's software is only dependent on the Mbed API, specifically it requires to be built with OS 6. This is not included in the repository and must be downloaded via the appropriate methods.

### Porting to other Mbed devices
This software will *only* work with the mbed LPC 1768, and below are detailed the areas that would need to be modified to port it to other devices:
* The DMA interface (hardware/dma.hpp or hardware/dma.cpp) uses data structures specific to the LPC 1768 as the native hardware abstraction layer that is supposed to be target independent does not abstract DMA functionality.
* All pinouts to hardware peripherals would need to be updated and are in a few files but should be mostly limited to main.cpp and game/inputs.cpp
* The analogOutAsync class (hardware/analogOutAsync.hpp) assumes there is only one analog out pin, this would need to be reworked to accomodate devices with more than one.

### Runtime Requirements
As mentioned in Hardware Overview, the game requires that those peripherals are connected properly. However, the game also requires the SD Card to have some specific content.
The game expects the following files to be on the SD card and will not run without them:
* highScores.csv - A CSV file with high scores. Should have a header with the fields "name" and "score", and 5 rows of entries below. If an entry is not present yet, it should be left blank but still present in the CSV.
* sounds/gameplay1.wav - A PCM sampled WAVE file for the last track in the gameplay loop
* sounds/gameplay2.wav - A PCM sampled WAVE file for the second to last track in the gameplay loop
* sounds/gameplay3.wav - A PCM sampled WAVE file for the second track in the gameplay loop
* sounds/gameplay4.wav - A PCM sampled WAVE file for the first track in the gameplay loop
* sounds/throw.wav - A PCM sampled WAVE file for the sound effect when the knife is thrown
* sounds/titleScreen.wav - A PCM sampled WAVE file for the music to play when the title screen is displayed

All WAVE files must be at exactly 22050hz sample rate, mono channel, and 16 bit signed.

Due to copyright restrictions, I cannot provide a prepared SD card folder with the content, it must be prepared independently for your own exclusive consumption.

### Video Demonstration
A brief video showing what the game is capable of: [Watch on YouTube](https://youtu.be/k7781Y48_8A)
