#ifndef CONFIG
#define CONFIG

#include "../../shared/include/shared_config.h"
#include "../../shared/include/shared_log.h"

void procesar_archivo_config_cpu(t_config*); 

t_log* logger;
int entradas_tlb;
char* reemplazo_tlb;
int retardo_noop;
char* ip_memoria;
char* puerto_memoria;
char* ip_escucha;
char* puerto_escucha_dispatch;
char* puerto_escucha_interrupt;

#endif