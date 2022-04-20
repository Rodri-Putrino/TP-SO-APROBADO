#include "../include/main.h"

#define CONFIG_FILE_PATH "./cfg/Kernel.config"

int main(void) {

    t_config* config_kernel;
	t_log* logger = log_create("./cfg/Kernel.log", "KERNEL", true, LOG_LEVEL_INFO);

	config_kernel = iniciar_config(CONFIG_FILE_PATH);
	procesar_archivo_config_kernel(config_kernel);

    int socket_servidor = iniciar_servidor(logger, "KERNEL", "127.0.0.1", "8000");
    int conexion_consola = esperar_cliente(logger, "CONSOLA", socket_servidor);

    int conexion_cpu = crear_conexion(logger, "CPU", "127.0.0.1", "8003");

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

    close(conexion_consola);
	close(conexion_cpu);
	close(socket_servidor);

    finalizar_programa(logger, config_kernel);
}
