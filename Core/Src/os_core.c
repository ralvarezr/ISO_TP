/*
 * Copyright (c) 2022 Ricardo Alvarez <ricardo.alvarezr94@gmail.com>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @file   : os_core.c
 * @date   : 10 jul. 2022
 * @author : Ricardo Alvarez <ricardo.alvarezr94@gmail.com>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/
#include "os_core.h"
#include "stm32f429xx.h"
#include <stddef.h>

#include "GPIO.h" //**************PRUEBA******************

/********************** macros and definitions *******************************/

/*************	POSICIONES DENTRO DEL STACK	********/
#define XPSR_POS	1
#define PC_POS		2
#define LR_POS		3
#define R12_POS		4
#define R3_POS		5
#define R2_POS		6
#define R1_POS		7
#define R0_POS		8
#define LR_IRQ_POS	9
#define R4_POS		10
#define R5_POS		11
#define R6_POS		12
#define R7_POS		13
#define R8_POS		14
#define R9_POS		15
#define R10_POS		16
#define R11_POS		17


/************	DEFINCIONES VARIAS	***************/
#define EXEC_RETURN 			(0xFFFFFFF9)
#define AUTO_STACKING_SIZE		(8u)
#define AUTO_STACKING_FULL_SIZE	(17u)

/********************** internal data declaration ****************************/
static os_control_t os_control = {
		.system_status = OS_BOOTING,
		.n_tasks = 0,
		.n_tasks_blocked = 0,
		.error = OS_ERROR_GENERAL
};

static task_t idle;

/********************** internal functions declaration ***********************/
static void scheduler(void);
static void init_idle_task(void);
static task_t* get_next_task_ready(void);

/********************** internal data definition *****************************/

/********************** external data definition *****************************/

/********************** internal functions definition ************************/

/********************** external functions definition ************************/

/************************************************************************************************
 * @fn void idle_task(void)
 * @brief Tarea Idle (Weak).
 *
 * @details
 * Esta tarea se ejecuta solo cuando no hay otras tareas en estado Ready
 * para ser ejecutadas. Se define como weak para que pueda ser redefinida por el usuario.
 *
 * @param None.
 * @return None.
 ************************************************************************************************/
__attribute__((weak)) void idle_task(void)
{
	//Programa de prueba para la tarea Idle.
	//Enciden el Led Rojo de la placa nucleo y luego desbloquea las dos tareas bloqueadas.
	uint32_t j = 0;
	uint32_t h = 0;

	while(1)
	{
		//__WFI();
		j++;
		if (j >= 6000000)
		{
			toggle_LED3();
			j = 0;
			h++;
			if (h == 4)
			{
				test_unblock_task(os_control.tasks_list[0]);
				test_unblock_task(os_control.tasks_list[1]);
				h = 0;
			}
		}
	}
}

/************************************************************************************************
 * @fn void tick_hook(void)
 * @brief tickHook (Weak).
 *
 * @details
 * Esta función se ejecuta en el contexto de interrupción del SysTick. Es la última tarea a ser ejecutada
 * por el SysTick_Handler. Se define como weak para que pueda ser redefinida por el usuario.
 *
 * @param None.
 * @return None.
 ************************************************************************************************/
__attribute__((weak)) void tick_hook(void)
{

	__asm("nop");
}

/************************************************************************************************
 * @fn void return_hook(void)
 * @brief Hook de Retorno de las tareas (Weak).
 *
 * @details
 * Esta función se ejecuta solo cuando una tarea termina su ejecución. Esto no debería suceder ya que
 * no están definidas las tareas de tipo One-Shot.
 * Se define como weak para que pueda ser redefinida por el usuario.
 *
 * @param None.
 * @return None.
 ************************************************************************************************/
__attribute__((weak)) void return_hook(void)
{
	while(1)
	{

	}
}

/************************************************************************************************
 * @fn void error_hook(void)
 * @brief Hook de Error del OS (Weak).
 *
 * @details
 * Esta función se ejecuta solo cuando ocurre un error relacionado al OS.
 * Se define como weak para que pueda ser redefinida por el usuario.
 *
 * @param None.
 * @return None.
 ************************************************************************************************/
__attribute__((weak)) void error_hook(void)
{
	while(1)
	{

	}
}

/************************************************************************************************
 * @fn void init_idle_task(void)
 * @brief Inicializa la tarea Idle.
 *
 * @details
 * Esta tarea está presente siempre en el OS pero no está dentro de la lista de tareas.
 * Tiene la prioridad más baja de todas las tareas.
 *
 ************************************************************************************************/
