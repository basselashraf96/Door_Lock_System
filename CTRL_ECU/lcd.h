/*
 * lcd.h
 *
 *  Created on: Dec 29, 2019
 *      Author: Basel Ashraf
 */

#include "common_macros.h"
#include "std_types.h"
#include <avr/io.h>
#include <util/delay.h>

#ifndef LCD_H_
#define LCD_H_

#define RS PB1
#define RW PB2
#define E PB3

/* LCD HW PINS */
#define LCD_CTRL_PORT PORTB
#define LCD_CTRL_PORT_DIR DDRB
#define LCD_DATA_PORT PORTA
#define LCD_DATA_PORT_DIR DDRA

/* LCD COMMANDS */
#define CLEAR_DISPLAY 0x01
#define FOUR_BITS_MODE  0x28
#define EIGHT_BITS_MODE 0x38
#define CURSOR_OFF 0x0C
#define CURSOR_ON 0x0E
#define SET_CURSOR_LOCATION 0x80


void LCD_sendCommand(uint8 command);
void LCD_displayCharacter(uint8 data);
void LCD_init(void);
void LCD_displayString(const char *Str);
void LCD_clearScreen(void);
void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str);
void LCD_goToRowColumn(uint8 row,uint8 col);
void LCD_intgerToString(int data);
#endif /* LCD_H_ */
