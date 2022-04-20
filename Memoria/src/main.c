#include "../include/main.h"

#define CONFIG_FILE_PATH "./cfg/Memoria.config"

int main(void) {

	t_config* config_memoria;

	t_log *logger = log_create("./cfg/Memoria.log", "MEMORIA", 1, LOG_LEVEL_INFO);

	config_memoria = iniciar_config(CONFIG_FILE_PATH);
	procesar_archivo_config_memoria(config_memoria);

	int socket_servidor = iniciar_servidor(logger, "MEMORIA", "127.0.0.1", "8001");

	int conexion_cpu = esperar_cliente(logger, "CPU", socket_servidor);
	int conexion_kernel = esperar_cliente(logger, "KERNEL", socket_servidor);


	close(conexion_kernel);
	close(conexion_cpu);
	close(socket_servidor);

	finalizar_programa(logger, config_memoria);

	return EXIT_SUCCESS;
}
