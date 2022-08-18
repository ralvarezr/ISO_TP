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
 * @file   : os_core.h
 * @date   : 10 jul. 2022
 * @author : Ricardo Alvarez <ricardo.alvarezr94@gmail.com>
 * @version	v1.0.0
 */

#ifndef INC_OS_CORE_H_
#define INC_OS_CORE_H_

/********************** inclusions *******************************************/
#include <stdint.h>
#include <stdbool.h>
#include "list.h"

/********************** macros ***********************************************/

#define STACK_SIZE	        (256u)          // Tamaño de Stack de cada tarea.
#define TASK_NAME_SIZE      (256u)
#define MAX_TASKS_AMOUNT    (8u)            // Cantidad máxima de tareas.

/********************** typedef **********************************************/

/****************************************************************************
 *  Estado de las tareas.
 ****************************************************************************/
typedef enum 
{
        TASK_READY,
        TASK_RUNNING,
        TASK_BLOCKED
}task_status_t;


/****************************************************************************
 *  Estructura de las tareas.
 ****************************************************************************/
struct _task {
        uint32_t stack[STACK_SIZE/4];
        uint32_t stack_pointer;
        void *entry_point;
        task_status_t status;
        uint8_t priority;
        uint8_t id;
        uint32_t ticks_blocked;
       // char name[TASK_NAME_SIZE];
};

typedef struct _task task_t;

/****************************************************************************
 *  Estructura de los semáforos binarios.
 ****************************************************************************/
struct _semaphore {
        bool taken;
        task_t *tasks_blocked[MAX_TASKS_AMOUNT];
        uint8_t n_tasks_blocked;
        task_t *task_taken;						//Variable para llevar el control de cuál tarea tomó el semáforo.
};

typedef struct _semaphore semaphore_t;

/****************************************************************************
 *  Estructura de las colas.
 ****************************************************************************/
struct _queue {
        lista_t *list;
        uint8_t max_size;
        task_t *task_blocked_send;
        task_t *task_blocked_recv;

};

typedef struct _queue queue_t;

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/

void os_init(void);
void os_task_init(task_t *task, void *entry_point, uint8_t priority);
void os_cpu_yield(void);
void os_enter_critical(void);
void os_exit_critical(void);

void task_delay(uint32_t time);

void semaphore_init(semaphore_t *semaphore);
void semaphore_take(semaphore_t *semaphore);
void semaphore_give(semaphore_t *semaphore);

bool queue_create(queue_t *queue, uint8_t lenght);
void queue_receive(queue_t *queue, uint32_t *buffer);
void queue_send(queue_t *queue, uint32_t element);
void queue_delete(queue_t *queue);


#endif /* INC_OS_CORE_H_ */
/********************** end of file ******************************************/
