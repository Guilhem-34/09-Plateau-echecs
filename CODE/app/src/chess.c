/*
 * chess.c
 *
 *  Created on: Apr 18, 2024
 *      Author: Bat14
 */


#include <stdint.h>
#include <string.h>
#include "chess.h"
#include "main.h"
#include "motor.h"



//float 		pi = 3.1415926535;
uint8_t 	SQUARE_LENGTH = 5; 			// Each square on our board measure 5x5 cm
uint8_t 	PULLEY_DIAMETER = 2;		// Each pulley on our board measure 2cm of diameter

// Conversion into steps number
uint32_t	STEPS_PER_SQUARE = 159;		// STEPS_PER_SQUARE = (200*SQUARE_LENGTH)/(pi*PULLEY_DIAMETER);
										// STEPS_PER_SQUARE = 159.15 ~ 159

/*
 *
 * Let's begin with a few explanations before starting writing the functions.
 *
 * For our tests, we would like to start with a 5x5 chess board.
 *
 * Therefore, we have to initialize our pieces on the board with a special alias.
 *
 * In chess, we have 6 different pieces that will be associated with a letter to identify them in our code :
 *
 *
 * 		# King : 		K						1
 *
 * 		# Queen : 		Q						2
 *
 * 		# Rook : 		R						3
 *
 * 		# Bishop : 		B						4
 *
 * 		# Knight : 		N						5
 *
 * 		# Pawn :		P						6
 *
 *
 *
 * All of the letter will be followed by the player number (1 or 2)
 *
 *
 * At the end, we should see something like this in the console for our test:
 *
 *
 *
 * 					--------------------------
 * 				   |  R2  N2  K2  K2  Q2  B2  |
 * 				   |  P2  P2  P2  P2  P2  P2  |
 * 				   |					      |
 * 				   |					      |
 * 				   |					      |
 * 				   |					      |
 * 				   |					      |
 * 				   |					      |
 * 				   |					      |
 * 				   |  R1  N1  K1  K1  Q1  B1  |
 * 				    --------------------------
 *
 *
 */

void Chess_Init(int board[CHESS_SIZE][CHESS_SIZE]){

	int i; int j;

	for(i = 0; i < CHESS_SIZE; i++){
		for( j = 0; j < CHESS_SIZE; j++){

			switch(i){

				case 0:

					if(j==0) {
						board[i][j] = -3;
						break;
					}
					else if(j==1) {
						board[i][j] = -5;
						break;
					}
					else if(j==2) {
						board[i][j] = -1;
						break;
					}
					else if(j==3) {
						board[i][j] = -1;
						break;
					}
					else if(j==4) {
						board[i][j] = -2;
						break;
					}
					else {
						board[i][j] = -4;
						break;
					}

				case 1 :

					board[i][j] = -6;
					break;


				case CHESS_SIZE - 2 :


					board[i][j] = 6;
					break;

				case CHESS_SIZE - 1 :


					if(j==0) {
						board[i][j] = 3;
						break;
					}
					else if(j==1) {
						board[i][j] = 5;
						break;
					}
					else if(j==2) {
						board[i][j] = 1;
						break;
					}
					else if(j==3) {
						board[i][j] = 1;
						break;
					}
					else if(j==4) {
						board[i][j] = 2;
						break;
					}
					else {
						board[i][j] = 4;
						break;
					}

				default :
					board[i][j] = 0;
					break;
			}

		}
	}
}




/*
 * void SENSORS_Init()
 * Initialize the hall effect sensors pins
 *
 * 1st bit of adress		(PC6)
 * 2nd bit of adress		(PC7)
 * 3rd bit of adress		(PC8)
 *
 * Enable					(PC9)
 *
 * Input					(PB8)
 *
 */

void SENSORS_Init() {

	// Enable GPIOB and GPIOC clock
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

	// Configure PC7, PC6, PC8 and PC9 as output
	GPIOC->MODER &= ~( GPIO_MODER_MODER6_Msk | GPIO_MODER_MODER7_Msk | GPIO_MODER_MODER8_Msk | GPIO_MODER_MODER9_Msk);
	GPIOC->MODER |= (0x01 << GPIO_MODER_MODER6_Pos) | (0x01 << GPIO_MODER_MODER7_Pos) | (0x01 << GPIO_MODER_MODER8_Pos) | (0x01 << GPIO_MODER_MODER9_Pos);

	// Configure PB8 as input
	GPIOB->MODER &= ~GPIO_MODER_MODER8_Msk;
	GPIOB->MODER |= (0x00 << GPIO_MODER_MODER8_Pos);

	// Configure PC7, PC6, PC5 and PC9 as Push-Pull output
	GPIOC->OTYPER &= ~( GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7 | GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);

	// Configure PC7, PC6, PC8 and PC9 as High-Speed Output
	GPIOC->OSPEEDR &= ~( GPIO_OSPEEDR_OSPEEDR6_Msk | GPIO_OSPEEDR_OSPEEDR7_Msk | GPIO_OSPEEDR_OSPEEDR8_Msk | GPIO_OSPEEDR_OSPEEDR9_Msk );
	GPIOC->OSPEEDR |=  (0x03 << GPIO_OSPEEDR_OSPEEDR6_Pos) | (0x03 << GPIO_OSPEEDR_OSPEEDR7_Pos) | (0x03 << GPIO_OSPEEDR_OSPEEDR8_Pos) | (0x03 << GPIO_OSPEEDR_OSPEEDR9_Pos);


	// Disable PC7, PC6, PC8, PC9 and PB8 Pull-down
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR6_Msk | GPIO_PUPDR_PUPDR7_Msk | GPIO_PUPDR_PUPDR8_Msk | GPIO_PUPDR_PUPDR9_Msk);
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR8_Msk;

	// Set Enable pin
	GPIOC->BSRR |= GPIO_BSRR_BS_9;

}



