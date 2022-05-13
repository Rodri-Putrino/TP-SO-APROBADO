#ifndef EJECUCION_PROCESOS
#define EJECUCION_PROCESOS

#include <stdio.h>
#include <stdbool.h>
#include "./config.h"
#include "../../shared/include/shared_log.h"
#include "../../shared/include/shared_utils.h"
#include "../../shared/include/shared_servidor.h"
#include "../../shared/include/shared_mensajes.h"

void realizar_ciclo_de_instruccion(t_pcb*);

#endif