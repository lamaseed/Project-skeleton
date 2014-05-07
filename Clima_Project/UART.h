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
#include "FreeRTOS.h"

#include "queue.h"
 #include "stm32f10x.h"
#include "semphr.h"
#include "stm32f10x_gpio.h" 
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

typedef struct {
	 uint16_t Value_1;
	 uint16_t Value_2;
 } Room_Temp;

typedef struct {

  Room_Temp *Temperature;		  // The current Temperature. This variable
                              // should only be accessed through
								              // the function "getTemperature"
  xSemaphoreHandle lock;      // Mutex semaphore protecting the struct

  portTickType pollingPeriod;	// how often the status of pins is polled


} Room; 

void SendData(void* str);
void setupComTask(Room *Floor);
void setTemperature(Room *Floor);
void getPosition(Room *Floor,uint16_t *Array);
