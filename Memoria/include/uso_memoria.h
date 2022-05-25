#ifndef USO_MEMORIA
#define USO_MEMORIA

#include <stdio.h>
#include <commons/bitarray.h>
#include "./config.h"
#include "./crear_estructuras.h"

void escribir_memoria(void *dato, int tamanio_dato, int dir);
void* leer_memoria(int tamanio_dato, int dir);
void traer_pagina_a_memoria(int dir_tablaN1, int pag);
int asignar_marco();
void liberar_marco_proceso(int dir_tablaN1);

#endif