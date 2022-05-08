#ifndef CREAR_ESTRUCTURAS
#define CREAR_ESTRUCTURAS

#include <stdlib.h>
#include <math.h>
#include <commons/collections/list.h>
#include <commons/bitarray.h>
#include "./config.h"

typedef struct
{
	int num_pag;
	int dir;
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

t_list *tablasN1, *tablasN2;

//MEMORIA PRINCIPAL (espacio usuario)
void *memoria;

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
    -Guarda las tablas de nivel 2 en la lista general 'tablasN2'
    -Asigna los num_tabla para las tablas de nivel 2 y los num_pagina
    -Asigna la dir de las tablas de nivel 2
    -No asigna las dir de las paginas
    -No guarda la tabla de nivel 1 en la lista general

    NOTA: las listas de tablas se eliminan todas juntas al final del proceso memoria
    ya que, si se eliminan antes, cambiaria las direcciones de todas
    las tablas porque cambiaria el tamanio de la lista general
*/
t_tablaN1* crear_tablaN1(int tamanio_proceso);

/*
    Libera las paginas
*/
void eliminar_paginas_proceso(t_tablaN1 *t);

/*
    Asigna memoria al puntero (inicializa memoria principal)
*/
void iniciar_memoria(void *mem, int tamanio_total);

/*
    Crea bitmap a partir del tamanio de la memoria y del tamanio de la pagina
    -Todos los bits comienzan en valor 1 
    NOTA: si el valor de un bit es 1 entonces el marco esta libre
    NOTA2: hay 1 byte perdido en el malloc de la variabl char *bitmap
*/
t_bitarray* crear_bitmap(int tamanio_memoria);

/*
    Otra forma de llamar a bitarray_destroy()
*/
void eliminar_bitmap(t_bitarray *bitmap);

//funcion auxiliar
void eliminar_lista(void *l);
void crear_listas_tablas();
void eliminar_listas_tablas();

#endif