/*
 * timer1.h
 *
 *  Created on: Jan 27, 2020
 *      Author: Basel Ashraf
 */

#ifndef TIMER1_H_
#define TIMER1_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"
/********************************************************************************
 * 							Preprocessor Configurations
 ********************************************************************************/

#define NORMAL__MODE
#define PWM
#define CTC
#define COMP1A
#define COMP1B


#undef NORMAL__MODE
#undef PWM
/*#undef CTC*/
/*#undef COMP1A*/
#undef COMP1B

/********************************************************************************
 * 								Timer1 Configurations
 ********************************************************************************/
typedef enum
{
	NORMAL , TOGGLE=5 , CLEAR=10 , SET=15
}compareMatchMode;

typedef enum
{
	NORMAL_MODE , PHASE_8 , PHASE_9 , PHASE_10 , CTC_OCR1A , FAST_8 , FAST_9 , FAST_10 ,
	FREQUENCY_ICR1 , FEQUECNCY_OCR1A , PHASE_ICR1 , PHASE_OCR1A , CTC_ICR1 , RESERVED ,
	FAST_ICR1 , FAST_OCR1A
}timer1Mode;

typedef enum
{
	NO_CLOCK , F_CPU_CLK , CLK_8 , CLK_64 , CLK2_256 , CLK_1024 , CLK_FALLING , CLK_RISING
}timer1CLK;

typedef struct
{
	timer1Mode mode;
	compareMatchMode compareMode;
	timer1CLK clock;
}timer1Config;


/********************************************************************************
 * 								FUNCTION PROTOTYPES
 ********************************************************************************/

void timer1_init(timer1Config * timerConfig , uint16 initialValue , uint16 compareValue );
void timer1StopClock(void);
void callBackFunctionTimer1(void(*CallBackPtr)(void));




#endif /* TIMER1_H_ */
