#include "../include/tlb.h"

void agregar_entrada_tlb(entrada_tabla_N2 *e)
{
    entrada_tlb *aux = malloc(sizeof(entrada_tlb));
    aux->num_pag = e->num_pag;
    aux->marco = e->dir;

    if(list_size(tlb) == entradas_tlb)
    {
        //APLICAR ALGORITMO REEMPLAZO
        //ELIMINAR PAGINA ELEGIDA
    }

    list_add(tlb, aux);
}

int buscar_pagina_tlb(int pag)
{
    for(int i = 0; i < entradas_tlb; i++)
    {
        entrada_tlb *info = list_get(tlb, i);
        //SI ES LA PAG, RETORNAR MARCO
        if(info->num_pag == pag)
            return info->marco;
    }
    //NUNCA ENCONTRO MARCO, DEVUELVE PAGE FAULT (representado por un -1)
    return -1;
}

void limpiar_tlb()
{
    list_clean_and_destroy_elements(tlb, free);
}