#include "../include/crear_estructuras.h"

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
        aux3->num_pag = paginas_reservadas;
    }
    return t;
}