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
 * The UART module, which is responsible for the 
 * communication between the ARM and the AVR
 */
 
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "stm32f10x_gpio.h" 
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"

#include "UART.h"
#include "assert.h"


/*--------------USART2 IRQ_HANDLER---------------*/

uint16_t input_Data[2];				  // Current Temperature in the form of 2 chars
uint8_t switcher = 0;
xSemaphoreHandle ComToken;

void USART2_IRQHandler(void){
	
	//if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  ??
	
		input_Data[switcher%2] = USART_ReceiveData(USART2);
		switcher++;	
	
	if (switcher%2 == 0) 
		xSemaphoreGive(ComToken);
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
}

/*--------------COMMUNICATION TASK---------------*/

static void UpdateTask(void *params)
{
	Room Floor = *((Room*)params);
  for(;;)
  {  
    xSemaphoreTake(ComToken, portMAX_DELAY);
		
		setTemperature(&Floor);									// Here we can add FLOOR_NUM = {0,1} to navigate through floors
		
		printf("%c",Floor.Temperature->Value_1);
		printf("%c",Floor.Temperature->Value_2);
  } 
}

/*--------------------------------------------*/
void setupComTask(Room *Floor) {
  portBASE_TYPE res;
  vSemaphoreCreateBinary(ComToken);
  res = xTaskCreate(UpdateTask, "UpdateTask",
                    100,(void*)Floor,
										1, NULL);
 assert(res == pdTRUE);
}

/*--------------------------------------------*/
void setTemperature(Room *Floor) {

  	xSemaphoreTake(Floor->lock, portMAX_DELAY);
			Floor->Temperature->Value_1 = input_Data[0];
			Floor->Temperature->Value_2 = input_Data[1];
		xSemaphoreGive(Floor->lock);

}
void getPosition(Room *Floor,uint16_t *Array) {
	
		xSemaphoreTake(Floor->lock, portMAX_DELAY);
			memcpy(Array,Floor->Temperature,2*sizeof(uint16_t));
		xSemaphoreGive(Floor->lock);
	
}
/**************************************************************************************/
