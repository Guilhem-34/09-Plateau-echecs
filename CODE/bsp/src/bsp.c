/*
 * bsp.c
 *
 *  Created on: 24 mai 2021
 *      Author: Laurent
 */

#include "main.h"
#include "bsp.h"

/*
 * BSP_LED_Init()
 * Initialize LED pin (PA5) as a High-Speed Push-Pull output
 * Set LED initial state to OFF
 */

void BSP_LED_Init() {
	// Enable GPIOA clock
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// Configure PA5 as output
	GPIOA->MODER &= ~GPIO_MODER_MODER5_Msk;
	GPIOA->MODER |= (0x01 << GPIO_MODER_MODER5_Pos);

	// Configure PA5 as Push-Pull output
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_5;

	// Configure PA5 as High-Speed Output
	GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEEDR5_Msk;
	GPIOA->OSPEEDR |= (0x03 << GPIO_OSPEEDR_OSPEEDR5_Pos);

	// Disable PA5 Pull-up/Pull-down
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR5_Msk;

	// Set Initial State OFF
	GPIOA->BSRR |= GPIO_BSRR_BR_5;
}

/*
 * BSP_LED_On()
 * Turn ON LED on PA5
 */

void BSP_LED_On() {
	GPIOA->BSRR = GPIO_BSRR_BS_5;
}

/*
 * BSP_LED_Off()
 * Turn OFF LED on PA5
 */

void BSP_LED_Off() {
	GPIOA->BSRR = GPIO_BSRR_BR_5;
}

/*
 * BSP_LED_Toggle()
 * Toggle LED on PA5
 */

void BSP_LED_Toggle() {
	GPIOA->ODR ^= GPIO_ODR_5;
}

/*
 * BSP_DBG_Pin_Init()
 * Initialize Debug pin (PA6) as a High-Speed Push-Pull output
 * Set pin initial state to OFF
 */

void BSP_DBG_Pin_Init() {
	// Enable GPIOA clock
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// Configure PA6 as output
	GPIOA->MODER &= ~GPIO_MODER_MODER6_Msk;
	GPIOA->MODER |= (0x01 << GPIO_MODER_MODER6_Pos);

	// Configure PA6 as Push-Pull output
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_6;

	// Configure PA6 as High-Speed Output
	GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEEDR6_Msk;
	GPIOA->OSPEEDR |= (0x03 << GPIO_OSPEEDR_OSPEEDR6_Pos);

	// Disable PA6 Pull-up/Pull-down
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR6_Msk;

	// Set Initial State OFF
	GPIOA->BSRR |= GPIO_BSRR_BR_6;
}

/*
 * BSP_PB_Init()
 * Initialize Push-Button pins as input with Pull-up
 *
 * PB_BOTTOM		(PC0)
 * PB_LEFT			(PC1)
 * Embedded PB		(PC13)
 *
 */

void BSP_PB_Init() {
	// Enable GPIOC clock
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

	// Configure PC0 as input
	GPIOC->MODER &= ~GPIO_MODER_MODER0_Msk;
	GPIOC->MODER |= (0x00 << GPIO_MODER_MODER0_Pos);

	// Configure PC1 as input
	GPIOC->MODER &= ~GPIO_MODER_MODER1_Msk;
	GPIOC->MODER |= (0x00 << GPIO_MODER_MODER1_Pos);

	// Configure PC13 as input
	GPIOC->MODER &= ~GPIO_MODER_MODER13_Msk;
	GPIOC->MODER |= (0x00 <<GPIO_MODER_MODER13_Pos);

	// Enable PC0 and PC1 Pull-up
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR0_Msk;
	GPIOC->PUPDR |= (0x1UL << GPIO_PUPDR_PUPDR0_Pos);

	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR1_Msk;
	GPIOC->PUPDR |= (0x1UL << GPIO_PUPDR_PUPDR1_Pos);

	// Disable PC13 Pull-up/Pull-down
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13_Msk;

	// Enable SYSCFG clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// Select Port C as interrupt source for EXTI line 0 and 1
	SYSCFG->EXTICR[0] &= ~ SYSCFG_EXTICR1_EXTI0_Msk;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PC;

	SYSCFG->EXTICR[0] &= ~ SYSCFG_EXTICR1_EXTI1_Msk;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PC;

	// Enable EXTI line 0 and 1
	EXTI->IMR |= EXTI_IMR_IM0;
	EXTI->IMR |= EXTI_IMR_IM1;

	// Disable Rising / Enable Falling trigger
	EXTI->RTSR |= EXTI_RTSR_RT0;
	EXTI->FTSR &= ~EXTI_FTSR_FT0;

	EXTI->RTSR |= EXTI_RTSR_RT1;
	EXTI->FTSR &= ~EXTI_FTSR_FT1;


}


