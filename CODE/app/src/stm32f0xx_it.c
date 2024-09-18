/**
 ******************************************************************************
 * @file    Templates/Src/stm32f0xx_it.c 
 * @author  MCD Application Team
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and 
 *          peripherals interrupt service routine.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx.h"
#include "stm32f0xx_it.h"
#include "bsp.h"

/** @addtogroup STM32F0xx_HAL_Examples
 * @{
 */

/** @addtogroup Templates
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void) {
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void) {
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1) {
	}
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void) {
}

/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void) {
}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */

extern uint8_t system_delay;

void SysTick_Handler(void) {
	// Decrement system_delay every time a SysTick interrupt is triggered
	if (system_delay > 0)
		system_delay--;
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                               */
/******************************************************************************/

/*
 * This function handles TIM6 interrupts
 */

extern uint8_t timebase_irq;

void TIM6_DAC_IRQHandler() {
	// Test for TIM6 update pending interrupt
	if ((TIM6->SR & TIM_SR_UIF) == TIM_SR_UIF) {
		// Clear pending interrupt flag
		TIM6->SR &= ~TIM_SR_UIF;

		// Do what you need
		timebase_irq = 1;
	}
}


/**
  * This function handles EXTI lines 0 and 1 interrupt requests.
  */

extern uint8_t PB_BOTTOM;		//line 0
extern uint8_t PB_LEFT;			//line 1
extern uint8_t state;
extern uint8_t complete;


void EXTI0_1_IRQHandler()
{
	// Test for line 0 pending interrupt
	if ((EXTI->PR & EXTI_PR_PR0_Msk) != 0)
	{
		// Clear pending bit 0 by writing a '1'
		EXTI->PR = EXTI_PR_PR0;

		// Do what you need
		PB_BOTTOM = 1;
	}


	// Test for line 1 pending interrupt
	if ((EXTI->PR & EXTI_PR_PR1_Msk) != 0)
	{
		// Clear pending bit 1 by writing a '1'
		EXTI->PR = EXTI_PR_PR1;

		// Do what you need
		PB_LEFT = 1;
	}

}


/**
  * This function handles EXTI lines 8 interrupt request.
  */

/*
extern uint8_t SENSOR_STATE;


void EXTI4_15_IRQHandler()
{
	// Test for line 8 pending interrupt
	if ( ((EXTI->PR & EXTI_PR_PR8_Msk) != 0) && (SENSOR_STATE == 0) )
	{
		// Clear pending bit 8 by writing a '1'
                // Do not use OR masking here
		EXTI->PR = EXTI_PR_PR8;

		SENSOR_STATE = 1;
	}

}

*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
