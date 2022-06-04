#include "../include/planificador_mediano_plazo.h"

static pthread_t planificador_mediano_plazo;

void iniciar_planificador_mediano_plazo() {

    pthread_create(&planificador_mediano_plazo, NULL, (void*) suspender_procesos, NULL);
    pthread_detach(planificador_mediano_plazo);
}

void suspender_procesos() {

    while (1) {

        sem_wait(&sem_proceso_a_suspender);
        
        int conexion_memoria = crear_conexion(logger, "Memoria", ip_memoria, puerto_memoria);

        t_pcb* pcb = desencolar_proceso_bloqueado();
        if(puede_suspenderse(pcb)) {
            enviar_pcb(SUSPENDER_PROCESO, pcb, conexion_memoria, logger);
            encolar_proceso_en_suspendidos_bloqueados(pcb);
        }
    }
}