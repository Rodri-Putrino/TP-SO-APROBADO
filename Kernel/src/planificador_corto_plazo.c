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
                recibir_pcb_luego_de_ejecutar(conexion_dispatch);
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

            t_pcb* pcb = desencolar_proceso_listo();
            encolar_proceso_en_ejecucion(pcb);
            
            log_info(logger, "Antes de enviar PCB a CPU");
            t_pcb* un_pcb = malloc(sizeof(t_pcb));

            un_pcb->tam_proceso = 27;
            un_pcb->id = 46;
            un_pcb->program_counter = 0;
            un_pcb->estimacion_anterior = estimacion_inicial;
            un_pcb->ultima_rafaga = 0;

            enviar_pcb_test(RECIBIR_PCB, un_pcb, conexion_dispatch, logger);
            /*
            t_paquete* paquete = crear_paquete(RECIBIR_PCB);
            agregar_a_paquete(paquete, pcb, sizeof(t_pcb));
            enviar_paquete(paquete, conexion_dispatch, logger);
            eliminar_paquete(paquete);
            */

            recibir_pcb_luego_de_ejecutar(conexion_dispatch);

            close(conexion_dispatch);
        } 
    }
}

bool algoritmo_es_srt() {
    return 0 == strcmp(algoritmo_planificacion, "SRT");
}

void recibir_pcb_luego_de_ejecutar(int conexion) {

    int op_code = recibir_operacion(conexion);
    //TODO crear funcion => t_pcb* pcb = recibir_pcb(conexion);

    switch(op_code)
    {
        case EXIT:
            log_info(logger, "Petición recibida: EXIT");
            t_pcb* pcb = desencolar_proceso_en_ejecucion();
            encolar_proceso_en_terminados(pcb);
            sem_post(&sem_multiprogramacion);
            enviar_mensaje("El proceso ha finalizado su ejecucion", pcb->id, logger);
            break;

        case IO:
            log_info(logger, "Petición recibida: IO"); 
            break;

        case ACTUALIZAR_PCB:
            log_info(logger, "Petición recibida: ACTUALIZAR_PCB"); 
            break;
        
        default:
            log_error(logger, "El OP_CODE recibido es inválido");
            break;
    }
}