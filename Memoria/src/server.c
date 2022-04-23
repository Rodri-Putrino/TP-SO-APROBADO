#include "../include/server.h"

void servidor() {
    int socket_servidor = iniciar_servidor(logger, "Memoria", ip_escucha, puerto_escucha);
    log_info(logger, "Servidor Memoria iniciado");

    pthread_t hilo_escucha_cpu;
    pthread_t hilo_escucha_kernel;

    while(1) 
    {
        int conexion_cpu = esperar_cliente(logger, "CPU", socket_servidor);
	    int conexion_kernel = esperar_cliente(logger, "KERNEL", socket_servidor);
        pthread_create(&hilo_escucha_cpu, NULL, (void*) atender_peticiones_cpu, (void*) conexion_cpu);
        pthread_create(&hilo_escucha_kernel, NULL, (void*) atender_peticiones_kernel, (void*) conexion_kernel);
        pthread_detach(hilo_escucha_cpu);
        pthread_detach(hilo_escucha_kernel);
    }

    close(socket_servidor);
}

void atender_peticiones_cpu(void* conexion) {
    int conexion_cpu = (int) conexion;

    int op_code = recibir_operacion(conexion_cpu);

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

        default: 
            log_error(logger, "El OP_CODE recibido es inválido");
            break;
    }

    close(conexion_cpu);

    log_info(logger, "El cliente se ha desconectado");
}

void atender_peticiones_kernel(void* conexion) {
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
}