/*
 * BSP_PB_GetState()
 * Returns the state of the button (0=released, 1=pressed)
 */

uint8_t	BSP_PB_GetState()
{
	uint8_t state;

	if ((GPIOC->IDR & GPIO_IDR_13) == GPIO_IDR_13)
	{
		state = 0;
	}
	else
	{
		state = 1;
	}

	return state;
}



/*
 * BSP_Console_Init()
 * USART1 @ 115200 Full Duplex
 * 1 start - 8-bit - 1 stop
 * TX -> PA2 (AF1)
 * RX -> PA3 (AF1)
 */


void BSP_Console_Init() {
	// Enable GPIOA clock
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// Configure PA2 and PA3 as Alternate function
	GPIOA->MODER &= ~(GPIO_MODER_MODER2_Msk | GPIO_MODER_MODER3_Msk);
	GPIOA->MODER |= (0x02 << GPIO_MODER_MODER2_Pos) | (0x02 << GPIO_MODER_MODER3_Pos);

	// Set PA2 and PA3 to AF1 (USART2)
	GPIOA->AFR[0] &= ~(0x0000FF00);
	GPIOA->AFR[0] |= (0x0001100);

	// Enable USART2 clock
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	// Clear USART1 configuration (reset state)
	// 8-bit, 1 start, 1 stop, CTS/RTS disabled
	USART2->CR1 = 0x00000000;
	USART2->CR2 = 0x00000000;
	USART2->CR3 = 0x00000000;

	// Select PCLK (APB1) as clock source
	// PCLK -> 48 MHz
	RCC->CFGR3 &= ~RCC_CFGR3_USART2SW_Msk;

	// Baud Rate = 115200
	// With OVER8=0 and Fck=48MHz, USARTDIV =   48E6/115200 = 416.6666
	// BRR = 417 -> Actual BaudRate = 115107.9137 -> 0.08% error
	//
	// With OVER8=1 and Fck=48MHz, USARTDIV = 2*48E6/115200 = 833.3333
	// BRR = 833 -> Actual BaudRate = 115246.0984 -> 0.04% error (better choice)

	USART2->CR1 |= USART_CR1_OVER8;
	USART2->BRR = 833;

	// Enable both Transmitter and Receiver
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;

	// Enable USART2
	USART2->CR1 |= USART_CR1_UE;
}





void BSP_TIMER_Timebase_Init(uint16_t period_ms) {
	// Enable TIM6 clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

	// Reset TIM6 configuration
	TIM6->CR1 = 0x0000;
	TIM6->CR2 = 0x0000;

	// Set TIM6 prescaler
	// Fck = 48MHz -> /48000 = 1kHz counting frequency
	TIM6->PSC = (uint16_t) 48000 - 1;

	// Set TIM6 auto-reload register for SAMPLING_PERIOD (µs)
	TIM6->ARR = (uint16_t) period_ms - 1;

	// Enable auto-reload preload
	TIM6->CR1 |= TIM_CR1_ARPE;

	// Enable Interrupt upon Update Event
	TIM6->DIER |= TIM_DIER_UIE;

	// Start TIM6 counter
	TIM6->CR1 |= TIM_CR1_CEN;
}


/*
 * motors_timer_delay_init()
 * Initialize TIM2 with 1us counting period
 * ARR is set to maximum value -> delay [2us-65ms]
 */

void BSP_MOTORS_DELAY_TIM_init(void)
{
	// Enable TIM2 clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	// Reset TIM2 configuration
	TIM2->CR1 = 0x0000;
	TIM2->CR2 = 0x0000;

	// Set TIM2 prescaler
	// Fck = 48MHz -> /48 = 1MHz counting frequency
	TIM2->PSC = (uint16_t) 48 -1;

	// Set ARR to maximum value
	TIM2->ARR = (uint16_t) 0xFFFF;
}


/*
 * motors_timer_delay_us(uint16_t us)
 * waits here for us
 */
void BSP_MOTORS_DELAY_TIM_us(uint16_t us)
{
	// Resets TIM2 counter
	TIM2->EGR |= TIM_EGR_UG;

	// Start TIM2 counter
	TIM2->CR1 |= TIM_CR1_CEN;

	// Wait until TIM2 counter reaches delay
	while(TIM2->CNT < us);

	// Stop TIM2 counter
	TIM2->CR1 &= ~TIM_CR1_CEN;
}




