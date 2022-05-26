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
            enviar_pcb(pcb,SUSPENDER_PROCESO,pcb,socket_cliente,logger);
            encolar_proceso_en_suspendidos_bloqueados(pcb);
        }
    }
}

int puede_suspenderse(t_pcb* pcb) {

    return (pcb->tiempo_bloqueado->fin - pcb->tiempo_bloqueado->inicio) > tiempo_max_bloqueado;
}