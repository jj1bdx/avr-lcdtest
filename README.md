# avr-lcdtest

Arduino Uno R3 C++ code for testing Adafruit RGB LCD connected through I2C.

## Code example

* The current code (in avr-lcdtest-serialdisp.cpp and avr-lcdtest.cpp) works as an external 16x2 LCD display connected to the USB serial port (9600bps)
* See lcd-loadav.sh for how to show the load average of a Unix-based system (e.g., OS X) periodically (for every 5 seconds)

## Other code examples

* avr-lcdtest-bearfort.cpp: Standalond LCD temperature display code with a [bearfort](https://github.com/jj1bdx/bearfort/) sensor
* avr-lcdtest-ledflash.cpp: Basic LED flashing code
* avr-lcdtest-simple.cpp: Basic LED flashing and LCD counter code

## How to build

    # build libavrtools.a 
    (cd AVRTools && make clean && make)
    # build and link
    make

## Acknowlegment

* [Igor Mikolic-Torreira](https://github.com/igormiktor): most of the work in this repository has been done through his [AVRTools](https://github.com/igormiktor/AVRTools) C++ library.

## LICENSE

GPLv3.

AVRTools is released under GPLv3, so this repository is also licensed as GPLv3.

My code modules are released under the MIT License.
