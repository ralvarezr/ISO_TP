/*
 * main.c
 *
 *  Created on: Jul 10, 2022
 *      Author: Ricardo
 */

/*==================[inclusions]=============================================*/

#include "main.h"
#include "RCC.h"
#include "SysTick.h"
#include "GPIO.h"
#include "os_core.h"

/*==================[macros and definitions]=================================*/

#define MILLISECOND	(100u)
/*==================[Global data declaration]==============================*/

task_t s_tarea1, s_tarea2, s_tarea3;


/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/**
 * @brief Funcion de Inicizalicion del Hardware.
 * @return None.
 */
static void hardware_init(void)
{
	Sys_ClockConfig();
	SysTick_ClockConfig(SysTick_ClockMax / MILLISECOND);

}


/*==================[Definicion de tareas para el OS]==========================*/

void tarea1(void)
{
	//Programa de prueba.

	while (1)
	{
		toggle_LED1();
		os_task_delay(2000);

	}
}


void tarea2(void)
{

	//Programa de prueba.
	while (1)
	{
		toggle_LED2();
		os_task_delay(1000);
	}
}

void tarea3(void)
{

	//Programa de prueba.
	while (1)
	{
		toggle_LED3();
		os_task_delay(500);
	}
}

/*============================================================================*/

int main(void)
{

	hardware_init();

	GPIO_Leds_Init();

	write_LED1(LOW);
	write_LED2(LOW);
	write_LED3(LOW);




	os_task_init(&s_tarea1, tarea1, 0);
	os_task_init(&s_tarea2, tarea2, 0);
	os_task_init(&s_tarea3, tarea3, 0);

	os_init();


	while (1)
	{

	}

	return 0;
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/

