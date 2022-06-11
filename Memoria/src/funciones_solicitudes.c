#include "../include/funciones_solicitudes.h"

void inicializar_estructuras(int socket_cliente, t_log *logger)
{
    t_list *parametros = recibir_paquete(socket_cliente, logger);
	int *id = list_get(parametros, 0);
    int *tamanio_proceso = list_get(parametros, 1);

    //CREA TABLA NIVEL 1 Y TABLAS NIVEL 2
    proceso_en_memoria *proceso = asignar_proceso(*id, *tamanio_proceso);
    int dir_tabla = list_add(tablasN1, proceso->tablaN1);
    list_add(procesos_en_memoria, proceso);

    //RESERVA MARCOS
    reservar_marcos_proceso(proceso);

    //CREA ARCHIVO SWAP
    generar_nuevo_archivo(*id);

    //ENVIAR DIR TABLA NIVEL 1
    enviar_num(socket_cliente, dir_tabla, logger);
}

void suspender_proceso(int socket_cliente, t_log *logger)
{
    t_list *parametros = recibir_paquete(socket_cliente, logger);

    int *id = list_get(parametros, 0);
    int *dir_tablaN1 = list_get(parametros, 1);

    //ESCRIBIR PAGINAS SI FUERON MODIFICADAS
    suspender_paginas(*id, *dir_tablaN1);

    //LIBERAR MARCOS PARA SER USADOS POR OTROS PROCESOS
    liberar_marcos_proceso(*id);
}

void pedido_lectura(int socket_cliente, t_log *logger)
{
    t_list *parametros = recibir_paquete(socket_cliente, logger);

    //DIR FISICA EN DONDE LEER
    int *dir = list_get(parametros, 0);
    //TAMANIO DE CUANTO LEER (no es constante, maximo es 'sizeof(int)')
    int *tamanio = list_get(parametros, 1);

    void *dato = leer_memoria(*tamanio, *dir);

    //ENVIAR DATO
    t_paquete *lectura = crear_paquete(PEDIDO_LECTURA);
    agregar_a_paquete(lectura, dato, *tamanio);
    enviar_paquete(lectura, socket_cliente, logger);
    eliminar_paquete(lectura);
}

void pedido_escritura(int socket_cliente, t_log *logger)
{
    t_list *parametros = recibir_paquete(socket_cliente, logger);

    //DIR FISICA EN DONDE ESCRIBIR
    int *dir = list_get(parametros, 0);
    //TAMANIO DE CUANTO ESCRIBIR (no es constante, maximo es 'sizeof(int)')
    int *tamanio = list_get(parametros, 1);
    //DATO A ESCRIBIR
    void *dato = list_get(parametros, 2);

    escribir_memoria(dato, *tamanio, *dir);
    enviar_num(socket_cliente, 1, logger);//ESCRITURA COMPLETA (RESPUESTA OK)
}

void solicitud_tabla_paginas(int socket_cliente, t_log *logger)
{
    t_list *parametros = recibir_paquete(socket_cliente, logger);

    int *dir_tabla = list_get(parametros, 0);
    int *num_entrada = list_get(parametros, 1);

    t_tablaN1 *t = list_get(tablasN1, *dir_tabla);
    entrada_tabla_N1 *e = list_get(t, *num_entrada);

    t_tablaN2 *respuesta = list_get(tablasN2, e->dir);

    enviar_tabla_N2(socket_cliente, respuesta, logger);
}

void solicitud_marco(int socket_cliente, t_log *logger)
{
    t_list *parametros = recibir_paquete(socket_cliente, logger);

    int *id = list_get(parametros, 0);
    int *dir_tablaN1 = list_get(parametros, 1);
    entrada_tabla_N2 *e2 = list_get(parametros, 2);

    if(e2->bit_presencia == 0)
    {
        traer_pagina_a_memoria(*id, *dir_tablaN1, e2);
    }

    enviar_num(socket_cliente, e2->dir, logger);
}