#include "../include/main.h"

#define CONFIG_FILE_PATH "./cfg/Kernel.config"
#define LOG_FILE_PATH "./cfg/Kernel.log"
#define NOMBRE_MODULO "KERNEL"

int main(void) {

    t_config* config_kernel;
	logger = iniciar_logger(LOG_FILE_PATH, NOMBRE_MODULO);

	config_kernel = iniciar_config(CONFIG_FILE_PATH);
	procesar_archivo_config_kernel(config_kernel);

    iniciar_estructuras_de_estados_de_procesos();
    iniciar_planificador_largo_plazo();

    pthread_t hilo_escucha;
    pthread_create(&hilo_escucha, NULL, (void*) escuchar_procesos_nuevos, NULL);

    /* ----- Caso de prueba de conexiones ----
    
    Conexiones con módulo CPU

    int conexion_dispatch = crear_conexion(logger, "CPU", ip_cpu, puerto_cpu_dispatch);
    int conexion_interrupt = crear_conexion(logger, "CPU", ip_cpu, puerto_cpu_interrupt);

    Conexion con módulo Memoria

    int conexion_memoria = crear_conexion(logger, "Memoria", ip_memoria, puerto_memoria);
    
    liberar_conexion(conexion_dispatch);
    liberar_conexion(conexion_interrupt);
    liberar_conexion(conexion_memoria);
    */

    pthread_join(hilo_escucha, NULL);

    finalizar_programa(logger, config_kernel);
}
