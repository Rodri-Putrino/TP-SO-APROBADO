#include "../include/main.h"

#define CONFIG_FILE_PATH "./cfg/Memoria.config"
#define LOG_FILE_PATH "./cfg/Memoria.log"
#define NOMBRE_MODULO "MEMORIA"

int main(void) {

	t_config* config_memoria;
	logger = iniciar_logger(LOG_FILE_PATH, NOMBRE_MODULO);

	config_memoria = iniciar_config(CONFIG_FILE_PATH);
	procesar_archivo_config_memoria(config_memoria);

/*	pthread_t hilo_servidor;
    pthread_create(&hilo_servidor, NULL, (void*) servidor, NULL);

	pthread_join(hilo_servidor, NULL);*/

	crear_listas_tablas();

//	USADOR PARA PROBAR CREAR Y DESTRUIR TABLAS

	t_tablaN1 *t = crear_tablaN1(tam_pagina * 10);

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
	list_add(tablasN1, t);

	eliminar_listas_tablas();

	finalizar_programa(logger, config_memoria);
	
	return EXIT_SUCCESS;
}
