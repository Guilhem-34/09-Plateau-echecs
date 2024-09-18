/*
 * motor.h
 *
 *  Created on: 3 févr. 2023
 *      Author: Laurent
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "stm32f0xx.h"

#define FALSE	0
#define TRUE	1

/* MOTOR driver functions */
void MOTORS_Init(void);
void MOTOR_RIGHT_ENABLE(uint16_t STEPS_NUMBER, int8_t DIR);
void MOTOR_LEFT_ENABLE(uint16_t STEPS_NUMBER, int8_t DIR);
void MOTORS_ENABLE(uint16_t LEFT_STEPS_NUMBER, int8_t LEFT_DIR, uint16_t RIGHT_STEPS_NUMBER, int8_t RIGHT_DIR);
void MOTOR_CLEAR_POSITION(void);

#endif /* INC_MOTOR_H_ */
