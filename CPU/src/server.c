#include "../include/server.h"

void servidor_interrupt() {
    int socket_servidor = iniciar_servidor(logger, "CPU", ip_escucha, puerto_escucha_interrupt);
    //log_info(logger, "Servidor CPU INTERRUPT iniciado");

    /*pthread_t hilo_escucha;

    while(1) 
    {
        int conexion_kernel = esperar_cliente(socket_servidor);
        pthread_create(&hilo_escucha, NULL, (void*) atender_peticiones_interrupt, (void*) conexion_kernel);
        pthread_detach(hilo_escucha);
    }*/

    close(socket_servidor);
}

void servidor_dispatch() {
    int socket_servidor = iniciar_servidor(logger, "CPU", ip_escucha, puerto_escucha_interrupt);
    log_info(logger, "Servidor CPU DISPATCH iniciado");

    pthread_t hilo_escucha;

    while(1) 
    {
        int conexion_kernel = esperar_cliente(socket_servidor);
        pthread_create(&hilo_escucha, NULL, (void*) atender_peticiones_dispatch, (void*) conexion_kernel);
        pthread_detach(hilo_escucha);
    }

    close(socket_servidor);
}

void atender_peticiones_interrupt(void* conexion) {
    int conexion_kernel = (int) conexion;
    log_info(logger, "Cliente conectado modo INTERRUPT\n");
    int op_code = recibir_operacion(conexion_kernel);

    switch(op_code)
    {
        case INTERRUPCION:
            log_info(logger, "Petición recibida: INTERRUPCION");
            break;

        default: 
            log_error(logger, "El OP_CODE recibido es inválido");
            break;
    }


    close(conexion_kernel);

    log_info(logger, "El cliente se ha desconectado");
}


void atender_peticiones_dispatch(void* conexion) {
    int conexion_kernel = (int) conexion;
    log_info(logger, "Cliente conectado modo DISPATCH\n");
    int op_code = recibir_operacion(conexion_kernel);

    switch(op_code)
    {

        case RECIBIR_PCB:
            log_info(logger, "Petición recibida: RECIBIR_PCB");
            break;

        default: 
            log_error(logger, "El OP_CODE recibido es inválido");
            break;
    }
    
    close(conexion_kernel);

    log_info(logger, "El cliente se ha desconectado");
}