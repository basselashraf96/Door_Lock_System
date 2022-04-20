/*
 * door_lock.h
 *
 *  Created on: Jan 29, 2020
 *      Author: Basel Ashraf
 */

#ifndef DOOR_LOCK_CONTROL_H_
#define DOOR_LOCK_CONTROL_H_

#include "timer1.h"
#include "uart.h"
#include "i2c.h"
#include "external_eeprom.h"
#include "wd_timer.h"
/*********************************************************************************
 * 								    MACROS
 *********************************************************************************/
#define CTRL_READY 0x10
#define HMI_READY 0x11
#define STOP_SENDING 0x12
#define SEND_PASS_TO_CONTROLLER 0x13
#define OPEN_DOOR 0x14
#define CLOSE_DOOR 0x15
#define FIRE_BUZZER 0x16
#define UPDATE_EEPROM 0x17
#define EEPROM_FLAG 1
/*********************************************************************************
 * 								FUNCTION PROTOTYPES
 *********************************************************************************/
uint32 reverseNumber(uint32 number);
uint8 savePasswordToEEPROM(uint32 data , uint16 address , uint8 locations);
uint32 getPasswordFromEEPROM(uint16 address , uint8 locations );
void SendPassword(uint32);
void recievePassword(uint8 recievedData , uint32 * recievedPassword);
void sendEepromFlag(uint16 address , uint8 *data);
void sendEepromPassword(uint16 address , uint8 locations , uint32 * data);





#endif /* DOOR_LOCK_CONTROL_H_ */
