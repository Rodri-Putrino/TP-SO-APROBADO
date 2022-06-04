#include "../include/funciones_solicitudes.h"

void inicializar_estructuras(int socket_cliente, t_log *logger)
{
    t_list *parametros = recibir_paquete(socket_cliente, logger);
	int *id = list_get(parametros, 0);
    int *tamanio_proceso = list_get(parametros, 1);

    //RESERVA MARCOS
    reservar_marcos_proceso(*id);

    //CREA TABLA NIVEL 1 Y TABLAS NIVEL 2
    t_tablaN1 *t = crear_tablaN1(*tamanio_proceso);
    int dir_tabla = list_add(tablasN1, t);

    //CREA ARCHIVO SWAP
    generar_nuevo_archivo(*id);

    //ENVIAR DIR TABLA NIVEL 1
    enviar_num(socket_cliente, dir_tabla, logger);
}

void suspender_proceso(int socket_cliente, t_log *logger)
{
    int id = recibir_num(socket_cliente, logger);

    //ESCRIBIR PAGINAS SI FUERON MODIFICADAS
    //suspender_paginas()

    //LIBERAR MARCOS PARA SER USADOS POR OTROS PROCESOS
    liberar_marcos_proceso(id);
}

void pedido_lectura(int socket_cliente, t_log *logger)
{
    t_list *parametros = recibir_paquete(socket_cliente, logger);

    //DIR FISICA EN DONDE LEER
    int *dir = list_get(parametros, 0);
    //TAMANIO DE CUANTO LEER (no es constante, maximo es 'sizeof(int)')
    int *tamanio = list_get(parametros, 1);

    int *dato = leer_memoria(*tamanio, *dir);

    //ENVIAR DATO
    enviar_num(socket_cliente, *dato, logger);
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
}

void solicitud_tabla_paginas(int socket_cliente, t_log *logger)
{
    t_list *parametros = recibir_paquete(socket_cliente, logger);

    int *dir_tabla = list_get(parametros, 0);
    int *num_entrada = list_get(parametros, 1);

    t_tablaN1 *t = list_get(tablasN1, *dir_tabla);
    entrada_tabla_N1 *e = list_get(t, *num_entrada);

    t_tablaN2 *respuesta = list_get(tablasN2, e->dir);

    t_paquete *a_enviar = crear_paquete(SOLICITUD_TABLA_PAGINAS);
    //agregar_a_paquete(a_enviar, respuesta);
    //NOTA: REVISAR SI SE PUEDE ENVIAR t_list POR PAQUETE
    eliminar_paquete(a_enviar);
}

void solicitud_marco(int socket_cliente, t_log *logger)
{
    t_list *parametros = recibir_paquete(socket_cliente, logger);

    int *id = list_get(parametros, 0);
    entrada_tabla_N2 *e2 = list_get(parametros, 1);

    if(e2->bit_presencia == 0)
    {
        traer_pagina_a_memoria(e2);
    }

    enviar_num(socket_cliente, e2->dir, logger);
}