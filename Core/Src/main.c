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

semaphore_t sem;


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
	semaphore_give(&sem);
	//Programa de prueba.

	while (1)
	{
		task_delay(2000);

		semaphore_take(&sem);
		{

			toggle_LED1();

			task_delay(500);

			toggle_LED1();

			task_delay(500);

			toggle_LED1();

			task_delay(5000);
		}
		semaphore_give(&sem);
	}
}


void tarea2(void)
{
	semaphore_give(&sem);
	//Programa de prueba.
	while (1)
	{
		task_delay(3000);

		semaphore_take(&sem);
		{

			toggle_LED2();

			task_delay(500);

			toggle_LED2();

			task_delay(500);

			toggle_LED2();

			task_delay(5000);
		}
		semaphore_give(&sem);

	}
}

void tarea3(void)
{
	semaphore_give(&sem);
	//Programa de prueba.
	while (1)
	{
		task_delay(1000);

		semaphore_take(&sem);
		{

			toggle_LED3();

			task_delay(500);

			toggle_LED3();

			task_delay(500);

			toggle_LED3();

			task_delay(5000);

		}
		semaphore_give(&sem);

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
	os_task_init(&s_tarea3, tarea3, 3);

	semaphore_init(&sem);

	os_init();

	while (1)
	{

	}

	return 0;
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/

