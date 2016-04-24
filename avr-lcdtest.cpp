/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2016 Kenji Rikitake
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* #define F_CPU (16000000UL) */

#include "AVRTools/ArduinoPins.h"
#include "AVRTools/InitSystem.h"
#include "AVRTools/SystemClock.h"
#include "AVRTools/I2cMaster.h"
#include "AVRTools/I2cLcd.h"
#include "AVRTools/USART0.h"

I2cLcd lcd;
char number[12];

int main() {

    int c;

    initSystem();
    initSystemClock();
    I2cMaster::start();
    USART0::start(9600);

    setGpioPinModeOutput(pPin13);
    setGpioPinLow(pPin13);

    lcd.init();
    lcd.clear();
    lcd.home();
    lcd.autoscrollOff();
    lcd.setBacklight(I2cLcd::kBacklight_White);
    for(;;) {
        if (-1 != (c = USART0::read())) {
            if (c == 12) {
                // CTRL/L: clear display
                lcd.clear();
                lcd.home();
            } else if (c == 16) {
                // CTRL/P: set upper row
                lcd.setCursor(0, 0);
            } else if (c == 14) {
                // CTRL/N: set lower row
                lcd.setCursor(1, 0);
            } else if (c < 32) {
                // do nothing for all the other control codes
                // (CR and LF are also ignored)
            } else {
                lcd.print((char)c);
            }
        }
    }

}
