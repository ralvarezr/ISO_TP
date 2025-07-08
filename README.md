# Basic Operating System for ARM Cortex-M4 (NUCLEO-F429ZI Board)

This project implements a simple Real-Time Operating System (RTOS) for the ARM Cortex-M4 processor, running on the STM32 NUCLEO-F429ZI development board. All system components are written in C, and designed for static memory allocation and deterministic behavior.

##  Features Overview

### Kernel (`os_core.h`)
The kernel is responsible for system memory management, task scheduling, context switching, and error handling. It uses **static memory allocation**, meaning all task and system resources are allocated at compile time.

Main components include:

- **Control Structure**: Holds the OS state, last error, pointer to the current and next task, number of blocked tasks, task list, an interrupt scheduling flag, and the pre-interrupt OS state.

- **OS States**:
  - `OS_BOOTING`: OS is initializing.
  - `OS_FRESH`: Initialized but no task is running yet.
  - `OS_RUNNING`: The OS is running.
  - `OS_IRQ`: The OS is handling an interrupt.

- **Scheduler**: Implements a Round-Robin scheduling policy to select the next task to run.

- **Tasks**: The OS supports **up to 8 concurrent tasks**, each defined with an infinite loop (one-shot tasks are not supported). Task states include `TASK_READY`, `TASK_RUNNING`, and `TASK_BLOCKED`.

  **Task API:**
  ```c
  void os_task_init(task_t *task, void *entry_point, uint8_t priority);
  ```

- **Priorities**: Four levels, from `0` (highest) to `3` (lowest).

- **Error Codes**:
  - `OS_ERROR_GENERAL`: Default no-error state.
  - `OS_ERROR_TASK_COUNT`: Exceeded the maximum number of tasks.
  - `OS_ERROR_TASK_PRIORITY`: Invalid task priority.
  - `OS_ERROR_IRQ`: Invalid operation inside an interrupt context.
  - `OS_ERROR_EXIT_IRQ`: Attempted to exit IRQ state while not inside it.

- **System Hooks** (all are `weak` and user-overridable):
  - `void idle_task(void);` — Executed when no tasks are ready.
  - `void tick_hook(void);` — Called every system tick (1ms).
  - `void return_hook(void);` — Called when a task ends.
  - `void error_hook(void);` — Called when the OS enters an error state.

###  Delays (`os_core.h`)
Allows a task to block itself for a specified time in milliseconds.

**Delay API:**
```c
void task_delay(uint32_t time);
```

### Semaphores (`os_core.h`)
Used to manage concurrent access to shared resources. Semaphores are statically allocated and initialized as **taken** by default. A task trying to take a taken semaphore will block until it becomes available.

**Semaphore API:**
```c
void semaphore_init(semaphore_t *semaphore);
void semaphore_take(semaphore_t *semaphore);
void semaphore_give(semaphore_t *semaphore);
```

### Queues (`os_core.h`)
Queues support message passing between tasks using `uint32_t` data. They are dynamically allocated and must be deleted to free memory. Tasks will block when sending to a full queue or receiving from an empty one.

**Queue API:**
```c
bool queue_create(queue_t *queue, uint8_t length);
void queue_receive(queue_t *queue, uint32_t *buffer);
void queue_send(queue_t *queue, uint32_t element);
void queue_delete(queue_t *queue);
```

### Interrupts (`os_irq.h`)
Supports ISR (Interrupt Service Routine) registration and removal. Currently supports `USART3_IRQn` and `EXTI15_10_IRQn`.

**Interrupt API:**
```c
bool os_irq_install(IRQn_Type irq, void *isr);
bool os_irq_remove(IRQn_Type irq);
```

### Additional Utilities (`os_core.h`)
- `void os_cpu_yield(void);` — Forces a scheduling decision.
- `void os_enter_critical(void);` — Disables interrupts (start of critical section).
- `void os_exit_critical(void);` — Enables interrupts (end of critical section).

## Notes
- The system is built with simplicity and clarity in mind, and is intended for educational purposes.
- All APIs are designed to run on bare-metal ARM Cortex-M4, leveraging CMSIS and STM32 HAL for low-level hardware control.