void init_idle_task(void)
{
	/**
	 * Configuración del Stack de la tarea.
	 */
	idle.stack[STACK_SIZE / 4 - XPSR_POS] = 1 << 24;					// xPSR.T = 1. Set Thumb bit.
	idle.stack[STACK_SIZE / 4 - PC_POS] = (uint32_t) idle_task;			// Entry Point.
	idle.stack[STACK_SIZE / 4 - LR_POS] = (uint32_t) return_hook;		// Dirección de retorno de la tarea. (No deberia retornar nunca).
	idle.stack[STACK_SIZE / 4 - LR_IRQ_POS] = EXEC_RETURN;

	/**
	 * Configuración del resto de parámetros de la tarea.
	 */
	idle.stack_pointer = (uint32_t) (idle.stack + (STACK_SIZE / 4) - AUTO_STACKING_FULL_SIZE);
	idle.status = TASK_READY;
	idle.id = 100;
	idle.priority = 100;
	idle.entry_point = idle_task;

}

/************************************************************************************************
 * @fn void os_init(void)
 * @brief Inicialización del OS.
 *
 * @details
 * Configura la prioridad de la interrupción PendSV a la más baja,
 * e inicializa la estructura de control del OS.
 * Esta funcion debe ser llamada ANTES que la funcion os_task_init.
 *
 * @param None.
 * @return None.
 *************************************************************************************************/
void os_init(void)
{

	NVIC_SetPriority(PendSV_IRQn, (1 << __NVIC_PRIO_BITS) - 1);

	os_control.current_task = NULL;
	os_control.next_task = NULL;
	os_control.system_status = OS_FRESH;

	init_idle_task();

}

/************************************************************************************************
 * @fn void os_task_init(task_t*, void*)
 * @brief Inicializa las tareas.
 *
 * @details
 * Configura los parámetros de cada tarea y actualiza la estructura
 * de control del OS.
 *
 * @param *task Puntero a la estructura de la tarea.
 * @param *entry_point Puntero a la tarea.
 * @param priority La prioridad de la tarea. (Entre 0 y 3, siendo 0 la mayor prioridad).
 * @return None.
 ************************************************************************************************/
void os_task_init(task_t *task, void *entry_point, uint8_t priority)
{

	if(MIN_PRIORITY < priority)
	{
		os_control.error = OS_ERROR_TASK_PRIORITY;
		error_hook();
	}

	static uint32_t tasks_amount = 0;

	if (tasks_amount < MAX_TASKS_AMOUNT)
	{

		/**
		 * Configuración del Stack de la tarea.
		 */
		task->stack[STACK_SIZE/4 - XPSR_POS] = 1 << 24;					// xPSR.T = 1. Set Thumb bit.
		task->stack[STACK_SIZE/4 - PC_POS] = (uint32_t)entry_point;		// Entry Point.
		task->stack[STACK_SIZE/4 - LR_POS] = (uint32_t)return_hook;		// Dirección de retorno de la tarea. (No deberia retornar nunca).
		task->stack[STACK_SIZE/4 - LR_IRQ_POS] = EXEC_RETURN;

		/**
		 * Configuración del resto de parámetros de la tarea.
		 */
		task->stack_pointer = (uint32_t)(task->stack + (STACK_SIZE/4) - AUTO_STACKING_FULL_SIZE);
		task->status = TASK_READY;
		task->id = tasks_amount;
		task->entry_point = entry_point;
		task->priority = priority;

		/**
		 * Actualizo la lista de tareas del OS.
		 */
		os_control.n_tasks = tasks_amount;
		os_control.tasks_list[os_control.n_tasks++] = task;
		tasks_amount++;
	}
	else
	{
		/**
		 * Si se supera la cantidad de tareas permitidas, indico el error y voy al hook.
		 */
		os_control.error = OS_ERROR_TASKS_COUNT;
		error_hook();
	}

}

/************************************************************************************************
 * @fn void SysTick_Handler(void)
 * @brief Handler del SysTick del sistema.
 *
 * @details
 * Llama al scheduler el OS para definir la próxima tarea a ejecutar, activa la interrupcion
 * por PendSV y limpia el pipeline de ejecución de procesador.
 *
 * @param None.
 * @return None.
 ************************************************************************************************/
void SysTick_Handler(void)
{
	/**
	 * Hasta que no se haya inicializado el OS, el scheduler ni la excep PendSV deben ejecutarse.
	 */
	if (OS_BOOTING != os_control.system_status)
	{

		/**
		 * Se ejecuta el scheduler para determinar cuál es la siguiente tarea a ser ejecutada y
		 * prepararla para el cambio de contexto.
		 */
		scheduler();

		/**
		 * Se setea el bit correspondiente a la excepcion PendSV
		 */
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

		/**
		 * Instruction Synchronization Barrier; flushes the pipeline and ensures that
		 * all previous instructions are completed before executing new instructions
		 */
		__ISB();

		/**
		 * Data Synchronization Barrier; ensures that all memory accesses are
		 * completed before next instruction is executed
		 */
		__DSB();

		/**
		 * Antes de salir del SysTick_Handler ejecuto el hook del tick.
		 */
		tick_hook();
	}
}

