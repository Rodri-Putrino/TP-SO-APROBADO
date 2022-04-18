#ifndef CONFIG
#define CONFIG

#include <commons/config.h>
#include <stdio.h>

void iniciar_config();
void procesar_archivo_config_consola(t_config*); 

char* ip_kernel;
char* puerto_kernel;

#endif