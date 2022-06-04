#include "../include/crear_estructuras.h"

/*----------------------MEMORIA PROCESO------------------------------------*/

entrada_tabla_N1* agregar_entrada_tablaN1(t_tablaN1 *tabla)
{
    entrada_tabla_N1* e = malloc(sizeof(entrada_tabla_N1));
    e->num_tabla = list_add(tabla, e);
    e->dir = -1;
    return e;
}

entrada_tabla_N2* agregar_entrada_tablaN2(t_tablaN2 *tabla)
{
    entrada_tabla_N2* e = malloc(sizeof(entrada_tabla_N2));
    e->num_pag = -1;
    e->dir = -1;
    e->bit_modificacion = 0;
    e->bit_presencia = 0;
    e->bit_uso = 0;
    list_add(tabla, e);
    return e;
}

int cantidad_paginas_necesarias(int tamanio_proceso)
{
    return ceil(tamanio_proceso / tam_pagina);
}

int cantidad_tablas_necesarias(int paginas_necesarias)
{
    return ceil(paginas_necesarias / paginas_por_tabla);
}

t_tablaN1* crear_tablaN1(int tamanio_proceso)
{
    t_tablaN1 *t = list_create();
    int paginas_necesarias = cantidad_paginas_necesarias(tamanio_proceso);

    for(int paginas_reservadas = 0; paginas_reservadas < paginas_necesarias; paginas_reservadas++)
    {
        if(paginas_reservadas % paginas_por_tabla == 0)
        {
            //NECESITO TABLA NUEVA
            //creo entrada
            entrada_tabla_N1 *e = agregar_entrada_tablaN1(t);
            //creo tabla nivel 2
            t_tablaN2 *t2 = list_create();
            //dir = index en lista general
            e->dir = list_add(tablasN2, t2);
        }
        //conseguir ultima entrada (ultima tabla 2)
        entrada_tabla_N1 *aux = list_get(t, list_size(t) -1);
        //buscar tabla en la dir que dice la entrada de tabla 1
        t_tablaN2 *aux2 = list_get(tablasN2, aux->dir);
        entrada_tabla_N2 *aux3 = agregar_entrada_tablaN2(aux2);
        
        //DIR = NUMERO PAGINA * TAMANIO PAGINA
        aux3->dir = aux3->num_pag * tam_pagina;
        aux3->bit_presencia = 0;
        
        aux3->num_pag = paginas_reservadas;
    }
    return t;
}
/* VERSION ANTERIOR
void eliminar_paginas_proceso(int dir_tablaN1)
{
    t_tablaN1 *t = list_get(tablasN1, dir_tablaN1);
    t_list_iterator *iteradorN1 = list_iterator_create(t);
    while(list_iterator_has_next(iteradorN1))
    {
        entrada_tabla_N1 *e1 = list_iterator_next(iteradorN1);
        t_list_iterator *iteradorN2 = list_iterator_create(list_get(tablasN2, e1->dir));
        while(list_iterator_has_next(iteradorN2))
        {
            entrada_tabla_N2 *e2 = list_iterator_next(iteradorN2);
            if(e2->bit_presencia == 1)
                liberar_marco_proceso(e2->dir);
            //NOTA: LIBERAR PAGINA
        }
        list_iterator_destroy(iteradorN2);
        list_replace_and_destroy_element(tablasN2, e1->dir, NULL, eliminar_lista);
    }
    list_iterator_destroy(iteradorN1);
    list_replace_and_destroy_element(tablasN1, dir_tablaN1, NULL, eliminar_lista);
}*/

void eliminar_paginas_proceso(int id, int dir_tablaN1)
{
    t_tablaN1 *t = list_get(tablasN1, dir_tablaN1);
    t_list_iterator *iteradorN1 = list_iterator_create(t);
    while(list_iterator_has_next(iteradorN1))
    {
        entrada_tabla_N1 *e1 = list_iterator_next(iteradorN1);
        t_tablaN2 *t2 = list_get(tablasN2, e1->dir);

        list_clean_and_destroy_elements(t2, free);

        list_replace_and_destroy_element(tablasN2, e1->dir, NULL, eliminar_lista);
    }
    list_iterator_destroy(iteradorN1);
    list_replace_and_destroy_element(tablasN1, dir_tablaN1, NULL, eliminar_lista);

    liberar_marcos_proceso(id);
}

void reservar_marcos_proceso(int id)
{
    t_reserva_marcos *aux = malloc(sizeof(t_reserva_marcos));

    int cantidad_marcos_reservados = 0;
    for(int i = 0; cantidad_marcos_reservados < marcos_por_proceso; i++)
    {
        if(bitarray_test_bit(marcos_memoria, i))
        {
            int *marco = malloc(sizeof(int));
            *marco = i;
            list_add(aux->marcos_reservados, marco);

            bitarray_clean_bit(marcos_memoria, i);
            cantidad_marcos_reservados++;
        }
    }
    aux->id_proceso = id;
    list_add(marcos_reservados_por_procesos, aux);
}

