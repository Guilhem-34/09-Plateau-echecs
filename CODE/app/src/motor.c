/*
 * motor.c
 *
 *  Created on: 3 févr. 2023
 *      Author: Laurent
 */

#include "motor.h"
#include "bsp.h"
#include "main.h"

uint16_t 	STEP_DELAY = 1200;
uint8_t		STEPS_PER_REVOLUTION = 200;
uint16_t 	CLEAR_STEPS_NUMBER = 5000;
extern uint8_t PB_BOTTOM;		//line 0
extern uint8_t PB_LEFT;			//line 1



/*
 * void MOTORS_Init()
 * Initialize Motors pins such as Step and Direction Pins for left and right motors
 *
 * LEFT MOTOR :
 * 		Step Pin	(PC2)
 * 		Dir Pin		(PC3)
 *
 * RIGHT MOTOR :
 * 		Step Pin	(PC4)
 * 		Dir Pin		(PC5)
 *
 * All pins are in GPIO Output Mode
 *
 */

void MOTORS_Init(){

	// Enable GPIOC clock
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

	// Configure PC2, PC3, PC4 and PC5 as output
	GPIOC->MODER &= ~GPIO_MODER_MODER2_Msk;
	GPIOC->MODER |= (0x01 << GPIO_MODER_MODER2_Pos);

	GPIOC->MODER &= ~GPIO_MODER_MODER3_Msk;
	GPIOC->MODER |= (0x01 << GPIO_MODER_MODER3_Pos);

	GPIOC->MODER &= ~GPIO_MODER_MODER4_Msk;
	GPIOC->MODER |= (0x01 << GPIO_MODER_MODER4_Pos);

	GPIOC->MODER &= ~GPIO_MODER_MODER5_Msk;
	GPIOC->MODER |= (0x01 << GPIO_MODER_MODER5_Pos);

	// Configure PC2, PC3, PC4 and PC5 as Push-Pull output
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT_2;
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT_3;
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT_4;
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT_5;

	// Configure PC2, PC3, PC4 and PC5 as High-Speed Output
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEEDR2_Msk;
	GPIOC->OSPEEDR |= (0x03 << GPIO_OSPEEDR_OSPEEDR2_Pos);

	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEEDR3_Msk;
	GPIOC->OSPEEDR |= (0x03 << GPIO_OSPEEDR_OSPEEDR3_Pos);

	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEEDR4_Msk;
	GPIOC->OSPEEDR |= (0x03 << GPIO_OSPEEDR_OSPEEDR4_Pos);

	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEEDR5_Msk;
	GPIOC->OSPEEDR |= (0x03 << GPIO_OSPEEDR_OSPEEDR5_Pos);

	// Disable PC2, PC3, PC4 and PC5 Pull-up/Pull-down
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR2_Msk;
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR3_Msk;
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR4_Msk;
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR5_Msk;

	// Set Initial State OFF
	GPIOC->BSRR = 0x00000000;
}




/*
 * void MOTOR_LEFT_ENABLE(){
 *
 * LEFT motor enable with a defined direction using STEP_DELAY
 *
 */

void MOTOR_LEFT_ENABLE(uint16_t STEPS_NUMBER, int8_t DIR){


	// Set direction to clockwise if DIR=1 and counterclockwise if DIR=(-1)
	if(DIR == 1){
		GPIOC->BSRR |= GPIO_BSRR_BS_3;
	}
	else{
		GPIOC->BSRR |= GPIO_BSRR_BR_3;
	}

	// Spin the right motor till the number of steps is reached
	for(int i = 0; i < STEPS_NUMBER; i++){
		GPIOC->BSRR |= GPIO_BSRR_BS_2;
		BSP_MOTORS_DELAY_TIM_us(STEP_DELAY);
		GPIOC->BSRR |= GPIO_BSRR_BR_2;
		BSP_MOTORS_DELAY_TIM_us(STEP_DELAY);
	}

}





/*
 * void MOTOR_RIGHT_ENABLE(){
 *
 * Right motor enable with a defined direction using STEP_DELAY
 *
 */

void MOTOR_RIGHT_ENABLE(uint16_t STEPS_NUMBER, int8_t DIR){


	// Set direction to clockwise if DIR=1 and counterclockwise id DIR=(-1)
	if(DIR == 1){
		GPIOC->BSRR |= GPIO_BSRR_BS_5;
	}
	else{
		GPIOC->BSRR |= GPIO_BSRR_BR_5;
	}


	// Spin the right motor till the number of steps is reached

	for(int i = 0; i < STEPS_NUMBER; i++){
		GPIOC->BSRR |= GPIO_BSRR_BS_4;
		BSP_MOTORS_DELAY_TIM_us(STEP_DELAY);
		GPIOC->BSRR |= GPIO_BSRR_BR_4;
		BSP_MOTORS_DELAY_TIM_us(STEP_DELAY);
	}

}





/*
 * void MOTORS_ENABLE(){
 *
 * Enable both motors with defined steps and directions using TIM2 = STEP_DELAY as step delay
 *
 */

