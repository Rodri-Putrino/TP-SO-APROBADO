/*
 ============================================================================
 Name        : MóduloC.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "../include/main.h"

int main(void) {

	t_log *logger = log_create("../cfg/Memoria.log", "MEMORIA", 1, LOG_LEVEL_INFO);

	int socket_servidor = iniciar_servidor(logger, "MEMORIA", "127.0.0.1", "8001");

	int conexion_cpu = esperar_cliente(logger, "CPU", socket_servidor);
	int conexion_kernel = esperar_cliente(logger, "KERNEL", socket_servidor);

	close(conexion_kernel);
	close(conexion_cpu);
	close(socket_servidor);


	return EXIT_SUCCESS;
}
