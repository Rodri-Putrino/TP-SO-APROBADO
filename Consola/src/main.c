/*
 ============================================================================
 Name        : MóduloA.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "../include/main.h"

#define IP "127.0.0.1"
#define PUERTO "8000"

int main(int argc, char** argv) {

    if(argc < 3) {
        printf("Cantidad de argumentos insuficientes \n");
        return -1;
    }

	printf("Path del archivo: %d \n", argv[1]);
    printf("Tamaño del proceso: %s \n", argv[2]); 

    t_log* logger = log_create("./cfg/proceso1.log", "PROCESO1", true, LOG_LEVEL_INFO);
    log_info(logger, "Soy el proceso 1! %s", mi_funcion_compartida());

    /*--------------------------------------*/

    int conexion_kernel = crear_conexion(logger, "KERNEL", IP, PUERTO);

    t_paquete *codigo = crear_paquete();
    codigo->codigo_operacion = MENSAJE;

    codigo = leer_archivo(argv[1]);
    agregar_a_paquete(codigo, (void*)(argv[2]), sizeof(int));

    enviar_paquete(codigo, conexion_kernel, logger);

    recibir_num(conexion_kernel, logger);

    close(conexion_kernel);

    /*--------------------------------------*/

    log_destroy(logger);
}
