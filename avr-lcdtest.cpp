/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Kenji Rikitake
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

/* 
 * Show avrhwrng output as 1 <= X <= 6 value on the top line
 * The bottom line shows history of the past 14 attempts
 */

#include "AVRTools/ArduinoPins.h"
#include "AVRTools/InitSystem.h"
#include "AVRTools/SystemClock.h"
#include "AVRTools/I2cMaster.h"
#include "AVRTools/I2cLcd.h"
#include "AVRTools/SPI.h"
#include "AVRTools/USART0.h"

uint8_t LEDtable[] =
    {0xc0, 0xf9, 0xa4, 0xb0, 0x99,
     0x92, 0x82, 0xf8, 0x80, 0x90,
     0xff};

#define pLatch pSS   // pPin10
#define pData  pMOSI // pPin11
#define pClock pSCK  // pPin13

void LEDsend(uint8_t data) {

    setGpioPinLow(pSS);
    (void)SPI::transmit(data);
    setGpioPinHigh(pSS);
}

uint8_t rngdice() {

    uint8_t v = 0;
    bool notok = true;
    
    while (notok) {
        v = readGpioPinDigitalV(makeGpioVarFromGpioPin(pPin07));
        v = (v << 1) |
            readGpioPinDigitalV(makeGpioVarFromGpioPin(pPin06));
        delay(1);
        v = (v << 1) |
            readGpioPinDigitalV(makeGpioVarFromGpioPin(pPin07));
        v = (v << 1) |
            readGpioPinDigitalV(makeGpioVarFromGpioPin(pPin06));
        delay(1);
        v = (v << 1) |
            readGpioPinDigitalV(makeGpioVarFromGpioPin(pPin07));
        v = (v << 1) |
            readGpioPinDigitalV(makeGpioVarFromGpioPin(pPin06));
        // 0 <= v <= 63    
        if (v <= 59) {
            notok = false;
        }
    }

    return (v % 6) + 1;
}

int main() {
    I2cLcd lcd;
    uint8_t d;
    uint8_t p[15];
    uint8_t i;
    uint8_t c;

    initSystem();
    initSystemClock();
    I2cMaster::start();
    USART0::start(9600);
    SPI::enable();
    // Max 4MHz for slave, 8MHz for master
    SPI::configure(SPI::SPISettings(8000000,
                SPI::kMsbFirst, SPI::kSpiMode0));
    setGpioPinHigh(pSS);

    // PRNG bits
    setGpioPinModeInput(pPin07);
    setGpioPinModeInput(pPin06);

    for (i = 0; i < sizeof(p); i++) {
        p[i] = 0x30;
    }

    lcd.init();
    lcd.clear();
    lcd.home();
    lcd.autoscrollOff();
    lcd.setBacklight(I2cLcd::kBacklight_White);
    for(;;) {
        d = rngdice();
        lcd.setCursor(0, 0);
        lcd.print("Dice = ");
        lcd.print((int)d);
        lcd.setCursor(1, 0);

        for (i = 0; i < sizeof(p) - 1; i++) {
            p[i] = p[i + 1];
        }
        c = 0x30 + d;
        p[sizeof(p) - 1] = c;

        USART0::write((char)c);

        for (i = 0; i < sizeof(p); i++) {
            lcd.print((char)p[i]);
        }

	    for (i = sizeof(p) - 1; i >= (sizeof(p) - 8); i--) {
			LEDsend(LEDtable[p[i]-0x30]);
		}

        delay(1000);
    }

}