/************************************************************************************************
 * @fn uint32_t getContextoSiguiente(uint32_t)
 * @brief Obtiene el Stack Pointer del contexto de la siguiente tarea a ejecutar.
 *
 * @details
 * Almacena el Stack Pointer de la tarea actual y devuelve el de la tarea siguiente.
 * Esta función es ejecutada desde la excepción PendSV.
 *
 * @param current_sp Stack Pointer de la tarea que está siendo ejecutada.
 * @return Stack Pointer de la siguiente tarea a ejecutar.
 ************************************************************************************************/
uint32_t getContextoSiguiente(uint32_t current_sp)
{
	uint32_t next_sp;


	if (os_control.system_status == OS_FRESH)
	{
		/**
		 * Si el OS acaba de arrancar, el Stack Pointer actual debe ser descartado
		 * ya que pertenece al stack de arranque del procesador, y se configura
		 * para que tarea actual (la primera según haya definido el scheduler) sea ejecutada.
		 */

		next_sp = os_control.current_task->stack_pointer;

		os_control.current_task->status = TASK_RUNNING;

		os_control.system_status = OS_RUNNING;
	}
	else
	{
		/**
		 * Si el OS ya está en ejecución, se almacena el SP de la tarea actual,
		 * esta tarea se pone en estado Ready sólo si no está bloqueada,
		 * y la tarea siguiente se convierteen la actual a ser ejecutada
		 * y se pone en estado Running.
		 */
		os_control.current_task->stack_pointer = current_sp;
		if (os_control.current_task->status != TASK_BLOCKED)
		{
			os_control.current_task->status = TASK_READY;
		}

		next_sp = os_control.next_task->stack_pointer;

		os_control.current_task = os_control.next_task;
		os_control.current_task->status = TASK_RUNNING;
	}

	return next_sp;
}

/************************************************************************************************
 * @fn void scheduler(void)
 * @brief Planificador del OS.
 *
 * @details
 * Determina cuál es la tarea siguiente a ser ejecutada. Las tareas son elegidas
 * mediante Round Robin. Esta función es de uso interno del OS, no es visible al usuario final.
 *
 * @param None.
 * @return None.
 *************************************************************************************************/
void scheduler(void)
{

	if(os_control.system_status == OS_FRESH)
	{
		/**
		 * Si el OS acaba de arrancar se elige la primera tarea en la lista como
		 * tarea actual para que sea ejecutada.
		 */

		os_control.current_task = os_control.tasks_list[0];
	}
	/**
	 * Si la cantidad de tareas bloqueadas es igual a la cantidad de tareas,
	 * entonces la siguiente tarea a ejecutar es la tarea idle.
	 */
	else if (os_control.n_tasks_blocked == os_control.n_tasks)
	{
		os_control.next_task = &idle;
	}
	/**
	 * Si el OS ya está en ejecución, entonces, según la política de scheduling de este OS,
	 * la siguiente tarea a ser ejecutada es la siguiente en la lista de tareas que no
	 * se encuentre bloqueada.
	 */
	else
	{
		os_control.next_task = get_next_task_ready();
	}

}

/*************************************************************************************************
 * @fn task_t* get_next_task_ready(void)
 * @brief Devuelve la siguiente tarea en estado Ready.
 *
 * @details
 * Busca en la lista de tareas cuál es la siguiente en estado Ready.
 * Si ninguna está en estado Ready, entonces verifica que la tarea actual esté en estado Running,
 * lo que indica que es la única tarea que no está bloqueada.
 * Por defecto se devuelve la tarea idle, ya que si no se encuentra alguna tarea en estado ready
 * o running es porque todas están bloqueadas.
 *
 * @param None.
 * @return task_t* Puntero a la siguiente tarea a ser ejecutada.
 *************************************************************************************************/
task_t* get_next_task_ready(void)
{
	task_t *retval = &idle;

	uint32_t index = os_control.current_task->id + 1;
	if(index >= os_control.n_tasks)
	{
		index = 0;
	}

	for (uint32_t j = 0; j < os_control.n_tasks; j++)
	{
		if((TASK_READY == os_control.tasks_list[index]->status) || (TASK_RUNNING == os_control.tasks_list[index]->status))
		{
			retval = os_control.tasks_list[index];
			break;
		}

		index++;
		if(index >= os_control.n_tasks)
		{
			index = 0;
		}
	}

	return retval;
}

/***********************************************************************************************************
 * FUNCIONES DE PRUEBA
 ***********************************************************************************************************/

/************************************************************************************************
 * @fn void test_block_task(void)
 * @brief
 *  Bloquea la tarea que se está ejecutando y llama al scheduler para cambiar a la siguiente tarea.
 *
 * @param None.
 * @return None.
 ************************************************************************************************/
void test_block_task(void)
{
	if(os_control.current_task->status == TASK_RUNNING)
	{
		os_control.current_task->status = TASK_BLOCKED;
		os_control.n_tasks_blocked++;
		scheduler();
	}
}

void test_unblock_task(task_t *task)
{
	if(task != NULL)
	{
		task->status = TASK_READY;
		os_control.n_tasks_blocked--;
	}
}
/********************** end of file ******************************************/
