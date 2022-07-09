#include "../include/uso_memoria.h"
/*

int tamanio_restante_pagina(int dir)
{
    int posicion_en_pagina = dir % tam_pagina;
    return (tam_pagina - posicion_en_pagina);
}

int dir_resto_dato_por_escribir(int tamanio_dato, int por_escribir)
{
    return tamanio_dato - por_escribir;
}

int conseguir_dir_marco_vacio(t_bitarray *marcos_memoria)
{
	int marco_vacio;
	for(marco_vacio = 0; !bitarray_test_bit(marcos_memoria, marco_vacio); marco_vacio++);
	int dir_marco_vacio = marco_vacio * tam_pagina;
	return dir_marco_vacio;
}

void escribir_memoria(void *dato, int tamanio_dato, int dir, int dir_tabla_pag)
{
    int por_escribir = tamanio_dato;
    while(por_escribir > 0)
    {
        int resto_pagina = tamanio_restante_pagina(dir);
        if(por_escribir <= resto_pagina)
        {
            memcpy(memoria + dir, dato + dir_resto_dato_por_escribir(tamanio_dato, por_escribir), por_escribir);
            por_escribir = 0;
        }
        else
        {
            memcpy(memoria + dir, dato + dir_resto_dato_por_escribir(tamanio_dato, por_escribir), resto_pagina);
            por_escribir -= resto_pagina;
            if(pagina_siguiente_en_memoria())
            {
                //buscarla
            }
            else
            {
                //reemplazar (depende de algoritmo)
            }
        }
    }
}*/

void escribir_memoria(void *dato, int tamanio_dato, int dir)
{
    //CONSIGUE PAGINA EN MARCO
    //void *memoria2 = malloc(4096);
    int marco = conseguir_marco_de_dir_fisica(dir);
    log_info(logger, "Pedido escritura en marco: %d",marco);
    entrada_tabla_N2 *pag = conseguir_pagina_en_marco(marco);
    log_info(logger, "Escribir pagina: %d", pag->num_pag);

    //PAGINA FUE USADA
    pag->bit_uso = 1;
    //PAGINA FUE MODIFICADA
    pag->bit_modificacion = 1;

    //ESCRIBE DATO
    log_info(logger, "Dirección física: %d", dir);
    log_info(logger, "Tamanio dato: %d", tamanio_dato);
    log_info(logger, "Dato: %d", (int) dato);
    //log_info(logger, "Memoria: %d", (int) memoria);
    //log_info(logger, "Memoria + Dir física: %d", *(int*) (memoria + dir));

    memcpy(memoria + dir, &dato, tamanio_dato);

    log_info(logger, "Un log_info muy trucho: %d", *(int*)(memoria +dir));
}

void* leer_memoria(int tamanio_dato, int dir)
{
    //CONSIGUE PAGINA EN MARCO
    int marco = conseguir_marco_de_dir_fisica(dir);
    entrada_tabla_N2 *pag = conseguir_pagina_en_marco(marco);

    //PAGINA FUE USADA
    pag->bit_uso = 1;

    //LEE Y RETORNA DATO
    void *ret = malloc(tamanio_dato);
    memcpy(ret, memoria + dir, tamanio_dato);
    return ret;
}

/*------------------------MANEJO PAGINAS--------------------------------*/

int dir_marco_vacio_proceso(int id)
{
    t_list *marcos = conseguir_numeros_marcos_proceso(id);
    for(int i = 0; i < list_size(marcos); i++)
    {
        int *num_marco = list_get(marcos, i);
        //SI ENCUENTRA VACIO LO RETORNA
        if(bitarray_test_bit(marcos_memoria, i));
            return *num_marco;
    }
    //SINO RETORNA -1
    return -1;
}

void traer_pagina_a_memoria(int id, int dir_tablaN1 ,entrada_tabla_N2 *e)
{
    //DIR MARCO VACIO O -1 SI NO ENCUENTRA
    int dir_marco = dir_marco_vacio_proceso(id);
    //BUSCAR PAGINA PARA REEMPLAZAR
    if(dir_marco == -1)
    {
        entrada_tabla_N2 *aux;
        if(strcmp(algoritmo_reemplazo, "CLOCK"))
        {
            aux = aplicar_busqueda_clock(id, dir_tablaN1);
        }
        else if(strcmp(algoritmo_reemplazo, "CLOCK-M"))
        {
            aux = aplicar_busqueda_clock_mejorado(id, dir_tablaN1);
        }
        //GUARDAR DIR MARCO ELEGIDO
        dir_marco = aux->dir;
        //SI FUE MODIFICADO, ESCRIBIR PAGINA EN MEMORIA
        if(aux->bit_modificacion == 1)
        {
            t_pedido_disco p;
            p.operacion_disco = ESCRIBIR_ARCHIVO;
            p.argumentos[0] = id;
            p.argumentos[1] = aux->dir;
            p.argumentos[2] = aux->num_pag;
            queue_push(pedidos_disco, &p);
            sem_post(&lista_tiene_pedidos);
        }
        aux->bit_presencia = 0;
    }
    e->bit_presencia = 1;
    log_info(logger, "el bit de presencia es: %d",e->bit_presencia);

    t_pedido_disco p;
    p.operacion_disco = LEER_ARCHIVO;
    p.argumentos[0] = id;
    p.argumentos[1] = dir_marco;
    p.argumentos[2] = e->num_pag;
    queue_push(pedidos_disco, &p);
    sem_post(&lista_tiene_pedidos);

    log_info(logger,"pagina %d del proceso %d lista en memoria",e->num_pag,id);
}