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

#include "AVRTools/Analog2Digital.h"
#include "AVRTools/ArduinoPins.h"
#include "AVRTools/InitSystem.h"
#include "AVRTools/SystemClock.h"
#include "AVRTools/I2cMaster.h"
#include "AVRTools/I2cLcd.h"

/* requires beaufort sensor shield */
/* temperature macros */

#define TC1 ((double)1.065) // 1.090/1023*1000
#define LM60TEMP(x) ((((double)x) * TC1 - 424.0) / 6.25)
#define ADTTEMP(x) (((double)((int16_t)x)) / (double)128.0)

I2cLcd lcd;
char number[12];

int main() {

    int16_t a0, a1, a2, a3;

    initSystem();
    initSystemClock();
    initA2D(kA2dReference11V);
    I2cMaster::start();

    setGpioPinModeInput(pPinA00);
    setGpioPinModeInput(pPinA01);
    setGpioPinModeInput(pPinA02);
    setGpioPinModeInput(pPinA03);

    setGpioPinModeOutput(pPin13);
    setGpioPinLow(pPin13);

    lcd.init();
    lcd.clear();
    lcd.home();
    lcd.autoscrollOff();
    lcd.setBacklight(0);
    delayMilliseconds(1000);
    lcd.setBacklight(I2cLcd::kBacklight_Green);
    lcd.displayTopRow("A0-A3 values:");
    delayMilliseconds(1000);
    lcd.setBacklight(I2cLcd::kBacklight_White);

    for(;;) {
        for(uint8_t color = 0; color < 8 ; color++) {
            a0 = (int16_t)(LM60TEMP(
                   readGpioPinAnalogV(makeGpioVarFromGpioPinAnalog(
                           pPinA00))) + 0.5);
            a1 = (int16_t)(LM60TEMP(
                   readGpioPinAnalogV(makeGpioVarFromGpioPinAnalog(
                           pPinA01))) + 0.5);
            a2 = (int16_t)(LM60TEMP(
                   readGpioPinAnalogV(makeGpioVarFromGpioPinAnalog(
                           pPinA02))) + 0.5);
            a3 = (int16_t)(LM60TEMP(
                   readGpioPinAnalogV(makeGpioVarFromGpioPinAnalog(
                           pPinA03))) + 0.5);
            lcd.setCursor(1, 0);
            lcd.print(a0);
            lcd.print((char)0x20);
            lcd.print(a1);
            lcd.print((char)0x20);
            lcd.print(a2);
            lcd.print((char)0x20);
            lcd.print(a3);
            lcd.print((char)0x20);
            writeGpioPinDigital(pPin13, color % 2);
        }
    }

}
