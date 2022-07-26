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

//#define MILLISECOND	1000
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
	SysTick_ClockConfig(SysTick_ClockMax);

}


/*==================[Definicion de tareas para el OS]==========================*/

void tarea1(void)
{
	uint32_t i = 0;
	while (1)
	{
		i++;
		if (i >= 500000)
		{
			toggle_LED1();
			i = 0;
		}
	}
}

void tarea2(void)
{

	uint32_t j = 0;

	while (1)
	{
		j++;
		if (j >= 600000)
		{
			toggle_LED2();
			j = 0;
		}
	}
}

void tarea3(void)
{

	uint32_t k = 0;
	while (1)
	{
		k++;
		if (k >= 700000)
		{
			toggle_LED3();
			k = 0;
		}
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


	os_task_init(&s_tarea1, tarea1);
	os_task_init(&s_tarea2, tarea2);
	os_task_init(&s_tarea3, tarea3);

	os_init();


	while (1)
	{

	}
	return 0;
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/

