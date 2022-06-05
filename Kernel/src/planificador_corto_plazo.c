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

            ordenar_cola_listos();   
            t_pcb* pcb = desencolar_proceso_listo();
            encolar_proceso_en_ejecucion(pcb);
            enviar_pcb(RECIBIR_PCB, pcb, conexion_dispatch, logger);
            recibir_pcb_luego_de_ejecutar(conexion_dispatch);
            close(conexion_dispatch);
        }
        else {
            log_info(logger, "Algoritmo Plani: FIFO");

            t_pcb* pcb = desencolar_proceso_listo();
            encolar_proceso_en_ejecucion(pcb);
            log_info(logger, "Antes de enviar PCB a CPU");
            enviar_pcb(RECIBIR_PCB, pcb, conexion_dispatch, logger);
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
    t_pcb *pcb_en_ejecucion, *pcb_actualizado;

    switch(op_code)
    {
        case EXIT:
            log_info(logger, "Petición recibida: EXIT");
            pcb_en_ejecucion = desencolar_proceso_en_ejecucion();
            destruir_proceso(pcb_en_ejecucion); //Porque tenemos que empezar a usar el pcb actulizado
            pcb_actualizado = recibir_pcb(conexion, logger);
            //proceso_finalizar_rafaga(pcb);
            encolar_proceso_en_terminados(pcb_actualizado);
            sem_post(&sem_multiprogramacion);
            enviar_mensaje("El proceso ha finalizado su ejecucion", pcb_actualizado->id, logger);
            break;

        case IO:
            log_info(logger, "Petición recibida: IO"); 
            break;

        case ACTUALIZAR_PCB:
            log_info(logger, "Petición recibida: ACTUALIZAR_PCB"); 
            pcb_en_ejecucion = desencolar_proceso_en_ejecucion();
            destruir_proceso(pcb_en_ejecucion); //Porque tenemos que empezar a usar el pcb actulizado
            pcb_actualizado = recibir_pcb(conexion, logger);
            proceso_finalizar_rafaga(pcb_actualizado);
            encolar_proceso_en_listos(pcb_actualizado);

            break;
        
        default:
            log_error(logger, "El OP_CODE recibido es inválido");
            break;
    }
}