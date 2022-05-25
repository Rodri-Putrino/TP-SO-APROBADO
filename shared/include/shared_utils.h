#ifndef SHARED_UTILS_H
#define SHARED_UTILS_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "./shared_log.h"
#include "./shared_config.h"
#include <commons/collections/list.h>

typedef struct
{
	int id;
	int tam_proceso; //(En bytes)
	t_list* instrucciones;
	int program_counter; //Indice de la lista
	int tabla_paginas; //indice a lista general de tablas nivel 1
	int estimacion_rafaga;
} t_pcb;

void finalizar_programa(t_log*, t_config*);

#endif