#ifndef EJECUCION_PROCESOS
#define EJECUCION_PROCESOS

#include <stdio.h>
#include <stdbool.h>
#include "./config.h"
#include "../../shared/include/shared_log.h"
#include "../../shared/include/shared_utils.h"
#include "../../shared/include/shared_servidor.h"
#include "../../shared/include/shared_mensajes.h"

void realizar_ciclo_de_instruccion(t_list*, int); //TODO Modificar a t_pcb*
int buscar_proxima_instruccion(t_list*); //TODO Modificar a t_pcb*
void interpretar_instruccion_y_ejecutar_pcb(int, t_list*, int); //TODO Modificar a t_pcb*
bool hay_interrupcion_para_atender();

#endif