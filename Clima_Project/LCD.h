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

 */
 
#include "FreeRTOS.h"

#include "queue.h"
 #include "stm32f10x.h"
#include "semphr.h"

#define WIDTH 320


void initDisplay (void);