void MOTORS_ENABLE(uint16_t LEFT_STEPS_NUMBER, int8_t LEFT_DIR, uint16_t RIGHT_STEPS_NUMBER, int8_t RIGHT_DIR){


	// Set left motor's direction to clockwise if LEFT_DIR=1 and counterclockwise if LEFT_DIR=(-1)
	if(LEFT_DIR == 1){
		GPIOC->BSRR |= GPIO_BSRR_BS_3;
	}
	else{
		GPIOC->BSRR |= GPIO_BSRR_BR_3;
	}


	// Set right motor's direction to clockwise if RIGHT_DIR=1 and counterclockwise if RIGHT_DIR=(-1)
	if(RIGHT_DIR == 1){
		GPIOC->BSRR |= GPIO_BSRR_BS_5;
	}
	else{
		GPIOC->BSRR |= GPIO_BSRR_BR_5;
	}


	uint8_t 	complete = 0;
	uint8_t 	motor_state = 0;



	while(!complete){





		switch(motor_state){



			case 0 :											// Make a step with both motors


				if(RIGHT_STEPS_NUMBER == 0){					// All right motor steps completed before left motor
					motor_state = 1;
					break;
				}
				else if(LEFT_STEPS_NUMBER == 0){					// All left motor steps completed before right motor
					motor_state = 2;
					break;
				}


				GPIOC->BSRR |= GPIO_BSRR_BS_2;					// Write a HIGH into left motor's step pin
				GPIOC->BSRR |= GPIO_BSRR_BS_4;					// Write a HIGH into right motor's step pin
				BSP_MOTORS_DELAY_TIM_us(STEP_DELAY);
				GPIOC->BSRR |= GPIO_BSRR_BR_2;					// Write a LOW into left motor's step pin
				GPIOC->BSRR |= GPIO_BSRR_BR_4;					// Write a LOW into right motor's step pin
				BSP_MOTORS_DELAY_TIM_us(STEP_DELAY);


				LEFT_STEPS_NUMBER--;
				RIGHT_STEPS_NUMBER--;

				break;



			case 1 :											// Make a step with left motor only

				if(LEFT_STEPS_NUMBER == 0 ){					// All right motor steps completed before left motor
					complete = 1;
					break;
				}

				MOTOR_LEFT_ENABLE(LEFT_STEPS_NUMBER,LEFT_DIR);
				complete = 1;
				break;



			case 2 :											// Make a step with right motor only

				if(RIGHT_STEPS_NUMBER == 0 ){					// All right motor steps completed before left motor
					motor_state = 1;
					break;
				}
				MOTOR_RIGHT_ENABLE(RIGHT_STEPS_NUMBER,RIGHT_DIR);
				complete = 1;
				break;

		}
	}
}




/*
 * void MOTOR_CLEAR_POSITION(){
 *
 * Clear the motors position by leading the system at the bottom-left of the board.
 *
 * Note that for a movement like this one, the right motor always turn counterclockwise.
 *
 * Wherever our system is, we'll say that it needs  at least 5000 steps to return to its initial position.
 *
 */

void MOTOR_CLEAR_POSITION(){


	uint8_t state = 0;


	while((PB_BOTTOM && PB_LEFT) != 1){



		switch(state){


			case 0:					// Go to the bottom left corner by a diagonal movement


				// Make the direction to counterclockwise
				GPIOC->BSRR |= GPIO_BSRR_BR_5;


				// make the step
				GPIOC->BSRR |= GPIO_BSRR_BS_4;
				BSP_MOTORS_DELAY_TIM_us(STEP_DELAY);
				GPIOC->BSRR |= GPIO_BSRR_BR_4;
				BSP_MOTORS_DELAY_TIM_us(STEP_DELAY);

				if(PB_BOTTOM == 1){			// If it reaches the bottom then it need to go left
					state = 1;
					break;
				}
				else if(PB_LEFT == 1){		// If it reaches the left then it need to go down
					state = 2;
					break;
				}

				break;


			case 1:					// bottom reached, go to the bottom left corner by a left movement


				// Make the direction to counterclockwise for both motors
				GPIOC->BSRR |= GPIO_BSRR_BR_5;		// Right Motor
				GPIOC->BSRR |= GPIO_BSRR_BR_3;		// Left Motor


				// Make the step

				GPIOC->BSRR |= GPIO_BSRR_BS_2;					// Write a HIGH into left motor's step pin
				GPIOC->BSRR |= GPIO_BSRR_BS_4;					// Write a HIGH into right motor's step pin
				BSP_MOTORS_DELAY_TIM_us(STEP_DELAY);
				GPIOC->BSRR |= GPIO_BSRR_BR_2;					// Write a LOW into left motor's step pin
				GPIOC->BSRR |= GPIO_BSRR_BR_4;					// Write a LOW into right motor's step pin
				BSP_MOTORS_DELAY_TIM_us(STEP_DELAY);

				break;


			case 2:					// left reached, go to the bottom left corner by a down movement


				// Make the direction to counterclockwise for right motor and clockwise for left motor
				GPIOC->BSRR |= GPIO_BSRR_BR_5;		// Right Motor
				GPIOC->BSRR |= GPIO_BSRR_BS_3;		// Left Motor


				// Make the step

				GPIOC->BSRR |= GPIO_BSRR_BS_2;					// Write a HIGH into left motor's step pin
				GPIOC->BSRR |= GPIO_BSRR_BS_4;					// Write a HIGH into right motor's step pin
				BSP_MOTORS_DELAY_TIM_us(STEP_DELAY);
				GPIOC->BSRR |= GPIO_BSRR_BR_2;					// Write a LOW into left motor's step pin
				GPIOC->BSRR |= GPIO_BSRR_BR_4;					// Write a LOW into right motor's step pin
				BSP_MOTORS_DELAY_TIM_us(STEP_DELAY);

				break;

		}

	}

	my_printf("Position cleared.\r\n");
}
