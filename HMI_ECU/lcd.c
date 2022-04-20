/*
 * lcd.c
 *
 *  Created on: Dec 29, 2019
 *      Author: Basel Ashraf
 */
#include "lcd.h"
void LCD_init(void)
{
	LCD_CTRL_PORT_DIR|=(1<<RS)|(1<<E)|(1<<RW);
	LCD_DATA_PORT_DIR=0xff;
	LCD_sendCommand(EIGHT_BITS_MODE);
	LCD_sendCommand(CURSOR_OFF);
	LCD_sendCommand(CLEAR_DISPLAY);
}

void LCD_sendCommand(uint8 command)
{
	CLEAR_BIT(LCD_CTRL_PORT,RS);
	CLEAR_BIT(LCD_CTRL_PORT,RW);
	_delay_ms(1);
	SET_BIT(LCD_CTRL_PORT,E);
	_delay_ms(1);
	LCD_DATA_PORT = command;
	_delay_ms(1);
	CLEAR_BIT(LCD_CTRL_PORT,E);
	_delay_ms(1);
}

void LCD_displayCharacter(uint8 data)
{
	SET_BIT(LCD_CTRL_PORT,RS);
	CLEAR_BIT(LCD_CTRL_PORT,RW);
	_delay_ms(1);
	SET_BIT(LCD_CTRL_PORT,E);
	_delay_ms(1);
	LCD_DATA_PORT = data;
	_delay_ms(1);
	CLEAR_BIT(LCD_CTRL_PORT,E);
	_delay_ms(1);
}

void LCD_displayString(const char *Str)
{
	uint8 i=0;
	while(Str[i]!='\0')
	{
		LCD_displayCharacter(Str[i]);
		i++;
	}
}

void LCD_clearScreen(void)
{
	LCD_sendCommand(CLEAR_DISPLAY);
}

void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str)
{
	LCD_goToRowColumn(row,col);
	LCD_displayString(Str);
}
void LCD_goToRowColumn(uint8 row,uint8 col)
{
	uint8 address;
	switch(row)
	{
	case 0 :
		address=col;
		break;
	case 1 :
		address=col+0x40;
		break;
	case 2 :
		address=col+0x10;
		break;
	case 3 :
		address=col+0x50;
		break;
	}
		LCD_sendCommand(address|SET_CURSOR_LOCATION);

}

void LCD_intgerToString(int data)
{
	char buff[16];
	itoa(data,buff,10);
	LCD_displayString(buff);
}
