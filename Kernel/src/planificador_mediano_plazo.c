#include "../include/planificador_mediano_plazo.h"

static pthread_t planificador_mediano_plazo;

void iniciar_planificador_mediano_plazo() {

    pthread_create(&planificador_mediano_plazo, NULL, (void*) suspender_procesos, NULL);
    pthread_detach(planificador_mediano_plazo);
}

void suspender_procesos() {

    /*while (1) {

        sem_wait(&sem_proceso_suspendido_listo);
        
        
        sem_wait(&sem_suspender_proceso);

        mover_proceso_bloqueado_a_suspendido(); // enviar id a mem

        sem_post(&sem_proceso_suspendido);

    }*/
}