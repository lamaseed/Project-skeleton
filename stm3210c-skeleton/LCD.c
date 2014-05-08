/**
 * Program skeleton for the course "Programming embedded systems"
 *
 * Project Authors:
 * 	Zacharopoulos 		Georgios
 * 	Karkanis 		Iosif
 *	Ntounas			Georgios
 *	Stamatakopoulos		Panagiotis
 */

/**

 */
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
 #include "stm32f10x.h"
#include "GLCD.h"

#include "LCD.h"
#include "Global.h"
/*-----------------------------------------------------------*/


/**
 * Display stdout on the display
 */
xQueueHandle printQueue;
xSemaphoreHandle lcdLock;

static void printTask(void *params) {
  unsigned char str[21] = "                    ";
  portTickType lastWakeTime = xTaskGetTickCount();
  int i;

  for (;;) {
    xSemaphoreTake(lcdLock, portMAX_DELAY);
    GLCD_setTextColor(White);
    GLCD_displayStringLn(Line9, str);
    xSemaphoreGive(lcdLock);

    for (i = 0; i < 19; ++i)
	  str[i] = str[i+1];

    if (!xQueueReceive(printQueue, str + 19, 0))
	  str[19] = ' ';

	vTaskDelayUntil(&lastWakeTime, 100 / portTICK_RATE_MS);
  }
}


/*-----------------------------------------------------------*/
 static void lcdTask(void *params) {
	uint16_t Current_Temp[2];
  for (;;) {
		
    xSemaphoreTake(lcdLock, portMAX_DELAY);
		getCurrentTemperature(Current_Temp);
		GLCD_setTextColor(White);
    GLCD_displayChar(Line5, WIDTH - 260, Current_Temp[0]);
		GLCD_setTextColor(White);
		GLCD_displayChar(Line5, WIDTH - 280, Current_Temp[1]);
		xSemaphoreGive(lcdLock);
		
		
		
    vTaskDelay(1000 / portTICK_RATE_MS);
  }
}
/*-----------------------------------------------------------*/
void initDisplay () {
  /* LCD Module init */
  lcdLock = xSemaphoreCreateMutex();  
	printQueue = xQueueCreate(128, 1);
	
  GLCD_init();
  GLCD_clear(Black);
  GLCD_setTextColor(White);
  GLCD_displayStringLn(Line5, " Temperature :");
	
	xTaskCreate(lcdTask, "lcd", 100, NULL, 1, NULL);
	xTaskCreate(printTask, "print", 100, NULL, 1, NULL);
}
 /*-----------------------------------------------------------*/
/* Retarget printing to the serial port 1 */
int fputc(int ch, FILE *f) {
  unsigned char c = ch;
  xQueueSend(printQueue, &c, 0);
  return ch;
}
 /*-----------------------------------------------------------*/
