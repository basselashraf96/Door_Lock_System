/*
 * timer1.c
 *
 *  Created on: Jan 27, 2020
 *      Author: Basel Ashraf
 */

#include "timer1.h"

/*********************************************************************************************************
 * 										CALL BACK FUNCTION CONFIG
 ********************************************************************************************************/
void (*g_callBackTimer1)(void) = NULL_PTR;

#ifdef NORMAL__MODE
ISR(TIMER1_OVF_vect)
{
	if(g_callBackTimer1 != NULL_PTR)
	{
		(*g_callBackTimer1)();
	}
}
#endif

#ifdef COMP1A
ISR(TIMER1_COMPA_vect)
{
	if(g_callBackTimer1 != NULL_PTR)
	{
		(*g_callBackTimer1)();
	}
}
#endif

#ifdef COMP1B
ISR(TIMER1_COMPB_vect)
{
	if(g_callBackTimer1 != NULL_PTR)
	{
		(*g_callBackTimer1)();
	}
}
#endif

/******************************************************************************************************
 * 											FUNCTION DIFINITION
 *****************************************************************************************************/

void timer1_init(timer1Config * timerConfig , uint16 initialValue , uint16 compareValue )
{
	TCCR1A = (TCCR1A & 0x0f) | ((timerConfig ->compareMode) << 4 ); /* masking last 4 bits of TIMER1A control register */

#ifndef PWM
	/* non_pwm mode */
	SET_BIT(TCCR1A , FOC1A);
	SET_BIT(TCCR1A , FOC1A);
#endif

	/* masking first 2 bits of TIMER1A control register with the first 2 bits of timer1 waveform mode */
	TCCR1A = (TCCR1A & 0xFC) | ((timerConfig -> mode) & 0x03);

	/* masking  bit 3 and 4 of TIMER1B control register with the other 2 bits of timer1 waveform mode */
	TCCR1B = (TCCR1B & 0xE7) | (((timerConfig -> mode) & 0x0C) << 1);

	/* masking first 3 bit in TIMER1B control register for clock prescaler selection */
	TCCR1B = (TCCR1B & 0Xf8) | ((timerConfig -> clock) & 0x07);

	TCNT1 = initialValue;
	OCR1A = compareValue;
	OCR1B = compareValue;

#ifdef NORMAL__MODE
	SET_BIT(TIMSK , TOIE1);  /* timer1 overflow interrupt enable */
#endif

#ifdef COMP1A
	SET_BIT(TIMSK , OCIE1A); /* compare match A interrupt enable */

#endif

#ifdef COMP1B
	SET_BIT(TIMSK , OCIE1B); /* compare match B interrupt enable */
#endif

}

void timer1StopClock(void)
{
	TCCR1B &= ~(1 << CS10) &~ (1 << CS11) &~ (1 <<CS12); /* stop clock */
}

void callBackFunctionTimer1(void(*CallBackPtr)(void))
{
 g_callBackTimer1 = CallBackPtr;
}



