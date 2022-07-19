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
#include "os_core.h"

/*==================[macros and definitions]=================================*/

#define MILLISECOND	1000
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
	SysTick_ClockConfig(SysTick_ClockMax/MILLISECOND);

}


/*==================[Definicion de tareas para el OS]==========================*/

void tarea1(void)  {
	uint32_t i = 5;
	while (1) {
		i++;
	}
}


void tarea2(void)  {
	uint32_t j = 10;
	while (1) {
		j++;
	}
}

void tarea3(void)  {
	uint32_t k = 20;
	while (1) {
		k++;
	}
}

/*============================================================================*/

int main(void)
{

	hardware_init();


	os_task_init(&s_tarea1, tarea1);
	os_task_init(&s_tarea2, tarea2);
	os_task_init(&s_tarea3, tarea3);

	os_init();

	// No debería llegar a este punto. Ya el scheduler tomó la CPU.
	while (1)
	{
		//__WFI();
	}
	return 0;
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/

