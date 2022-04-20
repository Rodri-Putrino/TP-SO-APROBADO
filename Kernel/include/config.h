#ifndef CONFIG
#define CONFIG

#include "../../shared/include/shared_config.h"

void procesar_archivo_config_kernel(t_config*); 

char* ip_memoria;
int puerto_memoria;
char* ip_cpu;
int puerto_cpu_dispatch;
int puerto_cpu_interrupt;
int puerto_escucha;
char* algoritmo_planificacion;
int estimacion_inicial;
double alfa;
int grado_multiprogramacion;
int tiempo_max_bloqueado; 

#endif