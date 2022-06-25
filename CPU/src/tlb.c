#include "../include/tlb.h"

void iniciar_estructuras()
{
    tlb = list_create();
}

void agregar_entrada_tlb(entrada_tabla_N2 *e)
{
    entrada_tlb *aux = malloc(sizeof(entrada_tlb));
    aux->num_pag = e->num_pag;
    aux->marco = e->dir;

    if(list_size(tlb) == entradas_tlb)
        reemplazar_pagina(e);

    list_add(tlb, aux);
}

int buscar_pagina_tlb(int pag)
{
    for(int i = 0; i < list_size(tlb); i++)
    {
        entrada_tlb *info = list_get(tlb, i);
        //SI ES LA PAG, RETORNAR MARCO
        if(info->num_pag == pag)
        {
            if(strcmp(reemplazo_tlb, "LRU"))
            {
                //PONER ENTRADA AL FINAL (ahora tiene mayor prioridad)
                entrada_tabla_N2 *e = list_remove(tlb, i);
                list_add(tlb, e);
            }
            return info->marco;
        }
    }
    //NUNCA ENCONTRO MARCO, DEVUELVE PAGE FAULT (representado por un -1)
    return -1;
}

/*
    NOTA: los criterios son LRU y FIFO
    -En el caso de FIFO solo se elimina el primer elemento (el primero que fue agregado)
    y se agrega el nuevo al final (ultimo en llegar)
    -En el caso de LRU se reordena la lista cada vez que hay un TLB HIT
    (se referencia una entrada que esta en la tlb) por lo que ambas funciones
    reemplazan la pagina de la misma forma
*/
void reemplazar_pagina(entrada_tabla_N2 *e)
{
    list_remove_and_destroy_element(tlb, 0, free);
    list_add(tlb, e);
}

void limpiar_tlb()
{
    list_clean_and_destroy_elements(tlb, free);
}
