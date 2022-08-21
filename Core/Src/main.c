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
#include "INT_EX.h"
#include "os_core.h"
#include "os_irq.h"
#include "USART.h"

/*==================[macros and definitions]=================================*/
#define MILLISECOND	(100u)

/*==================[Global data declaration]==============================*/
task_t s_tarea_led_verde, s_tarea_led_rojo, s_tarea_led_azul, s_tarea_uart;

semaphore_t sem1, sem2, sem3;

queue_t cola;
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

	GPIO_Leds_Init();

	GPIO_Button_Init();

	write_LED1(LOW);
	write_LED2(LOW);
	write_LED3(LOW);

	USART_Init(115200);

}


/*==================[Definicion de tareas para el OS]==========================*/

/**
 * @fn void tarea_uart(void)
 * @brief Imprime por USART3 el dato recibido por la cola.
 *
 * @param None.
 * @return None.
 */
void tarea_uart(void)
{
	uint32_t cantidad;

	while (1)
	{

		queue_receive(&cola, &cantidad);

		cantidad += 48;

		os_enter_critical();
		{

			USART_Tx(cantidad);
			USART_Tx_String("\r\n");

			os_exit_critical();
		}

	}
}


/**
 * @fn void tarea_led_verde(void)
 * @brief Hace un toggle del Led 1 (Verde) cada un 0.5 segundo.
 *
 * @param None.
 * @return None.
 */
void tarea_led_verde(void)
{

	semaphore_give(&sem1);

	while(1)
	{
		semaphore_take(&sem1);
		{
			toggle_LED1();

			semaphore_give(&sem1);
		}

		task_delay(500);
	}
}

/**
 * @fn void tarea_led_rojo(void)
 * @brief Hace un toggle del Led 2 (Rojo) cada un 1 segundo.
 *
 * @param None.
 * @return None.
 */
void tarea_led_rojo(void)
{

	semaphore_give(&sem2);

	while(1)
	{
		semaphore_take(&sem2);
		{
			toggle_LED2();

			semaphore_give(&sem2);
		}

		task_delay(1000);
	}
}

/**
 * @fn void tarea_led_azul(void)
 * @brief Hace un toggle del Led 3 (Azul) cada un 1.5 segundo.
 *
 * @param None.
 * @return None.
 */
void tarea_led_azul(void)
{

	semaphore_give(&sem3);

	while(1)
	{
		semaphore_take(&sem3);
		{
			toggle_LED3();

			semaphore_give(&sem3);
		}

		task_delay(1500);
	}
}

/**
 * @fn void usr_handler(void)
 * @brief Handler de Interrupción del usuario.
 *
 * @details
 * Lleva el conteo de cuentas veces se generó la interrupción y la envía a una cola.
 *
 * @param None.
 * @return None.
 */
void usr_handler(void)
{

	static uint32_t i = 0;

	if (LOW == read_BUTTON())
	{

		queue_send(&cola, ++i);

		INT_Clean_Int();
	}

}

/*============================================================================*/

int main(void)
{

	/**
	 * Este programa ejecuta 2 tareas. Una para encender el Led 1 cada 1s,
	 * y otra la cual está esperando un dato desde una cola para imprimirlo.
	 *
	 * El dato que va a la cola se genera desde la interrupción de botón de la placa.
	 * Esta interrupción lleva el conteo de cuántes veces ha sucedido.
	 */

	/**
	 * Se inicializa el hardware de la placa.
	 */
	hardware_init();

	/**
	 * Inicialización de la interrupción de botón de la placa.
	 */
	INT_Init();

	/**
	 * Se inicializan las dos tareas a ejecutar.
	 */
	os_task_init(&s_tarea_uart, tarea_uart, 0);
	os_task_init(&s_tarea_led_verde, tarea_led_verde, 3);
	os_task_init(&s_tarea_led_rojo, tarea_led_rojo, 3);
	os_task_init(&s_tarea_led_azul, tarea_led_azul, 3);

	/**
	 * Se inicializan los semáforos para encender los leds.
	 */
	semaphore_init(&sem1);
	semaphore_init(&sem2);
	semaphore_init(&sem3);

	/**
	 * Se crea la cola que envia datos desde la interrupción a la tarea de uart.
	 */
	queue_create(&cola, 1);

	/**
	 * Instala la interrupción por el botón de la placa.
	 */
	if (!os_irq_install(EXTI15_10_IRQn, usr_handler))
	{
		while(1)
		{
		}
	}

	/**
	 * Se inicializa el OS y arranca.
	 */
	os_init();

	/**
	 * No se debería ejecutar a partir de este punto.
	 */
	while (1)
	{

	}

	queue_delete(&cola);

	return 0;
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/

