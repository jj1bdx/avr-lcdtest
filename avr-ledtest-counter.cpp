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

// Test code for DFR0090 SPI 8-digit LED module
// With Pin D3: Latch, D4: Data, D5: Clock

#include "AVRTools/ArduinoPins.h"
#include "AVRTools/InitSystem.h"
#include "AVRTools/SystemClock.h"

uint8_t LEDtable[] = 
    {0xc0, 0xf9, 0xa4, 0xb0, 0x99,
     0x92, 0x82, 0xf8, 0x80, 0x90,
     0xff};

char number[12];

#define pLatch pPin03
#define pData  pPin04
#define pClock pPin05

void LEDsend(uint8_t data) {

    setGpioPinLow(pLatch);

    for (uint8_t count = 8; count > 0; count--) {
        if (data & 0x80) {
            setGpioPinHigh(pData);
        } else {
            setGpioPinLow(pData);
        }

        asm volatile("nop"); 
        setGpioPinHigh(pClock);
        asm volatile("nop"); 
        setGpioPinLow(pClock);

        data = data + data; // left shift
    }

    setGpioPinHigh(pLatch);
}

int main() {

    initSystem();
    initSystemClock();
    setGpioPinModeOutput(pLatch); 
    setGpioPinModeOutput(pData);
    setGpioPinModeOutput(pClock);
    setGpioPinHigh(pLatch);

    for(;;) {
        for(uint32_t counter = 0; counter < 100000000UL; counter++) {
            uint32_t d = counter;
            for(uint8_t digit = 0; digit < 8; digit++) {
                uint8_t i = d % 10;
                if (d == 0) {
                    i = 10;
                }
                LEDsend(LEDtable[i]);
                d = d / 10;
            }
            delay(5);
        }
    }

}
