/*
 * os_core.h
 *
 *  Created on: 10 jul. 2022
 *      Author: Ricardo
 */

#ifndef INC_OS_CORE_H_
#define INC_OS_CORE_H_

/********************** inclusions *******************************************/
#include <stdint.h>
#include <stdbool.h>

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

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/

void os_init(void);
void os_task_init(task_t *task, void *entry_point, uint8_t priority);

void task_delay(uint32_t time);

void semaphore_init(semaphore_t *semaphore);
void semaphore_take(semaphore_t *semaphore);
void semaphore_give(semaphore_t *semaphore);


#endif /* INC_OS_CORE_H_ */
/********************** end of file ******************************************/
