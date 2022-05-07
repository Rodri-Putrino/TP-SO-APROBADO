#include "../include/planificador_corto_plazo.h"

static pthread_t planificador_corto_plazo;

void iniciar_planificador_corto_plazo() {

    pthread_create(&planificador_corto_plazo, NULL, (void*) planificar_procesos, NULL);
    pthread_detach(planificador_corto_plazo);
}

void planificar_procesos() {

    log_info(logger, "Planificador de corto plazo iniciado");

    while (1) {

        sem_wait(&sem_proceso_listo);
        log_debug(logger, "Plani CP notificado proceso listo");

        int conexion_dispatch = crear_conexion(logger, "CPU", ip_cpu, puerto_cpu_dispatch);

        if(algoritmo_es_srt()) {
            log_info(logger, "Algoritmo Plani: SRT");
            
            if(hay_proceso_en_ejecucion()) {
                int conexion_interrupt = crear_conexion(logger, "CPU", ip_cpu, puerto_cpu_interrupt);
                //Enviar interrupción a CPU a través de Interrupt
                //Recibir pcb en ejecución a través de Dispatch
                close(conexion_interrupt);
            }

            /* -------------- Pasos a seguir -------------- */
            /*
            ordenar_cola_listos();   
            proceso = desencolar_proceso_listo();
            encolar_proceso_en_ejecucion(proceso);
            Enviar pcb a CPU a través de Dispatch
            */
            /* -------------------------------------------- */
            close(conexion_dispatch);

        }
        else {
            log_info(logger, "Algoritmo Plani: FIFO");

            t_pcb* pcb_listo = desencolar_proceso_listo();
            encolar_proceso_en_ejecucion(pcb_listo);
            t_paquete* paquete = crear_paquete(RECIBIR_PCB);
            agregar_a_paquete(paquete, pcb_listo, sizeof(t_pcb));
            enviar_paquete(paquete, conexion_dispatch, logger);
            eliminar_paquete(paquete);

            close(conexion_dispatch);
        } 
    }
}

bool algoritmo_es_srt() {
    return 0 == strcmp(algoritmo_planificacion, "SRT");
}