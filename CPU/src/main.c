#include "../include/main.h"

#define CONFIG_FILE_PATH "./cfg/CPU.config"
#define LOG_FILE_PATH "./cfg/CPU.log"
#define NOMBRE_MODULO "CPU"

int main(void) {

	t_config* config_cpu;
	logger_CPU = iniciar_logger(LOG_FILE_PATH, NOMBRE_MODULO);

	config_cpu = iniciar_config(CONFIG_FILE_PATH);
	procesar_archivo_config_cpu(config_cpu);

	pthread_t hilo_servidor_interrupt;
    pthread_create(&hilo_servidor_interrupt, NULL, (void*) servidor_interrupt, NULL);

	pthread_t hilo_servidor_dispatch;
    pthread_create(&hilo_servidor_dispatch, NULL, (void*) servidor_dispatch, NULL);

	pthread_join(hilo_servidor_interrupt, NULL);
	pthread_join(hilo_servidor_dispatch, NULL);
	finalizar_programa(logger_CPU, config_cpu);

	return 0;
}
