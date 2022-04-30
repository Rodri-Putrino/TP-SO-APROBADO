#include "../include/main.h"

#define CONFIG_FILE_PATH "./cfg/Memoria.config"
#define LOG_FILE_PATH "./cfg/Memoria.log"
#define NOMBRE_MODULO "MEMORIA"

int main(void) {

	t_config* config_memoria;
	logger = iniciar_logger(LOG_FILE_PATH, NOMBRE_MODULO);

	config_memoria = iniciar_config(CONFIG_FILE_PATH);
	procesar_archivo_config_memoria(config_memoria);

	pthread_t hilo_servidor;
    pthread_create(&hilo_servidor, NULL, (void*) servidor, NULL);

	pthread_join(hilo_servidor, NULL);

	finalizar_programa(logger, config_memoria);
	
	return EXIT_SUCCESS;
}
