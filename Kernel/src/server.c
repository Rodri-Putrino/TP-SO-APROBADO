#include "../include/server.h"

void escuchar_procesos_nuevos() {
    int socket_servidor = iniciar_servidor(logger, "KERNEL", ip_escucha, puerto_escucha);
    log_info(logger, "Servidor Kernel iniciado");

    pthread_t atender_consola;

    while(1) 
    {
        int conexion_consola = esperar_cliente(socket_servidor);
        pthread_create(&atender_consola, NULL, (void*) atender_procesos_nuevos, (void*) conexion_consola);
        pthread_detach(atender_consola);
    }

    close(socket_servidor);
}

void atender_procesos_nuevos(void* conexion) {
    int conexion_consola = (int) conexion;
    log_info(logger, "Cliente conectado \n");
    int op_code = recibir_operacion(conexion_consola);

    switch(op_code)
    {
        case NUEVO_PROCESO:

            log_info(logger, "Petición recibida: NUEVO_PROCESO");

            t_list *instrucciones = recibir_paquete(conexion_consola, logger);

            //TOMAR TAMANIO PROCESO (ultimo elemento de lista)
            char *tamanio_proceso = list_get(instrucciones, list_size(instrucciones) -1);
            printf("Tamanio proceso: %d\n", atoi(tamanio_proceso));
            list_remove_and_destroy_element(instrucciones, list_size(instrucciones) -1, free);

            t_list_iterator *iterador = list_iterator_create(instrucciones);
            while(list_iterator_has_next(iterador))
            {
                //VERSION ANTERIOR
                //char *i = list_iterator_next(iterador);
                //printf("Instruccion %s\n", i);

                t_instruccion *i = list_iterator_next(iterador);
                printf("Instruccion %d\nargumento 1: %d\nargumento 2: %d\n", i->op, i->arg[0], i->arg[1]);
            }

            t_pcb* pcb_nuevo = crear_proceso(conexion_consola, 20, instrucciones);

            printf("Tam proceso: %d\n", pcb_nuevo->tam_proceso);
            printf("Primero de la lista del proceso %s\n", list_get(pcb_nuevo->instrucciones,0));
            
            enviar_mensaje("El proceso ha finalizado su ejecucion", pcb_nuevo->id, logger);

            list_iterator_destroy(iterador);
            list_destroy_and_destroy_elements(instrucciones, free);

            break;

        case EXIT:
            log_info(logger, "Petición recibida: EXIT"); 
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

    close(conexion_consola);

    log_info(logger, "El cliente se ha desconectado");
}