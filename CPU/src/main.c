#include "../include/main.h"

#define CONFIG_FILE_PATH "./cfg/CPU.config"
#define LOG_FILE_PATH "./cfg/CPU.log"
#define NOMBRE_MODULO "CPU"

int main(void) {

	t_config* config_cpu;
	t_log* logger = iniciar_logger(LOG_FILE_PATH, NOMBRE_MODULO);

	config_cpu = iniciar_config(CONFIG_FILE_PATH);
	procesar_archivo_config_cpu(config_cpu);

	//printf("IP_MEMORIA: %s\n", ip_memoria);

	pthread_t hilo_servidor_interrupt;
	pthread_t hilo_servidor_dispatch;
    pthread_create(&hilo_servidor_interrupt, NULL, (void*) servidor_interrupt, NULL);
	pthread_create(&hilo_servidor_dispatch, NULL, (void*) servidor_dispatch, NULL);
	pthread_join(hilo_servidor_interrupt, NULL);
	pthread_join(hilo_servidor_dispatch, NULL);


/*
	int socket_servidor = iniciar_servidor(logger, "CPU", "127.0.0.1", "8001");

	int conexion_kernel = esperar_cliente(logger, "KERNEL", socket_servidor);
	int conexion_memoria = crear_conexion(logger, "MEMORIA", "127.0.0.1","8002");

	close(conexion_kernel);
	close(conexion_memoria);
	close(socket_servidor);
*/
	finalizar_programa(logger, config_cpu);

	return 0;
}
