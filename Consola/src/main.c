/*
 ============================================================================
 Name        : MóduloA.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <main.h>

int main(int argc, char** argv) {

    if(argc < 3) {
        printf("Cantidad de argumentos insuficientes \n");
        return -1;
    }

	printf("Path del archivo: %d \n", argv[1]);
    printf("Tamaño del proceso: %s \n", argv[2]); 
    
    t_log* logger = log_create("./cfg/proceso1.log", "PROCESO1", true, LOG_LEVEL_INFO);
    log_info(logger, "Soy el proceso 1! %s", mi_funcion_compartida());
    log_destroy(logger);
}
