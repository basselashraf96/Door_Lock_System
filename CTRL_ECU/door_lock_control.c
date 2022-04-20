/*
 * door_lock.c
 *
 *  Created on: Jan 29, 2020
 *      Author: Basel Ashraf
 */
#include "door_lock_control.h"
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

uint8 savePasswordToEEPROM(uint32 data , uint16 address , uint8 locations)
{
	uint32 mask = 0xFF; /* masking byte by byte from data */
	uint8 temp;
	uint8 shiftNumber = 0;
	for(uint8 i = 0 ; i <locations ; i++)
	{
		temp = mask & (data >> shiftNumber); /* taking 1 byte at a time */
		EEPROM_writeByte(address , temp);

		/* go to next location */
		address++;

		shiftNumber += 8; /* shift data to take the next 1 byte */
		_delay_ms(100); /* small delay for R/W */
	}
	return SUCCESS;
}
uint32 getPasswordFromEEPROM(uint16 address , uint8 locations )
{
	uint8 dataFromEEPROM;
	uint32 password = 0;
	address = address + (locations-1); /* accessing the last location of the data which are stored in */
	for(uint8 i=0 ; i < locations ; i++)
	{
		password = (password << 8); /*shifting the password 1 byte to get the next byte */
		EEPROM_readByte(address , &dataFromEEPROM);
		_delay_ms(100);
		password |= dataFromEEPROM; /* store the eeprom 1 byte data in 1 byte of the password */

		/* go to the next location */
		address--;



	}
	return password;


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

		UART_sendByte(CTRL_READY); /* Control Mc is ready for the next byte to recieve */

		recievedData = UART_recieveByte();
	}

}

void sendEepromFlag(uint16 address , uint8 *data)
{
	while(UART_recieveByte() != HMI_READY);
	EEPROM_readByte(address , data);
	UART_sendByte(*data);
}
void sendEepromPassword(uint16 address , uint8 locations , uint32 * data)
{

	*data = getPasswordFromEEPROM(address , locations);
	while(UART_recieveByte() != HMI_READY);
	SendPassword(*data);
}
