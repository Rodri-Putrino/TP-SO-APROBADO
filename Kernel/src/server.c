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

    int op_code = recibir_operacion(conexion_consola);

    switch(op_code)
    {
        case NUEVO_PROCESO: 

            log_info(logger, "Petición recibida: NUEVO_PROCESO");
            t_list *instrucciones = recibir_paquete(conexion_consola, logger);

            t_list_iterator *iterador = list_iterator_create(instrucciones);
            while(list_iterator_has_next(iterador))
            {
                char *i = list_iterator_next(iterador);
                printf("Instruccion %s\n", i);
            }

            list_iterator_destroy(iterador);
            list_destroy_and_destroy_elements(instrucciones, free);
            
            enviar_mensaje("Proceso registrado correctamente", conexion_consola, logger);

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