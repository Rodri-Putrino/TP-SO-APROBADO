#include "../include/main.h"

int main(void) {
	t_log* logger = log_create("./cfg/proceso1.log", "PROCESO1", true, LOG_LEVEL_INFO);

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

    log_destroy(logger);

    close(conexion_consola);
	close(conexion_cpu);
	close(socket_servidor);
}
