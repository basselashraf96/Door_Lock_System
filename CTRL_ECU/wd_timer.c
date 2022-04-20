/*
 * wd_timer.c
 *
 *  Created on: Feb 4, 2020
 *      Author: Basel Ashraf
 */
#include "wd_timer.h"

void WDT_ON(void)
{
	// Watchdog timer enables with timeout period 16.3ms .
	WDTCR = (1<<WDE);
}

// 	function to disable Watchdog timer.
void WDT_OFF(void)
{
	// set the WDTOE & WDE bits in the same operation
	WDTCR = (1<<WDTOE)|(1<<WDE);
	//wait 4 cycles before clear the WDE bit
	_delay_us(4);
	WDTCR = 0x00;
}
