# hobbysurpassemu
Emulates a programming card for Hobby Surpass ESCs using an Arduino

Uses de half duplex Serial library for Arduino available here: https://github.com/nickstedman/SoftwareSerialWithHalfDuplex
I copied the relevant files in this project for ease of use

## How to use
Compile and upload the .ino sketch to your arduino board.

You need to connect your Arduino board and the ESC using the cable normally attached to the receiver (three wire cable).
Use a jump wire to connect the GND between your Arduino and the ESC. Use a second wire to connect Pin 4 in your Arduino to the signal cable in your ESC.

DO NOT connect the voltage pin between the arduino and the ESC. The Arduino needs to be connected to your computer through USB to be able to send and receive commands, and it is powered directly from the USB bus. The ESC needs to be powered using a battery, as you would do if it was in your RC vehicle.

Once all the connections are done, open the Serial monitor in your Arduino IDE (or any other serial monitor) and use the default config at 9600 (8 bit, no parity). Reset your arduino and you will be see some messages about the status of the board. 

Switch on the ESC, the communication between both start and status should be 2 once the handshake is finished. At this point you can type commands following this syntax:

[Command][ARG1][ARG2]

All commands and args are single characters:

- Get current config: Use command "Q" without the quotes to read all the parameters in the ESC. Their values will be printed in your monitor.
- Get one parameter only: Use command "qY" with Y being a number between 0 and 9 (Note that only numbers up to 4 are real parameters of the ESC).
- Set one parameter: Use command "wYZ" with Y being the parameter number and Z the value (both Y and Z are numbers from 0 to 9).

The meaning of the parameters can be found in the PDF manual (available in this repo). For instance, parameter 0 is the cutoff voltage and has the following values: 0 for 3.1V, 1 for 2.9V, etc.

