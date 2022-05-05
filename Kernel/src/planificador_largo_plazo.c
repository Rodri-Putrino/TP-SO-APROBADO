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

        if (!grado_multiprogramacion_completo()) {
          
            log_info(logger, "Hay espacio para admitir al proceso");
            t_pcb* pcb = desencolar_proceso_nuevo();
            encolar_proceso_en_listos(pcb);
            //Enviar mensaje a Memoria para obtener valor de tabla de páginas. 
        } 
    }
}

bool grado_multiprogramacion_completo() {

    bool resultado;

    resultado = cantidad_procesos_en_sistema() >= grado_multiprogramacion;
    if(resultado == true)
    {
        log_info(logger, "Grado de multiprogramación completo");
    }

    return resultado;
}