/*
 * pushbutton_timer_delay_init()
 * Initialize TIM3 with 1ms counting period
 * ARR is set to maximum value -> delay [2ms-65s]
 */

void BSP_PB_DELAY_TIM_init(void)
{
	// Enable TIM3 clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	// Reset TIM3 configuration
	TIM3->CR1 = 0x0000;
	TIM3->CR2 = 0x0000;

	// Set TIM3 prescaler
	// Fck = 48MHz -> /48000 = 1kHz counting frequency
	TIM3->PSC = (uint16_t) 48000 -1;

	// Set ARR to maximum value
	TIM3->ARR = (uint16_t) 0xFFFF;
}



void BSP_DELAY_TIM_ms(uint16_t ms)
{
	// Resets TIM6 counter
	TIM6->EGR |= TIM_EGR_UG;

	// Start TIM6 counter
	TIM6->CR1 |= TIM_CR1_CEN;

	// Wait until TIM6 counter reaches delay
	while(TIM6->CNT < ms);

	// Stop TIM6 counter
	TIM6->CR1 &= ~TIM_CR1_CEN;
}



/*
 * pushbutton_timer_delay_us(uint16_t ms)
 * waits here for us
 */
void BSP_PB_DELAY_TIM_us(uint16_t ms)
{
	// Resets TIM3 counter
	TIM3->EGR |= TIM_EGR_UG;

	// Start TIM3 counter
	TIM3->CR1 |= TIM_CR1_CEN;

	// Wait until TIM3 counter reaches delay
	while(TIM3->CNT < ms);

	// Stop TIM3 counter
	TIM3->CR1 &= ~TIM_CR1_CEN;
}



void BSP_TIMER_PWM_Init()
{
	// Enable GPIOA clock
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// Configure PA8 and PA9 as Alternate Function
	GPIOA->MODER &= ~(GPIO_MODER_MODER8_Msk | GPIO_MODER_MODER9_Msk);
	GPIOA->MODER |=  (0x02 <<GPIO_MODER_MODER8_Pos) | (0x02 <<GPIO_MODER_MODER9_Pos);

	// Set PA8 and PA9 to AF2 (TIM1)
	GPIOA->AFR[1] &= ~(0x000000FF);
	GPIOA->AFR[1] |=  (0x00000022);

	// Enable TIM1 clock
	RCC -> APB2ENR |= RCC_APB2ENR_TIM1EN;

	// Reset TIM1 configuration
	TIM1->CR1  = 0x0000;
	TIM1->CR2  = 0x0000;
	TIM1->CCER = 0x0000;

	// Set TIM1 prescaler
	// Fck = 48MHz -> /48 = 50Hz counting frequency (1µs resolution)
	TIM1->PSC = (uint16_t) 960 -1;

	// Set Auto-Reload to period = 11ms
	TIM1->ARR = (uint16_t) 1000;

	// Enable Auto-Reload Preload register
	TIM1->CR1 |= TIM_CR1_ARPE;

	// Setup Input Capture
	TIM1->CCMR1 = 0x0000;
	TIM1->CCMR2 = 0x0000;

	// Setup PWM mode 1 output
	TIM1->CCMR1 |= (0x06 <<TIM_CCMR1_OC1M_Pos) | TIM_CCMR1_OC1PE;
	TIM1->CCMR1 |= (0x06 <<TIM_CCMR1_OC2M_Pos) | TIM_CCMR1_OC2PE;

	// Set default PWM values
	TIM1->CCR1 = 23;
	TIM1->CCR2 = 23;

	// Enable Outputs
	TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;

	// Enable Main output
	TIM1->BDTR |= TIM_BDTR_MOE;

	// Enable TIM1
	TIM1->CR1 |= TIM_CR1_CEN;
}





/*
 * BSP_NVIC_Init()
 * Setup NVIC controller for desired interrupts
 */
void BSP_NVIC_Init()
{
	// Set maximum priority for EXTI line 0 to 1 interrupts
	NVIC_SetPriority(EXTI0_1_IRQn, 0);
	// Enable EXTI line 0 to 1 for PB_LEFT and PB_BOTTOM interrupts
	NVIC_EnableIRQ(EXTI0_1_IRQn);

	// Enable sampling timer interrupts
	NVIC_SetPriority(TIM6_DAC_IRQn, 0);
	NVIC_EnableIRQ(TIM6_DAC_IRQn);


}
