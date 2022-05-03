#include "../include/uso_memoria.h"

int conseguir_dir_marco_vacio(t_bitarray *marcos_memoria)
{
	int marco_vacio;
	for(marco_vacio = 0; !bitarray_test_bit(marcos_memoria, marco_vacio); marco_vacio++);
	int dir_marco_vacio = marco_vacio * tam_pagina;
	return dir_marco_vacio;
}

int tamanio_restante_pagina(int dir)
{
    int posicion_en_pagina = dir % tam_pagina;
    return (tam_pagina - posicion_en_pagina);
}

int dir_resto_dato_por_escribir(int tamanio_dato, int por_escribir)
{
    return tamanio_dato - por_escribir;
}

void escribir_memoria(void *dato, int tamanio_dato, int dir, t_bitarray *marcos_memoria,void *memoria)
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
            bitarray_clean_bit(marcos_memoria, dir / tam_pagina);
            memcpy(memoria + dir, dato + dir_resto_dato_por_escribir(tamanio_dato, por_escribir), resto_pagina);
            por_escribir -= resto_pagina;
            dir = conseguir_dir_marco_vacio(marcos_memoria);
        }
    }
}