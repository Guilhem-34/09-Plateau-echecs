/*
 * main.c
 *
 *  Created on: 22 February 2023
 *      Author: Laurent
 */

#include "stm32f0xx.h"
#include "main.h"
#include "bsp.h"
#include "delay.h"
#include "motor.h"
#include "chess.h"


// Static functions
static void SystemClock_Config(void);

// Global variables
uint8_t timebase_irq;
uint8_t system_delay;
uint8_t c;
uint8_t PB_BOTTOM = 0; 			//Push Button on the bottom of the board
uint8_t PB_LEFT = 0; 			//Push Button on the left of the board
uint8_t SENSOR_STATE;



uint8_t ad_lut[6] = {0,1,4,6,7,5};


// Main program
int main() {


	/*
	 * Initializations
	 */




	// Configure System Clock (48MHz)
	SystemClock_Config();

	// Configure SysTick at 10ms (10Hz) interrupt period
	while(SysTick_Config(SystemCoreClock/100) != 0);

	// Initialize console
	BSP_Console_Init();
	my_printf("Console Ready!\r\n");
	my_printf("SYSCLK = %d\r\n", SystemCoreClock);

	// Initialize LED and the 4 Push-Buttons
	BSP_LED_Init();
	BSP_PB_Init();

	// Initialize Timers Delay
	BSP_MOTORS_DELAY_TIM_init();
	BSP_PB_DELAY_TIM_init();

	// Initialize Timer for PWM output
	BSP_TIMER_PWM_Init();

	// Configure sampling timer with 10ms period (100Hz)
	BSP_TIMER_Timebase_Init(10);

	// Initialize sensors
	SENSORS_Init();

	// Initialize NVIC
	BSP_NVIC_Init();

	// Initialize Motors
	MOTORS_Init();


	// Wait  here for user button
	while(BSP_PB_GetState() == 0);


	//TIM1->CCR1 = 23;

	//Initialize Motors Position
	//MOTOR_CLEAR_POSITION();

	int board[CHESS_SIZE][CHESS_SIZE];

	Chess_Init(board);

	Print_Board(board);

	/*
	my_printf("\n\n\n-------------DEMONSTRATION STARTED-------------\r\n\n\n");

	DEMO1();

	my_printf("\n\n\n-------------DEMONSTRATION FINISHED !-------------\r\n\n\n");
	*/

	/*
	my_printf("\n\n\n-------------DEMONSTRATION STARTED-------------\r\n\n\n");


	FIND_PATH_AND_MOVE(board, 1, 0, 3, 3);

	Print_Board(board);


	FIND_PATH_AND_MOVE(board, 5, 5, 0, 2);

	Print_Board(board);


	FIND_PATH_AND_MOVE(board, 1, 4, 1, 5);

	Print_Board(board);



	my_printf("\n\n\n-------------DEMONSTRATION FINISHED !-------------\r\n\n\n");

	*/



	/*
	 *  Main Loop
	 */



	uint8_t i;
	uint8_t	sensors_buffer[6];
	my_printf("\e[2J");
	my_printf("\e[10;1H");

	while(1)
	{


		// Do every 10ms
		if(timebase_irq == 1)
		{


			for(i=0; i< 6; i++){

				sensors_buffer[i] = SENSOR_GetState(ad_lut[i]);
			}

			my_printf("Sensor 1 : %d  |  Sensor 2 : %d  |  Sensor 3 : %d  |  Sensor 4 : %d  |  Sensor 5 : %d  |  Sensor 6 : %d\n\r", sensors_buffer[1], sensors_buffer[2], sensors_buffer[3], sensors_buffer[4], sensors_buffer[5], sensors_buffer[0]);
		}
	}

}

/*
 * 	Clock configuration for the Nucleo STM32F072RB board
 * 	HSE input Bypass Mode 			-> 8MHz
 * 	SYSCLK, AHB, APB1 				-> 48MHz
 *  PA8 as MCO with /16 prescaler 	-> 3MHz
 */

