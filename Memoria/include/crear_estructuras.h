#ifndef CREAR_ESTRUCTURAS
#define CREAR_ESTRUCTURAS

#include <stdlib.h>
#include <math.h>
#include <commons/collections/list.h>
#include "./config.h"

typedef struct
{
	int num_pag;
	int dir;
	int bit_presencia;
	int bit_uso;
	int bit_modificacion;
}entrada_tabla_N2;

typedef t_list t_tablaN1; //elementos tipo entrada_tabla_N2

typedef struct
{
	int num_tabla; //id tabla2
	int dir; //index en lista general de tablasN2
} entrada_tabla_N1;

typedef t_list t_tablaN2; //elementos tipo entrada_tabla_N1;

t_list *tablasN1, *tablasN2;

/* 
    Crea una entrada de tablaN1
    -Define num_tabla
    -No define dir (valor -1)
*/
entrada_tabla_N1* agregar_entrada_tablaN1(t_tablaN1 *tabla);

/*
    Igual a crear entrada tablaN1 pero para tablaN2:
    -Valores de bits inician en 0
    -dir = -1
    -num_pagina = -1
*/
entrada_tabla_N2* agregar_entrada_tablaN2(t_tablaN2 *tabla);
int cantidad_paginas_necesarias(int tamanio_proceso);
int cantidad_tablas_necesarias(int paginas_necesarias);

/*
    Crea tabla de nivel 1 con las tablas de nivel 2 necesarias
    Guarda las tablas en las listas generales 'tablasN1' y 'tablasN2'
*/
t_tablaN1* crear_tablaN1(int tamanio_proceso);

#endif