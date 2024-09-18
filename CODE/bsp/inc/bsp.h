/*
 * bsp.h
 *
 *  Created on: 24 mai 2021
 *      Author: Laurent
 */

#ifndef INC_BSP_H_
#define INC_BSP_H_

#include "stm32f0xx.h"

/* LED driver functions */
void BSP_LED_Init(void);
void BSP_LED_On(void);
void BSP_LED_Off(void);
void BSP_LED_Toggle(void);

/* DEBUG pins */
void BSP_DBG_Pin_Init(void);

/*Push-Button driver functions */
void BSP_PB_Init(void);
uint8_t	BSP_PB_GetState(void);

/* Debug Console init */
void BSP_Console_Init(void);

/*Timer delays*/
void BSP_TIMER_Timebase_Init(uint16_t period_ms);
void BSP_MOTORS_DELAY_TIM_init(void);
void BSP_MOTORS_DELAY_TIM_us(uint16_t us);
void BSP_PB_DELAY_TIM_init(void);
void BSP_PB_DELAY_TIM_us(uint16_t ms);
void BSP_TIMER_PWM_Init(void);
void BSP_DELAY_TIM_ms(uint16_t ms);


/*
 * NVIC initialization
 */
void BSP_NVIC_Init(void);

#endif /* INC_BSP_H_ */
