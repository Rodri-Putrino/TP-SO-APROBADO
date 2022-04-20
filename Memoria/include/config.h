#ifndef CONFIG
#define CONFIG

#include "../../shared/include/shared_config.h"

void procesar_archivo_config_memoria(t_config*); 

int puerto_escucha;
int tam_memoria;
int tam_pagina;
int paginas_por_tabla;
int retardo_memoria;
char* algoritmo_reemplazo;
int marcos_por_proceso;
int retardo_swap;
char* path_swap;

#endif

/*
PUERTO_ESCUCHA=8002
TAM_MEMORIA=4096
TAM_PAGINA=64
PAGINAS_POR_TABLA=4
RETARDO_MEMORIA=1000
ALGORITMO_REEMPLAZO=CLOCK-M
MARCOS_POR_PROCESO=4
RETARDO_SWAP=2000
PATH_SWAP=/home/utnso/swap
*/