static void SystemClock_Config() {
	uint32_t HSE_Status;
	uint32_t PLL_Status;
	uint32_t SW_Status;
	uint32_t timeout = 0;

	timeout = 1000000;

	// Start HSE in Bypass Mode
	RCC->CR |= RCC_CR_HSEBYP;
	RCC->CR |= RCC_CR_HSEON;

	// Wait until HSE is ready
	do {
		HSE_Status = RCC->CR & RCC_CR_HSERDY_Msk;
		timeout--;
	} while ((HSE_Status == 0) && (timeout > 0));

	// Select HSE as PLL input source
	RCC->CFGR &= ~RCC_CFGR_PLLSRC_Msk;
	RCC->CFGR |= (0x02 << RCC_CFGR_PLLSRC_Pos);

	// Set PLL PREDIV to /1
	RCC->CFGR2 = 0x00000000;

	// Set PLL MUL to x6
	RCC->CFGR &= ~RCC_CFGR_PLLMUL_Msk;
	RCC->CFGR |= (0x04 << RCC_CFGR_PLLMUL_Pos);

	// Enable the main PLL
	RCC->CR |= RCC_CR_PLLON;

	// Wait until PLL is ready
	do {
		PLL_Status = RCC->CR & RCC_CR_PLLRDY_Msk;
		timeout--;
	} while ((PLL_Status == 0) && (timeout > 0));

	// Set AHB prescaler to /1
	RCC->CFGR &= ~RCC_CFGR_HPRE_Msk;
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

	//Set APB1 prescaler to /1
	RCC->CFGR &= ~RCC_CFGR_PPRE_Msk;
	RCC->CFGR |= RCC_CFGR_PPRE_DIV1;

	// Enable FLASH Prefetch Buffer and set Flash Latency
	FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;

	// Select the main PLL as system clock source
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;

	// Wait until PLL becomes main switch input
	do {
		SW_Status = (RCC->CFGR & RCC_CFGR_SWS_Msk);
		timeout--;
	} while ((SW_Status != RCC_CFGR_SWS_PLL) && (timeout > 0));

	/*--- Use PA8 as LSE output ---*/

	// Set MCO source as LSE
	RCC->CFGR &= ~RCC_CFGR_MCO_Msk;
	RCC->CFGR |= RCC_CFGR_MCOSEL_LSE;     // Change here

	// No prescaler
	RCC->CFGR &= ~RCC_CFGR_MCOPRE_Msk;

	// Enable GPIOA clock
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// Configure PA8 as Alternate function
	GPIOA->MODER &= ~GPIO_MODER_MODER8_Msk;
	GPIOA->MODER |= (0x02 << GPIO_MODER_MODER8_Pos);

	// Set to AF0 (MCO output)
	GPIOA->AFR[1] &= ~(0x0000000F);
	GPIOA->AFR[1] |= (0x00000000);

	// Update SystemCoreClock global variable
	SystemCoreClockUpdate();
}


void DEMO1(void){

	MOVE_STRAIGHT("UP", 5);
	MOVE_STRAIGHT("RIGHT", 1);
	MOVE_STRAIGHT("DOWN", 5);
	MOVE_STRAIGHT("RIGHT", 1);
	MOVE_STRAIGHT("UP", 5);
	MOVE_STRAIGHT("RIGHT", 1);
	MOVE_STRAIGHT("DOWN", 5);
	MOVE_STRAIGHT("RIGHT", 1);
	MOVE_STRAIGHT("UP", 5);
	MOVE_STRAIGHT("RIGHT", 1);
	MOVE_STRAIGHT("DOWN", 5);
}


void DEMO2(void){

	MOVE_STRAIGHT("RIGHT", 5);
	MOVE_STRAIGHT("UP", 1);
	MOVE_STRAIGHT("LEFT", 5);
	MOVE_STRAIGHT("UP", 1);
	MOVE_STRAIGHT("RIGHT", 5);
	MOVE_STRAIGHT("UP", 1);
	MOVE_STRAIGHT("LEFT", 5);
	MOVE_STRAIGHT("UP", 1);
	MOVE_STRAIGHT("RIGHT", 5);
	MOVE_STRAIGHT("UP", 1);
	MOVE_STRAIGHT("LEFT", 5);
}


void DEMO3(void){

	MOVE_STRAIGHT("RIGHT", 5);
	MOVE_STRAIGHT("LEFT", 5);
	/*
	MOVE_STRAIGHT("RIGHT", 5);
	MOVE_STRAIGHT("LEFT", 5);
	MOVE_STRAIGHT("RIGHT", 5);
	MOVE_STRAIGHT("LEFT", 5);
	*/
}
