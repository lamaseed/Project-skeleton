/**
 * Program skeleton for the course "Programming embedded systems"
 *
 * Project Authors:
 * 	Zacharopoulos 		Georgios
 * 	Karkanis 					Iosif
 *	Ntounas						Georgios
 *	Stamatakopoulos		Panagiotis
 */

/**
 * Main file of the system; module setup and initialisation takes
 * place here
 */
 
#include <stdio.h>
#include <string.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "setup.h"
#include "assert.h"
#include "stm3210c_eval_ioe.h"

#include "UART.h"
#include "LCD.h"
/*-----------------------------------------------------------*/
Room_Temp room_Temp[] = { {0,0},{0,0} }; // 0.o

Room Floor_1 = {
	room_Temp,
	NULL,
	100
};
/* Configuration for the USART2 on PD5,PD6 */
void setupUARTModule() {
	
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Enable GPIO, AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
	
  /* Enable UART clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;  // PD.05 USART2.TX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
 
  /* Configure USART Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; // PD.06 USART2.RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
 
  GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);   // Re-map USART, as USART2 is used as alternate pins on PD5/6, requires AFIO	
  
	USART_InitStructure.USART_BaudRate = 4800;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 
  /* USART configuration */
  USART_Init(USART2, &USART_InitStructure);
 
  /* Enable the USART2 */
  USART_Cmd(USART2, ENABLE);
	/* Here the USART2 receive interrupt is enabled
	 * and the interrupt controller is configured
	 * to jump to the USART2_IRQHandler() function
	 * if the USART2 receive interrupt occurs
	 */	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;					  // we want to configure the USART2 interrupts
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		// this sets the priority group of the USART2 interrupts
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					// this sets the sub priority inside the group
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							// the USART2 interrupts are globally enabled
	NVIC_Init(&NVIC_InitStructure);															// the properties are passed to the NVIC_Init function which takes care of the low level stuff
	
	Floor_1.lock =	xSemaphoreCreateMutex();
	
	setupComTask(&Floor_1);
}
/*-----------------------------------------------------------*/
/* Functions defined in global.h */
void getCurrentTemperature(uint16_t *Array) {
   getPosition(&Floor_1, Array);
}

int main(void)
{
	
  prvSetupHardware();
	initDisplay();
	setupUARTModule();
	
  //printf("Setup completed\n");  // this is redirected to USART 1
	
	printf("Setup complete    ");  // this is redirected to the display
  vTaskStartScheduler();
	
  assert(0);
}

/**************************************************************************************/
