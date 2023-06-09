# PT2262 RF Signal Encoder Simulation using Arduino Nano

This project aims to simulate the PT2262M4 RF signal encoder module using an Arduino Nano. The module is capable of encoding RF signals for controlling devices. It utilizes the Atmel 328P microcontroller on the Arduino Nano board, 4 buttons as input for encoding 4 control commands, a 315MHz RF modulation module, and is powered by a 9V battery.

## Features

- Simulates PT2262M4 RF signal encoding using Arduino Nano
- Utilizes the Atmel 328P microcontroller on the Arduino Nano board
- 4 buttons for inputting control commands
- 315MHz RF modulation module for signal transmission
- Powered by a 9V battery

## Hardware Requirements

- Arduino Nano board with Atmel 328P microcontroller
- 4 buttons for inputting control commands
- 315MHz RF modulation module
- 9V battery for power supply

## Software Requirements

- Programming language: C
- PlatformIO for development
- Arduino.h library for Arduino-specific functions

## Installation and Setup

1. Connect the hardware components as per the circuit diagram.
2. Install PlatformIO IDE on your development machine.
3. Create a new PlatformIO project and initialize it with the Arduino Nano board.
4. Copy the `pt2262.h` library file into your project's `lib` directory.
5. Copy the `main.cpp` code into your project's `src` directory, replacing the existing `main.cpp` file.
6. Compile and upload the code to the Arduino Nano board using PlatformIO.

## Summary of Main Functions

Here is a summary of the main functions in the `pt2262.h` library:

### `void pt2262Init(st_pt2262 *PT2262, uint8_t led_pin, uint8_t data_out_pin, uint8_t data_input_pin0, uint8_t data_input_pin1, uint8_t data_input_pin2, uint8_t data_input_pin3)`
- Initializes the `st_pt2262` structure and sets up the control pins.
- `led_pin`: Pin for the LED (output).
- `data_out_pin`: Pin for data output (output).
- `data_input_pin0` to `data_input_pin3`: Pins for data input (input).

### `void sendBit(st_pt2262 *PT2262, uint8_t PT2262_BIT)`
- Sends a bit through the PT2262 module.
- `PT2262_BIT`: Bit value to send (`PT2262_BIT_0`, `PT2262_BIT_1`, `PT2262_BIT_FLOAT`, or `PT2262_BIT_SYNC`).

### `void sendFrame(st_pt2262 *PT2262, uint8_t dataCode)`
- Sends a data frame (code frame) through the PT2262 module.
- `dataCode`: The 4-bit data to be sent.

### `void getState(st_pt2262 *PT2262)`
- Reads the state of the buttons into the `st_pt2262` structure.
- `PT2262->DATA_STATE`: Stores the data state (`0` or `1`) of the buttons.

Please refer to the source code and documentation for more detailed information about the functions and how to use them.

## Usage

1. Connect the Arduino Nano to your computer using a USB cable.
2. Open the serial monitor in PlatformIO to view debug output (optional).
3. Press the buttons connected to the Arduino Nano to input control commands.
4. The encoded RF signal will be generated and transmitted using the RF modulation module.
5. Ensure that the RF receiver is configured to receive and decode the transmitted signal.

Note: Adjust the pin numbers and other configuration parameters in the code according to your specific hardware setup.

## Contributing

Contributions to this project are welcome. You can submit bug reports, feature requests, or pull requests on the GitHub repository.

## License

This project is licensed under the [MIT License](LICENSE).
