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
/*
 * Bearfort sensor firmware
 * for Arduino UNO R3 and
 * Bearfort sensor shield
 * (ADT7410 on TWI/I2C,
 *  LM60 (or LM61) on ADC0 to ADC3 (four LM60s)
 * by Kenji Rikitake
 */

/* #define F_CPU (16000000UL) */

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>

/* UART library */

#include "uart.h"

FILE uart_str = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

/* I2C library */

#include <util/twi.h>
#include <alloca.h>
#include "i2c.h"

// LCD1602 library

#include "lcd1602.h"

/* initialize IO ports */

static void ioinit(void) {

    // disable interrupt before initialization
    cli();

    // forced initialization of unnecessary interrupts

    MCUCR = 0x00;
    EICRA = 0x00;
    EIMSK = 0x00;
    TIMSK0 = 0x00;
    TIMSK1 = 0x00;
    TIMSK2 = 0x00;
    PCMSK0 = 0x00;
    PCMSK1 = 0x00;
    PCMSK2 = 0x00;
    PCICR = 0x00;
    PRR = 0x00;

    // TXD/PD1/Pin 1 to output
    // RXD/PD0/Pin 0 to input 

    DDRD = 0xfe;
    PORTD = 0xff;

    // PCx all input, no pullup
    DDRC = 0x00;
    PORTC = 0x00;

    // PB5 = LED on Arduino 2009
    DDRB = 0xff;
    PORTB = 0x3f;

    /* USART 0 */
    /* no USART IRQ, disable TX/RX */
    UCSR0B = 0x00;
    /* clk 1x */
    UCSR0A = 0x00;
    /* async, no parity, 8bit */
    UCSR0C = 0x06;
    /* see uart.c */
    uart_init();

    /* Timer 0 */
    /* timer stop */
    TCCR0B = 0x00;
    /* reset counter */
    TCNT0 = 0;
    /* no external output, CTC */
    TCCR0A = 0x02;

    /* enable interrupt for avr-i2c library */
    sei();

    /* initialize I2C */
    i2c_init();

    // initialize lcd1602
    lcd1602_init();
}

/* main function */

int main() {

    /* initialize ports, timers, serial, and IRQ */
    ioinit();
    stdin = &uart_str;
    stdout = &uart_str;

    for (;;) {

        lcd1602_clear();
        lcd1602_goto_xy(0, 0);
        lcd1602_send_string("Hello,");
        lcd1602_goto_xy(0, 1);
        lcd1602_send_string("World!");

        _delay_ms(10000);
    }
    /* NOTREACHED */
}

/* end of code */
