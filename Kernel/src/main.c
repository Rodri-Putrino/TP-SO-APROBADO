#include "../include/main.h"

#define CONFIG_FILE_PATH "./cfg/Kernel.config"
#define LOG_FILE_PATH "./cfg/Kernel.log"
#define NOMBRE_MODULO "KERNEL"

int main(void) {

    t_config* config_kernel;
	t_log* logger = iniciar_logger(LOG_FILE_PATH, NOMBRE_MODULO);

	config_kernel = iniciar_config(CONFIG_FILE_PATH);
	procesar_archivo_config_kernel(config_kernel);

    int socket_servidor = iniciar_servidor(logger, "KERNEL", "127.0.0.1", puerto_escucha);

    //Conexion con módulo Consola

    int conexion_consola = esperar_cliente(logger, "CONSOLA", socket_servidor);

    //Conexiones con módulo CPU

    int conexion_dispatch = crear_conexion(logger, "CPU", ip_cpu, puerto_cpu_dispatch);
    int conexion_interrupt = crear_conexion(logger, "CPU", ip_cpu, puerto_cpu_interrupt);

    //Conexion con módulo Memoria

    int conexion_memoria = crear_conexion(logger, "MEMORIA", ip_memoria, puerto_memoria);

    recibir_num(conexion_consola, logger);
    t_list *instrucciones = recibir_paquete(conexion_consola, logger);

    t_list_iterator *iterador = list_iterator_create(instrucciones);
    while(list_iterator_has_next(iterador))
    {
        char *i = list_iterator_next(iterador);
        printf("Instruccion %s\n", i);
    }


    list_iterator_destroy(iterador);
    list_destroy(instrucciones);

	close(socket_servidor);
    close(conexion_consola);
	close(conexion_dispatch);
	close(conexion_interrupt);
	close(conexion_memoria);

    finalizar_programa(logger, config_kernel);
}
