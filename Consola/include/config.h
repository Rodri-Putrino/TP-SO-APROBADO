#ifndef CONFIG
#define CONFIG

#include <commons/config.h>
#include <stdio.h>

t_config* iniciar_config();
void procesar_archivo_config_consola(t_config*); 

char* ip_kernel;
char* puerto_kernel;

#endif