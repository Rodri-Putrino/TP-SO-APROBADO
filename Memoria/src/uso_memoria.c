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
    int marco = conseguir_marco_de_dir_fisica(dir);
    entrada_tabla_N2 *pag = conseguir_pagina_en_marco(marco);

    //PAGINA FUE USADA
    pag->bit_uso = 1;
    //PAGINA FUE MODIFICADA
    pag->bit_modificacion = 1;

    //ESCRIBE DATO
    memcpy(memoria + dir, dato, tamanio_dato);
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

void traer_pagina_a_memoria(int dir_tablaN1, int pag)
{
    entrada_tabla_N2 *e = conseguir_entrada_pagina(dir_tablaN1, pag);

    e->dir = 0;//aplicar algoritmo
    e->bit_presencia = 1;
}

int asignar_marco()
{
    int num_marco;
    for(num_marco = 0; !bitarray_test_bit(marcos_memoria, num_marco); num_marco++);
    bitarray_set_bit(marcos_memoria, num_marco);
    return num_marco;
}

void liberar_marco_proceso(int dir)
{
    bitarray_clean_bit(marcos_memoria, dir);
}