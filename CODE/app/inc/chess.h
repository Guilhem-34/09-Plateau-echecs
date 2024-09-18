/*
 * chess.h
 *
 *  Created on: Apr 18, 2024
 *      Author: Bat14
 */

#ifndef INC_CHESS_H_
#define INC_CHESS_H_


#include <stdint.h>

#define CHESS_SIZE 6




void Chess_Init(int board[CHESS_SIZE][CHESS_SIZE]);
void SENSORS_Init(void);
uint8_t SENSOR_GetState(uint8_t sensor_adress);
void Print_Board(int board[CHESS_SIZE][CHESS_SIZE]);
void MOVE_STRAIGHT(char * DIRECTION, uint8_t SQUARE_NUMBER);
void FIND_PATH_AND_MOVE(int board[CHESS_SIZE][CHESS_SIZE], uint8_t SQUARE1_X, uint8_t SQUARE1_Y, uint8_t SQUARE2_X, uint8_t SQUARE2_Y);



#endif /* INC_CHESS_H_ */
