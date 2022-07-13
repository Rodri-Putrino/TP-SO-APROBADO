#include "../include/planificador_largo_plazo.h"

static pthread_t planificador_largo_plazo;

void iniciar_planificador_largo_plazo() {

    pthread_create(&planificador_largo_plazo, NULL, (void*) controlar_grado_de_multiprogramacion, NULL);
    pthread_detach(planificador_largo_plazo);
}

void controlar_grado_de_multiprogramacion() {

    log_debug(logger, "Planificador de largo plazo iniciado");

    while (1) {

        sem_wait(&sem_proceso_nuevo);
        log_debug(logger, "Plani LP notificado proceso nuevo");
        mostrar_grado_multiprogramacion_actual();
        sem_wait(&sem_multiprogramacion);

        log_info(logger, "Proceso admitido en el sistema");

        if (hay_proceso_suspendido_listo()) {
            t_pcb* pcb = desencolar_proceso_suspendido_listo();
            log_info(logger, "PCB ID %d desuspendido", pcb->id);
            encolar_proceso_en_listos(pcb);

            int conexion_memoria = crear_conexion(logger, "MEMORIA", ip_memoria, puerto_memoria);
            //t_paquete *p = crear_paquete(DESUSPENDER_PROCESO);
            
            //  TODO: SERIALIZAR PEDIDO, ahora rompe y son las 2:06 :(
            enviar_pedido_desuspender_proceso(conexion_memoria, pcb->id,logger);
            recibir_mensaje(conexion_memoria, logger);
            //agregar_a_paquete(p, (void*)pcb->id, sizeof(uint32_t));
            //enviar_paquete(p, conexion_memoria, logger);
            //eliminar_paquete(p);
        }
        else {
            t_pcb* pcb = desencolar_proceso_nuevo();
            encolar_proceso_en_listos(pcb);
            
            //int conexion_memoria = crear_conexion(logger, "Memoria", ip_memoria, puerto_memoria);
            //Enviar id y tam_proceso
            //Luego:
            //pcb_nuevo->tabla_paginas = recibir identificador de tabla de paginas
            //liberar_conexion(conexion_memoria);

        }
    }
}

void mostrar_grado_multiprogramacion_actual() {

    int cantidad_procesos_actual = cantidad_procesos_en_sistema();

    if(cantidad_procesos_actual == grado_multiprogramacion) {
        log_info(logger, "Grado de multiprogramación completo - Cantidad de procesos en sistema: %d", cantidad_procesos_actual);
    }
    else {
        log_info(logger, "Grado de multiprogramación actual: %d", cantidad_procesos_actual);
    }
}