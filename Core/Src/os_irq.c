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
 * @file   : os_irq.c
 * @date   : 18 aug. 2022
 * @author : Ricardo Alvarez <ricardo.alvarezr94@gmail.com>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/
#include "os_irq.h"


/********************** macros and definitions *******************************/
#define IRQ_AMOUNT	(91u)

/********************** internal functions declaration ***********************/
static void os_IRQHandler(IRQn_Type irq);


void USART3_IRQHandler(void){os_IRQHandler(       USART3_IRQn      );}
void EXTI15_10_IRQHandler(void){os_IRQHandler(       EXTI15_10_IRQn      );}


/********************** internal data definition *****************************/
static void *isr_vector[IRQ_AMOUNT];

/********************** external data definition *****************************/

/********************** internal functions definition ************************/

/********************** external functions definition ************************/

/************************************************************************************************
 * @fn bool os_irq_remove(IRQn_Type)
 * @brief Remueve una IRQ del vector de interrupciones.
 *
 * @param irq Interrupción a remover.
 * @return true Si la IRQ pudo ser removida. false Si no estaba instalada previamente.
 ************************************************************************************************/
bool os_irq_remove(IRQn_Type irq)
{

	bool retval = false;

	if (NULL != isr_vector[irq])
	{
		isr_vector[irq] = NULL;

		NVIC_ClearPendingIRQ(irq);
		NVIC_DisableIRQ(irq);

		retval = true;
	}

	return retval;
}


/**
 * @fn bool os_irq_install(IRQn_Type, void*)
 * @brief Instala una IRQ en el vector de interrupciones.
 *
 * @param irq Interrupción a instalar.
 * @param isr Función que se ejecutará en esa interrupción.
 * @return true Si se pudo instalar. false si ya estaba instalada previamente.
 */
bool os_irq_install(IRQn_Type irq, void *isr)
{
	bool retval = false;

	if (NULL == isr_vector[irq])
	{
		isr_vector[irq] = isr;

		NVIC_ClearPendingIRQ(irq);
		NVIC_EnableIRQ(irq);

		retval = true;
	}

	return retval;
}

/**
 * @fn void os_IRQHandler(IRQn_Type)
 * @brief Handler genérico que ejecuta la isr asociada a la irq.
 *
 * @param irq Interrupción a ser ejecutada.
 * @return None.
 */
void os_IRQHandler(IRQn_Type irq)
{
	/**
	 * Cambio el OS a estado IRQ.
	 */
	os_enter_irq_mode();


	void (*user_function)(void) = isr_vector[irq];

	/**
	 * Si la función del usuario fue previamente instalada,
	 * se ejecuta.
	 */
	if (NULL != user_function)
	{
		user_function();
	}

	NVIC_ClearPendingIRQ(irq);

	/**
	 * Si en la ISR se ejecutó algo que requiere
	 * que se haga un re-scheduling, entonces
	 * se hace un CPU Yield y se limpia
	 * la bandera correspondiente.
	 */
	if (os_get_schedulerIRQ_state())
	{
		os_cpu_yield();

		os_unset_schedulerIRQ();
	}

	/**
	 * Cambio el OS al estado previo de entrar a la IRQ.
	 */
	os_exit_irq_mode();
}


/********************** end of file ******************************************/
