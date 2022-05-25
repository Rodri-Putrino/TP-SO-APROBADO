#ifndef SHARED_UTILS_H
#define SHARED_UTILS_H

#include <stdio.h>
#include <stdbool.h>
#include "./shared_log.h"
#include "./shared_config.h"
#include <commons/collections/list.h>

typedef struct
{
	int id;
	ssize_t tam_proceso; //(En bytes)
	t_list* instrucciones;
	int program_counter; //Indice de la lista
	int tabla_paginas; //indice a lista general de tablas nivel 1
	int estimacion_rafaga;
} t_pcb;

typedef struct
{
	int num_pag;
	int dir; //MARCO
	int bit_presencia;
	int bit_uso;
	int bit_modificacion;
}entrada_tabla_N2;

typedef struct
{
	int num_tabla; //id tabla2
	int dir; //index en lista general de tablasN2
} entrada_tabla_N1;

typedef t_list t_tablaN2; //elementos tipo entrada_tabla_N2
typedef t_list t_tablaN1; //elementos tipo entrada_tabla_N1

int tam_pagina;
int paginas_por_tabla;

void finalizar_programa(t_log*, t_config*);

#endif