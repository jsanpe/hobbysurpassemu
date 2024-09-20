# hobbysurpassemu
Emulates a programming card for Hobby Surpass ESCs using an Arduino

This project uses de half duplex Serial library for Arduino available here: https://github.com/nickstedman/SoftwareSerialWithHalfDuplex
I have copied the relevant files in this repo, so there is no need to download any other files to compile this project.

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

## How it works
The Surpass Hobby ESC communicates with the programming card using a simple UART-ish protocol with 1 start bit, 8 data bits, and 1 stop bit (no parity). The baud rate for the communication is not clear. While my oscilloscope reveals a bps of 827 I ended up lowering it down to 820 to make it fully reliable, but your mileage may vary. The project includes a lot of logging through the serial port to allow for verification of the codes received from the ESC. Perhaps depending on your ESC model, or on your arduino board characteristics you may need to adjust this number slightly up or down.

The protocol used by the ESC and the programming card is pretty straightforward. There is a handshake process, where the ESC probes to see if there is any programming card attached. If the handshake is successfull, then the ESC waits for read/write commands from the programming card. Here is the detail:

1 - Handshake: The ESC sends two bytes through the shared line 0x1B 0xFF. The programming card returns another two bytes 0x1C 0xFF. This finishes the handshake, the ESC is now ready to receive commands
2 - Read operation: The programming card can get the value for any parameter in the ESC. Parameters have numbers from 0 to 4 in the programming card that I am using. It is possible to read parameters outside this range, however their semantics (if they even exist) is not known. To get the value of a parameter, the programming card sends 0x1C followed by a byte with the parameter number to query, and ended with a 0x00. For instance, 0x1C 0x02 0x00 would query the ESC for the value for the 3rd parameter (remember that params are 0 indexed). The ESC responds to this message with the following two bytes: 0x1B Value
3 - Write operation: The programming card can set the value for any parameter in the ESC. Similarly to the read operation, only parameter in the 0 to 4 range have known semantics. To save a new value for a parameter in the ESC the message is 0x1B param value. For instance, 0x1B 0x02 0x01 would set the 3rd parameter to value 1. The ESC responds to this message with the following two bytes if the operation was successfull: 0x1B 0x0A

## Parameters and Values
The card that I'm using to program my Surpass Hobby ESC allows for setting 5 parameters:
- Parameter 0 - Low Voltage Cut-Off: 3.1V (0), 2.9V, (1), 2.7V (2), 0V (3)
- Parameter 1 - Start Mode (aka Punch): Medium (0). Soft (1), Strong (2)
- Parameter 2 - Max Brake Force (ESC <= 60Amp): 50% (0), 25% (1), 75% (2), 100% (3)
- Parameter 2 - Max Brake Force (ESC >= 80Amp): 25% (0), 50% (1), 75% (2), 100% (3)
- Parameter 3 - Max Reverse Force: 25% (0), 50% (1), 75% (2), 100% (3)
- Parameter 4 - Neutral Range: Narrow 9% (0), Normal 6% (1), Wide 12% (2)

All parameters default to their 0 value.
