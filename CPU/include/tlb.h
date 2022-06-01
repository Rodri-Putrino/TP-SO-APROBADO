#ifndef TLB
#define TLB

#include <stdlib.h>
#include <commons/collections/list.h>
#include "../include/config.h"
#include "../../shared/include/shared_utils.h"

typedef struct
{
    int num_pag;
    int marco;
}entrada_tlb;

t_list *tlb;

void agregar_entrada_tlb(entrada_tabla_N2 *e);
int buscar_pagina_tlb(int pag);
void limpiar_tlb();

#endif