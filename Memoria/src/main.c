#include "../include/main.h"

#define CONFIG_FILE_PATH "./cfg/Memoria.config"
#define LOG_FILE_PATH "./cfg/Memoria.log"
#define NOMBRE_MODULO "MEMORIA"

int main(void) {

	t_config* config_memoria;
	logger = iniciar_logger(LOG_FILE_PATH, NOMBRE_MODULO);

	config_memoria = iniciar_config(CONFIG_FILE_PATH);
	procesar_archivo_config_memoria(config_memoria);

	iniciar_estructuras_memoria();

	
	/*t_bitarray* bitmapTest = crear_bitmap(tam_memoria);

	imprimir_bitmap(bitmapTest)
	*/
	pthread_t hilo_disco;
	pthread_create(&hilo_disco, NULL, (void*)funciones_disco, NULL);

	pthread_t hilo_servidor;
    pthread_create(&hilo_servidor, NULL, (void*) servidor, NULL);
	
	pthread_join(hilo_servidor, NULL);

	

//	USADOR PARA PROBAR CREAR Y DESTRUIR TABLAS
/*
	t_tablaN1 *t_aux = crear_tablaN1(tam_pagina * 5);
	list_add(tablasN1, t_aux);
	t_tablaN1 *t = crear_tablaN1(tam_pagina * 10);
	list_add(tablasN1, t);
	eliminar_paginas_proceso(0);

	t_list_iterator *i1 = list_iterator_create(t);
	while(list_iterator_has_next(i1))
	{
		entrada_tabla_N1 *e1 = list_iterator_next(i1);
		t_list_iterator *i2 = list_iterator_create(list_get(tablasN2, e1->dir));
		while(list_iterator_has_next(i2))
		{
			entrada_tabla_N2 *e2 = list_iterator_next(i2);
			printf("NUM PAGINA: %d\n", e2->num_pag);
			printf("dir: %d\n", e2->dir);
			printf("bit presencia: %d\n", e2->bit_presencia);
			printf("bit uso: %d\n", e2->bit_uso);
			printf("bit modificacion: %d\n", e2->bit_modificacion);
		}
		list_iterator_destroy(i2);
	}
	list_iterator_destroy(i1);
*/
	eliminar_listas_tablas();

	finalizar_programa(logger, config_memoria);
	
	return EXIT_SUCCESS;
}
