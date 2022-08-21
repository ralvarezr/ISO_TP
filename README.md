# **Sistema Operativo básico para el procesador ARM Cortex-M4 implementado sobre la placa NUCLEO-F429ZI.**

El Sistema Operativo tiene las siguientes caracterísitcas y funcionalidades:

- **Kernel** (_os_core.h_): Se encarga de administrar la memoria del sistema, el scheduling, cambio de contexto y manejo de errores. Es de tipo estático, toda la memoria asociada a las tareas y otros elementos se asigna el tiempo de compilación. El kernel consta de los siguientes elementos:
  - **Estructura de control**: Estructura interna que controla el estado del SO, último error ocurrido, puntero a la tarea en ejecución, puntero a la siguiente tarea a ser ejecutada, la cantidad de tareas bloqueadas, la lista de tareas, una bandera para indicar la necesidad de ejecutar un scheduling al salir de una interrupción y el estado anterior del SO a entrar a una interrupción.
  - **Estados del SO**: Los estados del SO son los siguientes:
    - ***OS_BOOTING***: Indica que el SO aún no ha sido inicializado.
    - ***OS_FRESH***: Indica que el SO ya fue inicializado pero aún no está ejecutando ninguna tarea.
    - ***OS_RUNNING***: Indica que el SO se encuentra en ejecución.
    - ***OS_IRQ***: Indica que el SO está atiendiendo alguna interrupción.
  - **Scheduler**: Define cuál es la siguiente tarea en ser ejecutada según una política de scheduling de tipo Round-Robin.
  - **Tareas**: Permiten al usuario crear y asociar funciones para que sean ejecutadas por el SO según su prioridad. ***El SO soporta un máximo de 8 tareas***. Las tareas deben tener un ciclo infito dentro de ellas, ya que las tareas de tipo One-Shot no están soportadas por el SO. Las tareas pueden tener los estados: ***TASK_READY***, ***TASK_RUNNING*** y ***TASK_BLOCKED***. Las tareas tienen la siguiente API:
    - **_void os_task_init(task_t *task,  void *entry_point, uint8_t priority)_**
  - **Prioridades**: Definen cuáles son las tareas que deben ser ejecutadas primero. Las prioridades van del ***0*** al ***3***, siendo ***0*** la **mayor** prioridad de todas, y ***3*** la **menor** prioridad.
  - **Errores**: El SO contempla los siguientes escenarios de error:
    - ***OS_ERROR_GENERAL***: Es el estado de error por defecto. Indica que no ha ocurrido ningún error durante la ejecución.
    - ***OS_ERROR_TASK_COUNT***: Este error ocurre cuando se intentan inicializar más tareas de las permitidas por el SO (***ocho***).
    - ***OS_ERROR_TASK_PRIORITY***: Este error ocurre cuando se intenta inicializar una tarea con una prioridad fuera del rango permitido.
    - ***OS_ERROR_IRQ***: Este error ocurre cuando se realiza una acción no permitida dentro del handler de una Interrupción. Estas acciones son: llamar a un delay, intentar tomar un semáforo ya tomado, enviar un dato a una cola llena, o intentar recibir un dato de una cola vacía.
    - ***OS_ERROR_EXIT_IRQ***: Este ocurre cuando se intenta salir del estado ***OS_IRQ*** sin estar en ese estado.
  - **Hooks del Sistema**: Son funciones que se ejecutan en determinado momento y que pueden ser redefinidas por el usuario ya que son de tipo weak. Estas son:
    - ***void idle_task(void)***: Es la tarea que el SO ejecuta cuando no tiene otra tarea para ejecutar.
    - ***void tick_hook(void)***: Se ejecuta en cada tick del sistema (cada 1 milisegundo).
    - ***void return_hook(void)***: Se ejecuta cuando una tarea termina su ejecución.
    - ***void error_hook(void)***: Se ejecuta cuando el SO entra en algún estado de error.

- **Delays** (_os_core.h_): Permite al usuario bloquear la tarea durante el tiempo especificado. El tiempo está en milisegundos, por lo que un tiempo de 1000 equivale a 1 segundo. La API de los delays es:
  - ***void task_delay(uint32_t time)***

- **Semáforos** (_os_core.h_): Permiten al usuario tener control sobre recursos compartidos que se acceden de forma concurrente. Los semáforos se inicializan en estado ***Taken***, por lo que se debe hacer un ***give*** del semáforo antes de usarlo. Se puede usar un semáforo para múltiples tareas, por lo que cualquier tarea que intente tomar un semáforo previamente tomado, se bloqueará. El hacer ***give*** de un semáforo desbloquerá todas las tareas que hayan sido bloqueadas por él. Los semáforos son de tipo estático. La API de los semáforos es la siguiente:
  -  **_void semaphore_init(semaphore_t *semaphore)_**
  -  **_void semaphore_take(semaphore_t *semaphore)_**
  -  **_void semaphore_give(semaphore_t *semaphore)_**

- **Colas** (_os_core.h_): Permiten al usuario compartir datos entre tareas. El tipo de dato soportado por las colas es ***uint32_t***. Son de tipo dinámico, por lo que se debe hacer un ***delete*** de una cola para liberar los recursos tomados por ella. Si una tarea intenta enviar un dato a una cola que se encuentra llena, se bloqueará hasta que haya espacio disponible. Del mismo modo, si una tarea intenta recibir un dato de una cola vacía, se bloqueará hasta que haya un dato en la cola para ser recibido. La API de las colas son:
  - **_bool queue_create(queue_t *queue, uint8_t lenght)_**
  - **_void queue_receive(queue_t *queue, uint32_t *buffer)_**
  - **_void queue_send(queue_t *queue, uint32_t element)_**
  - **_void queue_delete(queue_t *queue)_**

- **Interrupciones** (_os_irq.h_): Permite al usuario instalar y definir el handler de ejecución de las interrupciones, así como también removerlas. Actualmente el SO sólo soporta las siguientes interrupciones: ***USART3_IRQn*** y ***EXTI15_10_IRQn***. La API de las interrupciones es la siguiente:
  - **_bool os_irq_install(IRQn_Type irq, void *isr)_**
  - ***bool os_irq_remove(IRQn_Type irq)***

- **Otras APIs** (_os_core.h_):
  - ***void os_cpu_yield(void)***. Fuerza un scheduling.
  - ***void os_enter_critical(void)***. Indica el inicio de una sección crítica. Desahbilita las interrupciones. 
  - ***void os_exit_critical(void)***. Indica el fin de una sección crítica. Habilita las interrupciones.
