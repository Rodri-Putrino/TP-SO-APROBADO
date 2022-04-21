#include "../include/main.h"

#define CONFIG_FILE_PATH "./cfg/Consola.config"
#define LOG_FILE_PATH "./cfg/Consola.log"
#define NOMBRE_MODULO "Consola"

int main(int argc, char** argv) {

    t_config* config_consola;
    t_log* logger = iniciar_logger(LOG_FILE_PATH, NOMBRE_MODULO);

    if(argc < 3) {
        printf("Cantidad de argumentos insuficientes \n");
        return EXIT_FAILURE;
    }

    log_info(logger, "Módulo Consola iniciado");
	printf("Path del archivo: %s \n", argv[1]);
    printf("Tamaño del proceso: %s \n", argv[2]); 

    config_consola = iniciar_config(CONFIG_FILE_PATH);
    procesar_archivo_config_consola(config_consola);

    int conexion_kernel = crear_conexion(logger, "KERNEL", ip_kernel, puerto_kernel);

    t_paquete* codigo = leer_archivo(argv[1]);
    agregar_a_paquete(codigo, (void*)(argv[2]), sizeof(int) + 1);

    enviar_paquete(codigo, conexion_kernel, logger);

    eliminar_paquete(codigo);

    close(conexion_kernel);

    /*--------------------------------------*/

    finalizar_programa(logger, config_consola);
}
