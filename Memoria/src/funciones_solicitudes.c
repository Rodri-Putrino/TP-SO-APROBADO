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
    imprimir_bitmap(marcos_memoria);

    //ENVIAR PEDIDO CREAR SWAP
    t_pedido_disco *p = crear_pedido_crear_archivo(*id);
    sem_wait(&(p->pedido_listo));

    //ENVIAR DIR TABLA NIVEL 1
    enviar_num(socket_cliente, dir_tabla, logger);

    log_info(logger,"estructuras del proceso %d creadas correctamente",*id);
    list_destroy_and_destroy_elements(parametros,free);
}

void suspender_proceso(int socket_cliente, t_log *logger)
{
    t_list *parametros = recibir_paquete(socket_cliente, logger);

    int *id = list_get(parametros, 0);
    int *dir_tablaN1 = list_get(parametros, 1);

    log_info(logger, "recibo de parametros de suspencion");

    //ESCRIBIR PAGINAS SI FUERON MODIFICADAS
    suspender_paginas(*id, *dir_tablaN1);

    //LIBERAR MARCOS PARA SER USADOS POR OTROS PROCESOS
    liberar_marcos_proceso(*id);
    imprimir_bitmap(marcos_memoria);
    list_destroy_and_destroy_elements(parametros,free);
}

void pedido_lectura(int socket_cliente, t_log *logger)
{
    uint32_t dir_fisica = recibir_pedido_lectura(socket_cliente, logger);

    log_info(logger, "Direccion fisica recibida: %d", dir_fisica);

    uint32_t dato = (uint32_t) leer_memoria(dir_fisica);

    //ENVIAR DATO A CPU ==> TODO
    //t_paquete *lectura = crear_paquete(PEDIDO_LECTURA);
    //agregar_a_paquete(lectura, (void*) dato, (int) sizeof(uint32_t));
    //enviar_paquete(lectura, socket_cliente, logger);
    //eliminar_paquete(lectura);

    enviar_valor_leido(dato, socket_cliente, logger);

    log_info(logger,"paquete de lectura enviado");

}

void pedido_escritura(int socket_cliente, t_log *logger)
{
    t_list *parametros = recibir_pedido_escritura(socket_cliente, logger);

    uint32_t dir_fisica = (uint32_t) list_get(parametros, 0);
    log_info(logger, "Direccion fisica recibida: %d", dir_fisica);

    uint32_t dato = (uint32_t) list_get(parametros, 1);
    log_info(logger, "Recibio dato: %d", dato);

    escribir_memoria(dato, dir_fisica);
    //enviar_num(socket_cliente, 1, logger);//ESCRITURA COMPLETA (RESPUESTA OK)
    enviar_mensaje("Escritura completa", socket_cliente, logger);
    
    log_info(logger,"paquete de escritura enviado");
    //list_destroy_and_destroy_elements(parametros,free);
    list_destroy(parametros);
}

void solicitud_tabla_paginas(int socket_cliente, t_log *logger)
{
    t_list *parametros = recibir_paquete(socket_cliente, logger);

    int *dir_tabla = list_get(parametros, 0);
    int *num_entrada = list_get(parametros, 1);

    log_info(logger,"recibo de parametros solicitud de tabla de paginas: %d Entrada: %d", *dir_tabla, *num_entrada);

    t_tablaN1 *t = list_get(tablasN1, *dir_tabla);
    log_info(logger, "Tamanio talba: %d", list_size(t));
    entrada_tabla_N1 *e = list_get(t, *num_entrada);

    t_tablaN2 *respuesta = list_get(tablasN2, e->dir);

    enviar_tabla_N2(socket_cliente, respuesta, logger);
    log_info(logger,"tabla de paginas %d enviada", *num_entrada);
    list_destroy_and_destroy_elements(parametros,free);
}   

void solicitud_marco(int socket_cliente, t_log *logger)
{
    t_list *parametros = recibir_paquete(socket_cliente, logger);

    int *id = list_get(parametros, 0);
    int *dir_tablaN1 = list_get(parametros, 1);
    int *num_pag = list_get(parametros, 2);
    log_info(logger,"el numero de pagina recibido por memoria es: %d",*num_pag);

    entrada_tabla_N2 *e2 = conseguir_entrada_pagina(*dir_tablaN1, *num_pag);
    

    log_info(logger,"el proceso %d solicita direccion fisica de pagina",*id);

    if(e2->bit_presencia == 0)
    {
        traer_pagina_a_memoria(*id, *dir_tablaN1, e2);
    }
    enviar_num(socket_cliente, e2->dir, logger);
    log_info(logger, "envio de marco para el proceso: %d", *id);
    list_destroy_and_destroy_elements(parametros,free);
}

void eliminar_proceso(int socket_cliente, t_log *logger){

    t_list *parametros = recibir_pedido_liberar_memoria(socket_cliente, logger);
    int id = (int)list_get(parametros, 0);
    int dir_tablaN1 = (int)list_get(parametros, 1);

    eliminar_paginas_proceso(id , dir_tablaN1); //chequear si podemos hacerlo sin que envien la dir XD POSTDATA: son la 1:20 no da pa pensar ahora :D

    t_pedido_disco *p = crear_pedido_eliminar_archivo(id);
    eliminar_estructura_proceso(id);
    sem_wait(&(p->pedido_listo));

    list_destroy(parametros);
}
