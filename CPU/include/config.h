#ifndef CONFIG
#define CONFIG

#include "../../shared/include/shared_config.h"

void procesar_archivo_config_cpu(t_config*); 

int entradas_tlb;
char* reemplazo_tlb;
int retardo_noop;
char* ip_memoria;
int puerto_memoria;
int puerto_escucha_dispatch;
int puerto_escucha_interrupt;

#endif