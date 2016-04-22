/* LCD1602 (HD44780) AVR library
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Rewritten by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 * Copyright (c) 2016 Kenji Rikitake.
 * Modified by Kenji Rikitake <kenji.rikitake@acm.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#include "lcd1602.h"
#include "i2c.h"
#include <util/delay.h>
#include <alloca.h>

static uint8_t lcd_led_pin;

void lcd_send_packet(uint8_t data) {
    i2c_txn_t *t;
    uint8_t msg[1];

    t = (i2c_txn_t *)alloca(sizeof(*t) + 2 * sizeof(t->ops[0]));
    i2c_txn_init(t, 1);
    i2c_op_init_wr(&t->ops[0], LCD_ADDR, msg, sizeof(msg));
    i2c_post(t);
    // Wait until completion of the transaction
    while (!(t->flags & I2C_TXN_DONE)) {}
}

void putnibble(uint8_t t)
{
	t <<= 4;
	lcd_send_packet(lcd_led_pin |= 0x04);
	_delay_us(50);
	lcd_send_packet(lcd_led_pin | t);
	lcd_send_packet(lcd_led_pin &= ~0x04);
	_delay_us(50);
}

void lcd1602_send_byte(uint8_t c, uint8_t rs)
{
	uint8_t highc = 0;
	highc = c >> 4;
	if (rs == LCD_COMMAND) {
		lcd_send_packet(lcd_led_pin &=~ 0x01);
    } else {
		lcd_send_packet(lcd_led_pin |= 0x01);
    }
	putnibble(highc);
	putnibble(c);
}

void lcd1602_send_char(uint8_t c)
{
	uint8_t highc = 0;

	highc = c >> 4;
	lcd_send_packet (lcd_led_pin |= 0x01);
	putnibble(highc);
	putnibble(c);
}

void lcd1602_init()
{
	lcd_led_pin = 0;
	putnibble(0b00000011);
	_delay_ms(5);
	putnibble(0b00000011);
	_delay_ms(5);
	putnibble(0b00000011);
	_delay_ms(1);
	putnibble(0b00000010);
	_delay_ms(1);
	lcd1602_send_byte(0x28, LCD_COMMAND);
	_delay_ms(1);
	lcd1602_send_byte(0x0C, LCD_COMMAND);
	_delay_ms(1);
	lcd1602_send_byte(0x06, LCD_COMMAND);
	_delay_ms(1);
	lcd_send_packet(lcd_led_pin |= 0x08);
	lcd_send_packet(lcd_led_pin &=~ 0x02);
}

void lcd1602_clear()
{
	lcd1602_send_byte(0x01, LCD_COMMAND);
	_delay_us(1500);
}

void lcd1602_goto_xy(uint8_t col, uint8_t row)
{
	uint8_t adr;

	adr = 0x40*row + col;
	adr |= 0x80;
	lcd1602_send_byte(adr, LCD_COMMAND);
}

void lcd1602_send_string(const char *str)
{
	uint8_t i;

	for(i = 0; str[i] != '\0'; i++)
		lcd1602_send_char((uint8_t)str[i]);
}
