/*
 * CONTROL_ECU.c
 *
 *  Created on: Jan 29, 2020
 *      Author: Basel Ashraf
 */


#include "door_lock_control.h"


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
	uint32 recievedPassword = 0;
	uint8 recievedData = 0;
	uint32 password = 0;
	uint32 getPassword = 0;
	bool data = 0;

	SREG = (1<<7); /* Enable I-Bit */
	timer1Config timer1_config = {CTC_OCR1A , NORMAL , CLK_1024}; /* Timer1 Configurations */
	callBackFunctionTimer1(timer1_count); /* Timer 1 Call Back Function */

	/*Hardware INIT */
	EEPROM_init();
	UART_init();

	/*
	 * MOTOR IN PINS AS OUTPUT (PD6 , PD7)
	 * BUZZER IN PINS AS OUTPUT (PD4)
	 */
	DDRD |= (1 << PD6) | (1 << PD7) | (1 << PD4);



	_delay_ms(100);

	/*sending data to (HMI MC) from EEPROM Registers */

	sendEepromFlag(0x031C , &data);
	sendEepromPassword(0x0310 , 4 , &getPassword);



	while(1)
	{
		recievedData = UART_recieveByte(); /* Storing Orders sent from (HMI MC) to initiate its task */

		if(recievedData == UPDATE_EEPROM) /* if users created or changed  password reset MC to update EEPROM */
		{
			WDT_ON(); /* watch dog timer on */
		}

		/************************************************************************************
		 * 						RECEIVING AND STORING PASSWORD CODE
		 ************************************************************************************/
		if(recievedData == SEND_PASS_TO_CONTROLLER) /* receive users password ORDER */
		{
			recievePassword(recievedData , &recievedPassword); /* receive password function */

			password = reverseNumber(recievedPassword); /* Rearrange password to its original form */

			savePasswordToEEPROM(password , 0x0310 , 4); /* store password in EEPROM */
			EEPROM_writeByte(0x031C , EEPROM_FLAG); /* raise EEPROM FLAG */
			_delay_ms(100);
			recievedData = 0; /* reset to receive another order */
		}

		/************************************************************************************
		 * 								 OPENING DOOR CODE
		 ************************************************************************************/
		if(recievedData == OPEN_DOOR) /* Receive opening door order */
		{
			timer1_init(&timer1_config , 0 , 7209);
			while(f_seconds != 1)
			{

				/* Rotate motor ACW */
				PORTD &= ~(1 << PD6);
				PORTD |= (1 << PD7);
			}

			/* Stop motor */
			PORTD &= ~(1 << PD6);
			PORTD &= ~(1 << PD7);

			f_seconds = 0; /* reset seconds flag */
			recievedData = 0;
			timer1StopClock(); /* stop timer1 clock */
		}

		/************************************************************************************
		 * 							     FIRING BUZZER CODE
		 ************************************************************************************/
		if(recievedData == FIRE_BUZZER) /* Receive Firing buzzer order */
		{
			timer1_init(&timer1_config , 0 , 7209);
			while(f_minutes != 1)
			{

				/*fire buzzer */
				PORTD |= (1 << PD4);
			}
			timer1StopClock();
			f_minutes = 0;
			PORTD &= ~(1 << PD4);
			recievedData = 0;

		}

		/************************************************************************************
		 * 								  CLOSING DOOR CODE
		 ************************************************************************************/
		if(recievedData == CLOSE_DOOR) /* Receive Closing door order */
		{
			timer1_init(&timer1_config , 0 , 7209);
			while(f_seconds != 1)
			{

				/* Rotate motor CW */
				PORTD &= ~(1 << PD7);
				PORTD |= (1 << PD6);
			}

			/* Stop motor */
			PORTD &= ~(1 << PD6);
			PORTD &= ~(1 << PD7);

			f_seconds = 0;
			recievedData = 0;
			timer1StopClock();
		}


	}


}
