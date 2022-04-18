/*
 ============================================================================
 Name        : MóduloB.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "../include/main.h"

int main(void) {
	t_log *logger = log_create("../cfg/CPU.log", "CPU", 1, LOG_LEVEL_INFO);

	int socket_servidor = iniciar_servidor(logger, "CPU", "127.0.0.1", "8001");

	int conexion_kernel = esperar_cliente(logger, "KERNEL", socket_servidor);
	int conexion_memoria = crear_conexion(logger, "MEMORIA", "127.0.0.1","8002");

	close(conexion_kernel);
	close(conexion_memoria);
	close(socket_servidor);

	return 0;
}
