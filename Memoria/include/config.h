#ifndef CONFIG
#define CONFIG

#include "../../shared/include/shared_config.h"

void procesar_archivo_config_memoria(t_config*); 

char* ip_escucha;
char* puerto_escucha;
int tam_memoria;
int tam_pagina;
int paginas_por_tabla;
int retardo_memoria;
char* algoritmo_reemplazo;
int marcos_por_proceso;
int retardo_swap;
char* path_swap;

#endif