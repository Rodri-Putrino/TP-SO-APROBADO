#include "../include/planificador_corto_plazo.h"

static pthread_t planificador_corto_plazo;

void iniciar_planificador_corto_plazo() {

    pthread_create(&planificador_corto_plazo, NULL, (void*) planificar_procesos, NULL);
    pthread_detach(planificador_corto_plazo);
}

void planificar_procesos() {

    log_info(logger, "Planificador de corto plazo iniciado");

    /*while (1) {

        sem_wait(&sem_proceso_listo);
    
    }*/
}