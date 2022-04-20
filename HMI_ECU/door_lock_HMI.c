/*
 * door_lock.c
 *
 *  Created on: Jan 29, 2020
 *      Author: Basel Ashraf
 */
#include "door_lock_HMI.h"
uint32 enterPassword(void)
{
	uint32 password = 0;
	uint8 counter=0;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"enter password");
	LCD_goToRowColumn(1,0);
	while(1)
	{

		_delay_ms(1400); /* delay for slow down input from kaypad */
		uint8 buttonPressed = KeyPad_getPressedKey(); /* get key from user */
		if(buttonPressed >= 0 && buttonPressed <= 9)
		{
			password = (password * 10) + buttonPressed; /* form a 4 byte value */

			counter++; /* counter for limitition the input Password */

			LCD_displayCharacter('*'); /* print '*' instead of the real number */


		}
		if(counter == 5)
		{
			do
			{
				/* stop the user from entering any more input and waiting to press enter to leave loop */
			}
			while(KeyPad_getPressedKey() != '#');
			break;
		}

		if(buttonPressed == '#')
		{
			break; /* leave loop when entering enter on keyboard */
		}

	}
	_delay_ms(100);

	return password; /* return the entered value from user */

}

/* enter password again for confirmation */
uint32 RenterPassword(void)
{
	uint32 password = 0;
	uint8 counter=0;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Renter password");
	LCD_goToRowColumn(1,0);
	while(1)
	{
		_delay_ms(1400);
		uint8 buttonPressed = KeyPad_getPressedKey();
		if(buttonPressed >= 0 && buttonPressed <= 9)
		{
			password = (password * 10) + buttonPressed;
			counter++;
			LCD_displayCharacter('*');


		}
		if(counter == 5)
		{
			do
			{

			}
			while(KeyPad_getPressedKey() != '#');
			break;
		}

		if(buttonPressed == '#')
		{
			break;
		}

	}
	_delay_ms(100);
	return password;
}


uint8 doorOptions(void)
{
	uint8 buttonPressed;
	bool rotateMotorFlag = '*';
	LCD_clearScreen();
	LCD_goToRowColumn(0,0);
	LCD_displayCharacter('*');
	LCD_displayString(" open door");
	LCD_goToRowColumn(1,0);
	LCD_displayCharacter('#');
	LCD_displayString(" change pass");
	do
	{
		_delay_ms(1400);
		buttonPressed = KeyPad_getPressedKey();
		if(buttonPressed == '#')
		{
			return 0;
		}
	}
	while(buttonPressed != '*');
	return rotateMotorFlag;

}


uint32 changePassword(void)
{
	uint32 password = 0;
	uint8 counter=0;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"enter old pass");
	LCD_goToRowColumn(1,0);
	while(1)
	{

		_delay_ms(1400); /* delay for slow down input from kaypad */
		uint8 buttonPressed = KeyPad_getPressedKey(); /* get key from user */
		if(buttonPressed >= 0 && buttonPressed <= 9)
		{
			password = (password * 10) + buttonPressed; /* form a 4 byte value */

			counter++; /* counter for limitition the input Password */

			LCD_displayCharacter('*'); /* print '*' instead of the real number */


		}
		if(counter == 5)
		{
			do
			{
				/* stop the user from entering any more input and waiting to press enter to leave loop */
			}
			while(KeyPad_getPressedKey() != '#');
			break;
		}

		if(buttonPressed == '#')
		{
			break; /* leave loop when entering enter on keyboard */
		}

	}
	_delay_ms(100);

	return password; /* return the entered value from user */

}

void SendPassword(uint32 password)
{

	while(password != 0)
	{
		UART_sendByte(password % 10); /* sending byte by byte from a 4 byte password */
		password /= 10;
	}
	UART_sendByte(STOP_SENDING);

}
void recievePassword(uint8 recievedData , uint32 * recievedPassword)
{
	recievedData=0;
	*recievedPassword=0;

	while(recievedData != STOP_SENDING)
	{
		*recievedPassword =(*(recievedPassword) * 10) + recievedData; /* Rearrange bytes to form the 4 byte passowrd */

		UART_sendByte(HMI_READY); /* Control Mc is ready for the next byte to recieve */
		recievedData = UART_recieveByte();
	}

}

uint32 reverseNumber(uint32 number)
{
	uint32 reverse = 0;
	uint8 remainder = 0;

	while(number != 0)
	{
		remainder = number % 10;
		reverse = (reverse*10) + remainder;
		number /= 10;
	}

	return reverse;
}

void Mismatch(void)
{
	LCD_clearScreen();

	for(uint8 i = 0 ;i<10;i++)
	{
		LCD_displayStringRowColumn(0,0,"MISMATCH!");
		LCD_displayStringRowColumn(1,0,"Try Again");
		_delay_ms(500);
	}
}

void recieveEepromFlag(uint8 * Ptr)
{
	*Ptr = FALSE;
	UART_sendByte(HMI_READY);
	*Ptr = UART_recieveByte();


}

uint32 recieveEepromPassword(uint8 recievedData , uint32 * recievedPassword)
{
	recievedData=0;
	*recievedPassword=0;
	uint32 password=0;
	recievePassword(recievedData , recievedPassword);
	password = reverseNumber(*recievedPassword);
	return password;


}
void closeDoor(void)
{
	uint8 buttonPressed;
	LCD_clearScreen();
	LCD_goToRowColumn(0,0);
	LCD_displayCharacter('*');
	LCD_displayString(" close door");
	do
	{
		buttonPressed = KeyPad_getPressedKey();
	}
	while(buttonPressed != '*');
}


