#include "../include/server.h"

static char* codigos_instrucciones[6] = {
    "NO_OP",
    "I/O",
    "READ",
    "WRITE",
    "COPY",
    "EXIT"
};

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
    
    log_info(logger, "Cliente conectado\n");
    int conexion_consola = (int) conexion;
    int op_code = recibir_operacion(conexion_consola);

    switch(op_code)
    {
        case NUEVO_PROCESO:

            log_info(logger, "Petición recibida: NUEVO_PROCESO");

            t_list *instrucciones = recibir_paquete(conexion_consola, logger);

            //TOMAR TAMANIO PROCESO (ultimo elemento de lista)
            char *tamanio_proceso = (char*) list_get(instrucciones, list_size(instrucciones) -1);
            printf("Tamanio proceso: %d\n", atoi(tamanio_proceso));
            list_remove_and_destroy_element(instrucciones, list_size(instrucciones) -1, free);

            t_list_iterator *iterador = list_iterator_create(instrucciones);
            while(list_iterator_has_next(iterador))
            {
                //VERSION ANTERIOR
                //char *i = list_iterator_next(iterador);
                //printf("Instruccion %s\n", i);

                t_instruccion *i = list_iterator_next(iterador);
                printf("Instruccion %s \n\t Argumento 1: %d\n\t Argumento 2: %d\n", codigos_instrucciones[i->op], i->arg[0], i->arg[1]);
            }

            t_pcb* pcb_nuevo = crear_proceso(conexion_consola, atoi(tamanio_proceso), instrucciones);
            encolar_proceso_en_nuevos(pcb_nuevo);

            list_destroy_and_destroy_elements(instrucciones, free);
            list_iterator_destroy(iterador);

            break;

        default: 
            log_error(logger, "El OP_CODE recibido es inválido");
            break;
    }

    //close(conexion_consola); La conexión se debe cerrar cuando se elimina el proceso,
    //                         dado que se almacena en el PCB.
    //log_info(logger, "El cliente se ha desconectado");
}