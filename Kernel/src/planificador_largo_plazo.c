#include "../include/planificador_largo_plazo.h"

static pthread_t planificador_largo_plazo;

void iniciar_planificador_largo_plazo() {

    pthread_create(&planificador_largo_plazo, NULL, (void*) controlar_grado_de_multiprogramacion, NULL);
    pthread_detach(planificador_largo_plazo);
}

void controlar_grado_de_multiprogramacion() {

    log_info(logger, "Planificador de largo plazo iniciado");

    while (1) {

        sem_wait(&sem_proceso_nuevo);
        log_debug(logger, "Plani LP notificado proceso nuevo");
        mostrar_grado_multiprogramacion_actual();
        sem_wait(&sem_multiprogramacion);

        if (hay_proceso_suspendido_listo()) {
            t_pcb* pcb = desencolar_proceso_suspendido_listo();
            encolar_proceso_en_listos(pcb);
        }
        else {
            t_pcb* pcb = desencolar_proceso_nuevo();
            log_info(logger, "ID: %d", pcb->id);
            log_info(logger, "Tamaño: %d", pcb->tam_proceso);
            encolar_proceso_en_listos(pcb);
            log_info(logger, "Proceso encolado en listos");
            //Enviar mensaje a Memoria para obtener valor de tabla de páginas.
        } 
    }
}

void mostrar_grado_multiprogramacion_actual() {

    int cantidad_procesos_actual = cantidad_procesos_en_sistema();

    if(cantidad_procesos_actual == grado_multiprogramacion) {
        log_info(logger, "Grado de multiprogramación completo, %d procesos en sistema", cantidad_procesos_actual);
    }
    else {
        log_info(logger, "Grado de multiprogramación actual: %d", cantidad_procesos_actual);
    }

}