void liberar_marcos_proceso(int id)
{
    int i = 0;
    t_reserva_marcos *aux = list_get(marcos_reservados_por_procesos, i);
    for(i = 1; aux->id_proceso != id; i++)
    {
        aux = list_get(marcos_reservados_por_procesos, i);
    }
    aux = list_remove(marcos_reservados_por_procesos, i);

    desmarcar_bitmap(aux->marcos_reservados);

    list_destroy_and_destroy_elements(aux->marcos_reservados, free);
    free(aux);
}

/*---------------------MEMORIA GENERAL---------------------------------*/

void desmarcar_bitmap(t_list *marcos)
{
    t_list_iterator *iterador = list_iterator_create(marcos);
    while(list_iterator_has_next(iterador))
    {
        int *marco = list_iterator_next(iterador);
        bitarray_set_bit(marcos_memoria, *marco);
    }
}

void iniciar_memoria(void *mem, int tamanio_total)
{
    mem = malloc(tamanio_total);
}

t_bitarray* crear_bitmap(int tamanio_memoria)
{
    int cantidad_bits = ceil(tamanio_memoria / tam_pagina);
    int bytes_necesarios = ceil(cantidad_bits / 8);

    char *bitmap = malloc(bytes_necesarios);
    t_bitarray *ret = bitarray_create(bitmap, bytes_necesarios);

    for(int i = 0; i < cantidad_bits; i++)
    {
        bitarray_set_bit(ret, i);
    }

    return ret;
}

void eliminar_bitmap(t_bitarray *bitmap)
{
    free(bitmap->bitarray);
    free(bitmap);
}

void eliminador(void *info)
{
    if(info != NULL)
        eliminar_lista(info);
}

void eliminar_lista(void *l)
{
    list_destroy_and_destroy_elements(l, free);
}

void crear_listas_tablas()
{
    tablasN1 = list_create();
    tablasN2 = list_create();
}

void eliminar_listas_tablas()
{
    list_destroy_and_destroy_elements(tablasN1, eliminador);
    list_destroy_and_destroy_elements(tablasN2, eliminador);
}

int conseguir_marco_de_dir_fisica(int dir)
{
    return (int)floor(dir / tam_pagina);
}

entrada_tabla_N2* tabla_contiene_marco(t_tablaN2 *t, int num_marco)
{
    entrada_tabla_N2 *ret = list_get(t, 0);
    for(int i = 0; i < paginas_por_tabla; i++)
    {
        if(ret->bit_presencia == 1 && ret->dir == num_marco)
            return ret;
        
        list_get(t, i);
    }
    return NULL;
}

entrada_tabla_N2* conseguir_pagina_en_marco(int num_marco)
{
    t_list_iterator *iterador = list_iterator_create(tablasN2);
    entrada_tabla_N2 *ret;

    t_tablaN2 *t = list_iterator_next(iterador);
    while((ret = tabla_contiene_marco(t, num_marco)) == NULL)
        t = list_iterator_next(iterador);

    list_iterator_destroy(iterador);
    return ret;
}

entrada_tabla_N2* conseguir_entrada_pagina(int dir_tablaN1, int pag)
{
    //conseguir tabla nivel 1
    t_tablaN1 *t = list_get(tablasN1, dir_tablaN1);

    //numero entrada = division numero pagina por paginas tablas redondeado para arriba
    int num_entrada_n1 = ceil(pag / paginas_por_tabla);
    entrada_tabla_N1 *e1 = list_get(t, num_entrada_n1);

    //conseguir tabla nivel 2
    t_tablaN2 *t2 = list_get(tablasN2, e1->dir);

    //desplazamiento en tabla = resto de division anterior
    int num_entrada_n2 = pag % paginas_por_tabla;
    entrada_tabla_N2 *e2 = list_get(t2, num_entrada_n2);
    return e2;
}

t_list* conseguir_numeros_marcos_proceso(int id)
{
    t_reserva_marcos *ret = list_get(marcos_reservados_por_procesos, 0);
    for(int i = 1; ret->id_proceso != id; i++)
    {
        ret = list_get(marcos_reservados_por_procesos, i);
    }
    return ret->marcos_reservados;
}

t_list* conseguir_marcos_proceso(int dir_tablaN1)
{
    t_tablaN1 *t = list_get(tablasN1, dir_tablaN1);
    t_list *marcos = list_create();
    t_list_iterator *iterador = list_iterator_create(t);
    while(list_iterator_has_next(iterador))
    {
        //ENTRADA TIENE DIR DE TABLA NIVEL 2
        entrada_tabla_N1 *e1 = list_iterator_next(iterador);
        //CONSIGUE TABLA
        t_list_iterator *iterador2 = list_iterator_create(list_get(tablasN2, e1->dir));
        while(list_iterator_has_next(iterador2))
        {
            //CONSIGUE ENTRADA DE TABLA N2
            entrada_tabla_N2 *e2 = list_iterator_next(iterador2);
            if(e2->bit_presencia == 1)
            {
                list_add(marcos, e2);
            }
        }
        list_iterator_destroy(iterador2);
    }
    list_iterator_destroy(iterador);
    return marcos;
}