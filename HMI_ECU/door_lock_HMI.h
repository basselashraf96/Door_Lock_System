/*
 * door_lock.h
 *
 *  Created on: Jan 29, 2020
 *      Author: Basel Ashraf
 */

#ifndef DOOR_LOCK_HMI_H_
#define DOOR_LOCK_HMI_H_

#include "timer1.h"
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
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
uint32 enterPassword(void);
uint32 RenterPassword(void);
uint8 doorOptions(void);
void closeDoor(void);
uint32 changePassword(void);
uint32 reverseNumber(uint32 number);
void SendPassword(uint32);
void recievePassword(uint8 recievedData , uint32 * recievedPassword);
void Mismatch(void);
void recieveEepromFlag(uint8 * Ptr);
uint32 recieveEepromPassword(uint8 recievedData , uint32 * recievedPassword);








#endif /* DOOR_LOCK_HMI_H_ */
