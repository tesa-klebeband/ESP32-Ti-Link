# ESP32-Ti-Link
ESP32-Ti-Link is an open-source project that enables communication between a Texas Instruments graphing calculator and a PC using an ESP32, acting as a link cable.
## Features
- Transfer data between TI graphing calculators and a PC
- Supports transfer of variables, programs, applications, the OS and more
- Detects communication errors

## Getting started
Clone this repository using:

`git clone https://github.com/tesa-klebeband/ESP32-Ti-Link.git`

### Adapter setup
1. Strip the insulation of a 2.5mm cable or solder wires to a 2.5mm plug
2. Connect the ground or shield of the cable to the ESP32's GND pin
3. Connect the tip wire to pin 23, or specify your own by changing the TIP definition in main.cpp
4. Connect the ring wire to pin 22, or specify your own by changing the RING definition
5. Download and install the CP210x driver
6. Install PlatformIO and the PlatformIO extension in vscode
7. Open the repository in vscode
8. Connect the ESP32 and upload the code

### Software setup
1. Download and install TiConnect
2. Connect the calculator to the ESP32 with the 2.5mm plug
3. Connect the ESP32 to your computer and start the TiConnect software
4. Whem promted select the COM port that is assigned to your ESP32

## Using multiple LEDs as transmission indicators
In order to visualize the direction of the transmission, you can change the following declarations to your liking:
- LED_RX_PIN: Indicates the calculator transmitting
- LED_TX_PIN: Indicates the computer transmitting
- LED_RX_TMT_PIN: Indicates a timeout when recieving data from the calculator
- LED_TX_TMT_PIN: Indicates a timeout in the calculator acknowledging to a transmission from the computer

## Timeouts
In order to continue when a timeout occurs, you can press the BOOT button on the ESP32. If you experience issues with timeouts while reading and/or writing to the calculator, you can try changing the following:
- TIMEOUT_RX_US: Recieve timeout in microseconds
- TIMEOUT_TX_US: Acknowledge timeout in microseconds
- RETRY_PIN: Pin number of the 'continue on timeout' pin

## Known issues
- Doesn't work with TILP-II (working on a fix)

## License
All files within this repo are released under the GNU GPL V3 License as per the LICENSE file stored in the root of this repo.
