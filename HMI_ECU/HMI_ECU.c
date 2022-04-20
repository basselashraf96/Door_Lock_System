/*
 * main.c
 *
 *  Created on: Jan 29, 2020
 *      Author: Basel Ashraf
 */


#include "door_lock_HMI.h"

/************************************************************************************
 * 								GLOBAL VARIABLES
 ************************************************************************************/
uint8 Tempsecond = 0;
uint8 second = 0;
uint8 f_seconds = 0;
uint8 f_minutes = 0;

/************************************************************************************
 * 								INTERRUPT SERVICE ROUTINE
 ************************************************************************************/
void timer1_count(void)
{
	Tempsecond++;
	second++;
	if(Tempsecond == 15)
	{
		Tempsecond = 0;
		f_seconds = 1;
	}
	if(second == 60)
	{
		second = 0;
		f_minutes = 1;
	}

}
/************************************************************************************
 * 								       MAIN CODE
 ************************************************************************************/

int main(void)
{
	bool CreatingPasswordComplete = FALSE;
	uint32 password = 0;
	uint32 repassword = 0;
	bool eepromFlag = 0;
	uint32 eepromPassword = 0;
	uint32 recievedPassword = 0;
	uint8 recievedData = 0;
	uint8 doorOption = 0;
	bool changePasswordFlag = FALSE;
	uint8 MismatchCounter = 0;
	bool UpdateEeprom = FALSE;
	bool closeDoorFlag = FALSE;

	SREG = (1<<7); /* Enable I-Bit */
	timer1Config timer1_config = {CTC_OCR1A , NORMAL , CLK_1024}; /* Timer1 Configurations */
	callBackFunctionTimer1(timer1_count); /* Timer 1 Call Back Function */

	/*Hardware INIT */
	LCD_init();
	UART_init();
	_delay_ms(1000);

	/*receive data from EEPROM */

	recieveEepromFlag(&eepromFlag); /* Receive Flag from EEPROM if raised the password is already created */

	eepromPassword = recieveEepromPassword(recievedData , &recievedPassword); /* Receive password from EEPROM to compare with the users entry */



	while(1)
	{
		/************************************************************************************
		 * 					     CREATE OR CHANGE PASSWORD CODE
		 ************************************************************************************/

		if(eepromFlag == FALSE || changePasswordFlag ==TRUE)
		{
			while(CreatingPasswordComplete != TRUE) /*leave loop when user created password */
			{
				CreatingPasswordComplete = FALSE;
				changePasswordFlag = FALSE;

				password = enterPassword(); /* user entered password */
				repassword = RenterPassword(); /* user Reentered password */

				if(password == repassword)
				{
					UART_sendByte(SEND_PASS_TO_CONTROLLER); /* Ordering (Control MC) to receive the user password */

					SendPassword(password); /* sending password function */

					CreatingPasswordComplete = TRUE; /* leave loop */

					UpdateEeprom = TRUE; /* update EEPROM with the new password */
				}

				else
				{
					Mismatch(); /* if user reentered password did not match print Mismatch on LCD */
				}

			}
		}
		if(UpdateEeprom == TRUE) /* if creating password is successful , Update EEPROM */
		{
			UART_sendByte(UPDATE_EEPROM);
			WDT_ON(); /* reset (HMI MC) to receive UPDATED EEPROM PASSWORD */
		}

		if(eepromFlag == TRUE) /* Go to door options directly if the EEPROM flag was raised */
		{
			doorOption = doorOptions();

			/************************************************************************************
			 * 					        MOTOR CONTROL CODE
			 ************************************************************************************/
			if(doorOption == '*')
			{
				password = enterPassword(); /* Entering  Password to compare with the EEPROM PASSWORD */

				if(password == eepromPassword)
				{
					MismatchCounter = 0; /*reset mismatch counter if user entered the right password */

					UART_sendByte(OPEN_DOOR); /* sending open door order to (control MC) to initiate motor rotation ACW */
					LCD_clearScreen();
					timer1_init(&timer1_config , 0 , 7209); /* initiate timer1 to start clock*/

					while(f_seconds != 1) /* print message for 15 seconds */
					{

						LCD_displayStringRowColumn(0,0,"DOOR IS ");
						LCD_displayStringRowColumn(1,0,"UNLOCKING");
					}

					f_seconds = 0;

					timer1StopClock(); /* stop timer1 clock */

					closeDoorFlag = TRUE;
				/*********************************************************************/

					/* Closing door Code */


					if(closeDoorFlag == TRUE)
					{
						closeDoorFlag = FALSE;
						closeDoor();
						UART_sendByte(CLOSE_DOOR);
						LCD_clearScreen();
						timer1_init(&timer1_config , 0 , 7209);
						while(f_seconds != 1)
						{

							LCD_displayStringRowColumn(0,0,"DOOR IS ");
							LCD_displayStringRowColumn(1,0,"LOCKING");
						}

						f_seconds = 0;
						timer1StopClock();
					}
				/**********************************************************************/
				}
				else
				{
					MismatchCounter++; /* if user entered incorrect password 3 times initiate BUZZER for 1 minute */
					if(MismatchCounter == 4)
					{
						LCD_clearScreen();
						MismatchCounter = 0;
						UART_sendByte(FIRE_BUZZER); /* sending orders to (Control MC) to initiate buzzer */
						timer1_init(&timer1_config , 0 , 7209);
						while(f_minutes != 1)
						{
							LCD_displayStringRowColumn(0,0,"THIEF!!!!!!!!");
						}
						f_minutes = 0;
						timer1StopClock();

					}
					else
					{
						Mismatch();
					}
				}
			}

			/************************************************************************************
			 * 					       CHANGING PASSWORD CODE
			 ************************************************************************************/
			else
			{
				while(changePasswordFlag != TRUE)
				{
					password = changePassword(); /* user entering old password to be able to have new one */
					if(password == eepromPassword)
					{
						MismatchCounter = 0; /* reset mismatch of user entered correct password */
						changePasswordFlag = TRUE; /* leave loop and go to create new password loop */
					}
					else
					{
						 /* if user entered incorrect password 3 times initiate BUZZER for 1 minute */

						MismatchCounter++;
						if(MismatchCounter == 4)
						{
							LCD_clearScreen();
							MismatchCounter = 0;
							UART_sendByte(FIRE_BUZZER);
							timer1_init(&timer1_config , 0 , 7209);
							while(f_minutes != 1)
							{
								LCD_displayStringRowColumn(0,0,"THIEF!!!!!!!!");
							}
							f_minutes = 0;
							timer1StopClock();

						}
						else
						{
							Mismatch();
						}
					}
				}
			}
		}



	}
}