/*
 * SENSOR_GetState()
 * Returns the state of the sensor (0= no detection, 1=detected)
 */





uint8_t SENSOR_GetState(uint8_t sensor_adress)
{

	uint8_t state;

	// Set adresses bits


	GPIOC->BSRR &= ~(GPIO_BSRR_BR_6 | GPIO_BSRR_BR_7 | GPIO_BSRR_BR_8 | GPIO_BSRR_BS_6 | GPIO_BSRR_BS_7 | GPIO_BSRR_BS_8) ;
	GPIOC->BSRR |= ( ((~sensor_adress & 0x00000007) << 22) | (sensor_adress << 6) );



	/*
	GPIOC->ODR &= ~(0x1c0);
	GPIOC->ODR |= (sensor_adress << 6) ;
	*/

	if ((GPIOB->IDR & GPIO_IDR_8) == GPIO_IDR_8)
	{
		state = 0;
	}
	else
	{
		state = 1;
	}

	return state;
}



void Print_Board(int board[CHESS_SIZE][CHESS_SIZE]){


	//affichage tableau


	my_printf(" --------------------------\r\n");

	for(int l = 0; l < CHESS_SIZE; l++){

		my_printf("|  ");
		for(int k = 0; k < CHESS_SIZE; k++){


			if(board[l][k] == 1) my_printf("K1  ");

			else if(board[l][k] == 2) my_printf("Q1  ");

			else if(board[l][k] == 3) my_printf("R1  ");

			else if(board[l][k] == 4) my_printf("B1  ");

			else if(board[l][k] == 5) my_printf("N1  ");

			else if(board[l][k] == 6) my_printf("P1  ");

			else if(board[l][k] == -1) my_printf("K2  ");

			else if(board[l][k] == -2) my_printf("Q2  ");

			else if(board[l][k] == -3) my_printf("R2  ");

			else if(board[l][k] == -4) my_printf("B2  ");

			else if(board[l][k] == -5) my_printf("N2  ");

			else if(board[l][k] == -6) my_printf("P2  ");

			else my_printf("    ");
		}
		my_printf("|\r\n");
		if(l != (CHESS_SIZE-1))my_printf("\r\n");

	}

	my_printf(" --------------------------\r\n\n\n");



}





// void MOVE_STRAIGHT(char DIRECTION, uint8_t SQUARE_NUMBER, uint8_t CASE_DIAMETER, uint8_t PULLEY_DIAMETER)
/*
 * 	Move the system from its actual position to a defined square number according to a direction : UP, DOWN, LEFT or RIGHT
 *
 *	To do that, we convert the distance into a number of steps for both motors
 *
 */

void MOVE_STRAIGHT(char * DIRECTION, uint8_t SQUARE_NUMBER){


	my_printf("The piece moves %s across %d squares.\r\n",DIRECTION, SQUARE_NUMBER);

	while (SQUARE_NUMBER != 0){


		if(strcmp(DIRECTION,"UP") == 0){


			// Direction : up -> left motor turns counterclockwise and right motor turns clockwise

			MOTORS_ENABLE(STEPS_PER_SQUARE, -1, STEPS_PER_SQUARE, 1);


		}else if(strcmp(DIRECTION,"DOWN") == 0){

			// Direction : down -> left motor turns clockwise and right motor turns counterclockwise

			MOTORS_ENABLE(STEPS_PER_SQUARE, 1, STEPS_PER_SQUARE, -1);

		}else if(strcmp(DIRECTION,"LEFT") == 0){

			// Direction : left -> left motor turns counterclockwise and right motor turns counterclockwise

			MOTORS_ENABLE(STEPS_PER_SQUARE, -1, STEPS_PER_SQUARE, -1);

		}else if(strcmp(DIRECTION,"RIGHT") == 0){

			// Direction : right -> left motor turns clockwise and right motor turns clockwise

			MOTORS_ENABLE(STEPS_PER_SQUARE, 1, STEPS_PER_SQUARE, 1);

		}else{
			my_printf("Wait, there is a direction error...\r\n");
			break;
		}


		SQUARE_NUMBER--;
	}

	if(SQUARE_NUMBER != 0){
		my_printf("Movement not done.\r\n");
	}
	else{
		my_printf("Movement done.\r\n");
	}

	my_printf("\n");

}






void FIND_PATH_AND_MOVE(int board[CHESS_SIZE][CHESS_SIZE], uint8_t SQUARE1_X, uint8_t SQUARE1_Y, uint8_t SQUARE2_X, uint8_t SQUARE2_Y){

	if(board[CHESS_SIZE - SQUARE2_Y - 1][SQUARE2_X] != 0){
		my_printf("A piece is already on the final square !\r\n\n\n");
		return;
	}


	int8_t dx = SQUARE2_X - SQUARE1_X;
	int8_t dy = SQUARE2_Y - SQUARE1_Y;


	//Make the move according to the x-axis
	if ( dx > 0 ){

		MOVE_STRAIGHT("RIGHT",dx);
	}
	else if( dx < 0){

		MOVE_STRAIGHT("LEFT",-dx);
	}

	//Make the move according to the y-axis
	if ( dy > 0 ){

		MOVE_STRAIGHT("UP",dy);
	}
	else if( dy < 0){

		MOVE_STRAIGHT("DOWN",-dy);
	}


	//Update the array
	board[CHESS_SIZE - SQUARE2_Y - 1][SQUARE2_X] = board[CHESS_SIZE - SQUARE1_Y - 1][SQUARE1_X];
	board[CHESS_SIZE - SQUARE1_Y - 1][SQUARE1_X] = 0;


}





