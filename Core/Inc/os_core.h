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
#define MIN_PRIORITY		(3u)			// Prioridad Mínima.
#define MAX_PRIORITY		(0u)			// Prioridad Máxima.
#define PRIORITIES_AMOUNT	(4u)			// Cantidad de Prioridades Implementadas.

/********************** typedef **********************************************/

/****************************************************************************
 *  Errores del OS.
 ****************************************************************************/
typedef enum
{
        OS_ERROR_GENERAL 		= -1,
        OS_ERROR_TASKS_COUNT	= -2,
		OS_ERROR_TASK_PRIORITY	= -3
}os_error_t;

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
 *  Estados del OS.
 ****************************************************************************/
typedef enum
{
		OS_BOOTING,
        OS_FRESH,
        OS_RUNNING
} os_status_t;

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
 *  Estructura de control interna de OS.
 ****************************************************************************/
struct _os_control {
        os_status_t system_status; 
        bool schedulerIRQ;              	// scheduling al volver de IRQ
        task_t *current_task;
        task_t *next_task;
        os_error_t error;
        task_t *tasks_list[MAX_TASKS_AMOUNT];
        uint32_t n_tasks;
        uint32_t n_tasks_blocked;			// Cantidad de tareas bloqueadas.
};

typedef struct _os_control os_control_t;

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/

void os_init(void);
void os_task_init(task_t *task, void *entry_point, uint8_t priority);

//Funciones de Prueba de estado bloqueado. No seran parte del OS.
void test_block_task(void);
void test_unblock_task(task_t *task);


#endif /* INC_OS_CORE_H_ */
/********************** end of file ******************************************/
