#include "../include/server.h"

void escuchar_procesos_nuevos() {
    int socket_servidor = iniciar_servidor(logger, "KERNEL", ip_escucha, puerto_escucha);
    log_info(logger, "Servidor Kernel iniciado");

    pthread_t atender_consola;

    while(1) 
    {
        int conexion_consola = esperar_cliente(logger, "CONSOLA", socket_servidor);
        pthread_create(&atender_consola, NULL, (void*) atender_procesos_nuevos, (void*) conexion_consola);
        pthread_detach(atender_consola);
    }

    close(socket_servidor);
}

void atender_procesos_nuevos(void* conexion) {
    int conexion_consola = (int) conexion;

    //recibir_num(conexion_consola, logger);
    int op_code = recibir_operacion(conexion_consola);

    switch(op_code)
    {
        case NUEVO_PROCESO: 

            log_info(logger, "Nuevo proceso recibido");
            t_list *instrucciones = recibir_paquete(conexion_consola, logger);

            t_list_iterator *iterador = list_iterator_create(instrucciones);
            while(list_iterator_has_next(iterador))
            {
                char *i = list_iterator_next(iterador);
                printf("Instruccion %s\n", i);
            }

            list_iterator_destroy(iterador);
            list_destroy(instrucciones);

            break;

        case EXIT:

            break;

        case IO:

            break;

        case ACTUALIZAR_PCB:

            break;

        default: 
            log_error(logger, "El OP_CODE recibido es inválido");
            break;
    }

    close(conexion_consola);

    log_info(logger, "El cliente se ha desconectado");
}