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

#define CONFIG_FILE_PATH "./cfg/CPU.config"

int main(void) {

	t_config* config_cpu;
	t_log *logger = log_create("./cfg/CPU.log", "CPU", 1, LOG_LEVEL_INFO);

	config_cpu = iniciar_config(CONFIG_FILE_PATH);
	procesar_archivo_config_cpu(config_cpu);

	printf("IP_MEMORIA: %s\n", ip_memoria);

	/*int socket_servidor = iniciar_servidor(logger, "CPU", "127.0.0.1", "8001");

	int conexion_kernel = esperar_cliente(logger, "KERNEL", socket_servidor);
	int conexion_memoria = crear_conexion(logger, "MEMORIA", "127.0.0.1","8002");

	close(conexion_kernel);
	close(conexion_memoria);
	close(socket_servidor);*/

	finalizar_programa(logger, config_cpu);

	return 0;
}
