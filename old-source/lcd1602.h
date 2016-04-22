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

#include <stdint.h>

#ifndef __LCD1602_H__
#define __LCD1602_H__

#define LED_PIN 0

#define LCD_ADDR 0x20
#define LCD_COMMAND 0
#define LCD_DATA 1

/*
 * LCD initialization
 */
void lcd1602_init(void);

/**
 * Sending byte to lcd
 * @c: byte
 * @rs: type of data
 */
void lcd1602_send_byte(uint8_t c, uint8_t rs);

/**
 * Sending char to lcd
 * @c: char
 */
void lcd1602_send_char(uint8_t c);

/*
 * Clear LCD
 */
void lcd1602_clear(void);

/**
 * Move cursor on lcd
 * @col: X
 * @row: Y
 */
void lcd1602_goto_xy(uint8_t col, uint8_t row);

/**
 * Writing string on the lcd
 * @str: string
 */
void lcd1602_send_string(const char *str);

#endif
