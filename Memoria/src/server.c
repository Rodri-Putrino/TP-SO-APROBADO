#include "../include/server.h"

void servidor() {
    int socket_servidor = iniciar_servidor(logger, "Memoria", ip_escucha, puerto_escucha);
    log_info(logger, "Servidor Memoria iniciado");

    pthread_t hilo_escucha;

    while(1) 
    {
        int conexion_general = esperar_cliente(socket_servidor);
        pthread_create(&hilo_escucha, NULL, (void*) atender_peticiones, (void*) conexion_general);
        pthread_detach(hilo_escucha);
    }

    close(socket_servidor);
}

void atender_peticiones(void* conexion) {
    int conexion_general = (int) conexion;
    log_info(logger, "Cliente conectado\n");
    int op_code = recibir_operacion(conexion_general);

    switch(op_code)
    {
        case ENVIAR_HANDSHAKE:
            log_info(logger, "Petición recibida: ENVIAR_HANDSHAKE"); 

            break;

        case PEDIDO_LECTURA:
            log_info(logger, "Petición recibida: PEDIDO_LECTURA"); 

            break;

        case PEDIDO_ESCRITURA:
            log_info(logger, "Petición recibida: PEDIDO_ESCRITURA"); 

            break;

        case PEDIDO_COPIA:
            log_info(logger, "Petición recibida: PEDIDO_COPIA"); 

            break;

        case SOLICITUD_TABLA_PAGINAS:
            log_info(logger, "Petición recibida: SOLICITUD_TABLA_PAGINAS"); 

            break;

        case SOLICITUD_MARCO:
            log_info(logger, "Petición recibida: SOLICITUD_MARCO"); 

            break;

        case SOLICITUD_DIRECCION_FISICA:
            log_info(logger, "Petición recibida: SOLICITUD_DIRECCION_FISICA"); 

            break;

        case INICIALIZAR_ESTRUCTURAS: 

            break;

        case SUSPENDER_PROCESO:

            break;

        default: 
            log_error(logger, "El OP_CODE recibido es inválido");
            break;
    }

    close(conexion_general);

    log_info(logger, "El cliente se ha desconectado");
}

/*void atender_peticiones_kernel(void* conexion) {
    int conexion_kernel = (int) conexion;

    int op_code = recibir_operacion(conexion_kernel);

    switch(op_code)
    {
        case INICIALIZAR_ESTRUCTURAS: 

            break;

        case SUSPENDER_PROCESO:

            break;

        default: 
            log_error(logger, "El OP_CODE recibido es inválido");
            break;
    }

    close(conexion_kernel);

    log_info(logger, "El cliente se ha desconectado");
}*/