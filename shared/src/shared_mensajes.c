#include "../include/shared_mensajes.h"

//NOTA:
//Los enteros que se envian estan en formato 'int', por ahi hay que cambiarlos a 'uint32_t'

int enviar_num(int coneccion, int num, t_log *logger)
{
    //send() ENVIA POR conexión EL num Y RETORNA LA CANTIDAD DE BYTES ENVIADOS
	int ret = send(coneccion, &num, sizeof(uint32_t), 0);
	if(ret < sizeof(uint32_t))
		log_error(logger, "Error al enviar numero");

	return ret;
}

int recibir_num(int coneccion, t_log* logger)
{
    //recv() RETORNA -1 SI EN CASO DE ERROR
	int num;
	if(recv(coneccion, &num, sizeof(int), MSG_WAITALL) > 0)
		return num;
	else
	{
        log_error(logger, "Error al recibir numero");
		return -1;
	}
}

/*-------------------------------*/
void* serializar_paquete(t_paquete* paquete, int bytes)
{
	//Inicia puntero vacio
	void * total_paquete = malloc(bytes);
	int desplazamiento = 0;

	//Copia codigo de operacion (la primer parte del mensaje que va a llegar)
	memcpy(total_paquete + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	
	//Copia tamanio del paquete
	memcpy(total_paquete + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	
	//Copia contenido del paquete
	memcpy(total_paquete + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return total_paquete;
}

void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio)
{
	//'Agranda' el paquete para agregar un valor
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	//Copia el tamanio del valor
	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	
	//Copia el valor
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	//Actualiza el tamanio del paquete (se agrego dato y tamanio del dato)
	paquete->buffer->size += tamanio + sizeof(int);
}

void enviar_paquete(t_paquete* paquete, int socket_cliente, t_log *logger)
{
	//Total mensaje = buffer + tamanio buffer + codigo operacion
	int bytes = paquete->buffer->size + 2*sizeof(int);
	
	//Transforma de t_paquete a *void
	void* a_enviar = serializar_paquete(paquete, bytes);

	//Envia datos
	if(send(socket_cliente, a_enviar, bytes, 0) <= 0)
		log_error(logger, "Datos no se enviaron correctamente");

	//Liberar datos
	free(a_enviar);
}

void eliminar_paquete(t_paquete* paquete)
{
	//Primer eliminar *void del paquete
	free(paquete->buffer->stream);

	//Despues el puntero que guardaba la estructura t_buffer
	free(paquete->buffer);

	//Despues el puntero que guardaba a t_paquete
	free(paquete);
}

void enviar_mensaje(char* mensaje, int socket_cliente, t_log *logger)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	//Definir codigo de operacion
	paquete->codigo_operacion = MENSAJE;
	
	//Iniciar estructura buffer del paquete
	paquete->buffer = malloc(sizeof(t_buffer));
	
	//Tamanio = largo mensaje + caracter '\0'
	paquete->buffer->size = strlen(mensaje) + 1;
	
	//Reservar espacio para mensaje
	paquete->buffer->stream = malloc(paquete->buffer->size);
	
	//Copiar mensaje en buffer
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	//Mensaje total = mensaje + tamanio + codigo operacion
	int bytes = paquete->buffer->size + 2*sizeof(int);

	//De paquete a *void
	void* a_enviar = serializar_paquete(paquete, bytes);
	send(socket_cliente, a_enviar, bytes, 0);
	/*printf("Msg %s \n", (char*)(a_enviar + 2*sizeof(int)));

	int check;
	if( (check = send(socket_cliente, a_enviar, bytes, 0)) <= 0)
		log_error(logger, "Mensaje no se envio correctamente");

	log_info(logger, "Se enviaron %d bytes", check);*/
	free(a_enviar);
	eliminar_paquete(paquete);
}


void crear_buffer(t_paquete* paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

t_paquete* crear_paquete(op_code cod_op)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = cod_op;
	crear_buffer(paquete);
	return paquete;
}

/*-----------------------------------*/

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) != 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void* recibir_buffer(int* size, int socket_cliente, t_log *logger)
{
	/*void* buffer;

	//Recibir tamanio de buffer
	if(recv(socket_cliente, size, sizeof(int), MSG_WAITALL) <= 0)
		log_error(logger, "Error al recibir tamanio buffer");
	
	//Asignar espacio en base a tamanio
	buffer = malloc(*size);
	
	//Recibir buffer
	if(recv(socket_cliente, buffer, *size, MSG_WAITALL) <= 0)
		log_error(logger, "Error al recibir buffer");

	return buffer;*/

	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente, t_log *logger)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente, logger);
	log_info(logger, "Mensaje recibido: \"%s\"", buffer);
	free(buffer);
}

t_list* recibir_paquete(int socket_cliente, t_log *logger)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	//Recibe tamanio del paquete y el paquete mismo
	buffer = recibir_buffer(&size, socket_cliente, logger);
	
	//Mientras: cantidad datos < tamanio paquete
	while(desplazamiento < size)
	{
		//Copia tamanio de dato
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		
		//Actualiza: se recibio un entero
		desplazamiento+=sizeof(int);
		
		//Asigna espacio segun tamanio recibido
		void* valor = malloc(tamanio); 
		
		//Copia valor recibido
		memcpy(valor, buffer+desplazamiento, tamanio);
		
		//Actualiza: recibio un dato
		desplazamiento+=tamanio;

		//Enlistar dato
		list_add(valores, valor);
		//free(valor);
	}

	free(buffer);
	return valores;
	return NULL;
}

void enviar_pcb_test(op_code cod_op, t_pcb* pcb, int socket_cliente, t_log* logger) {

	size_t size;
	log_info(logger, "ID antes de serializar: %u", pcb->id);
	log_info(logger, "TAM antes de serializar: %u", pcb->tam_proceso);
	log_info(logger, "Program_counter antes de serializar: %u", pcb->program_counter);
	log_info(logger, "Estimacion_anterior antes de serializar: %u", pcb->estimacion_anterior);
	log_info(logger, "Ultima_rafaga antes de serializar: %u", pcb->ultima_rafaga);
	void* stream = serializar_pcb_test(cod_op, pcb, &size, logger);
	if(send(socket_cliente, stream, size, 0) != size)
		log_error(logger, "Los datos no se enviaron correctamente");

	free(stream);
}

void* serializar_pcb_test(op_code cod_op, t_pcb* pcb, size_t* size, t_log* logger) {

	int length_lista = list_size(pcb->instrucciones);
	size_t size_instrucciones = sizeof(t_instruccion) * length_lista;
	log_info(logger, "Size instrucciones: %zu", size_instrucciones);

 	*size = sizeof(op_code) +
	 		sizeof(size_t) +
			size_instrucciones + //tamanio lista de instrucciones
			sizeof(t_instruccion) * length_lista + //tamanio de cada instruccion
	 		sizeof(uint32_t) * 5;
			//sizeof(rango_tiempo_t);

	size_t size_payload = *size - sizeof(op_code) - sizeof(size_t);
	log_info(logger, "Size payload: %zu", size_payload);

	void* stream = malloc(*size);
	int desplazamiento = 0;
	int desplazamiento_lista = 0;
	int indice = 0;

    memcpy(stream, &cod_op, sizeof(op_code));
	desplazamiento += sizeof(op_code);
	memcpy(stream + desplazamiento, &size_payload, sizeof(size_t));
	desplazamiento += sizeof(size_t);
	memcpy(stream + desplazamiento, &pcb->tam_proceso, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(stream + desplazamiento, &pcb->id, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(stream + desplazamiento, &pcb->program_counter, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(stream + desplazamiento, &pcb->estimacion_anterior, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(stream + desplazamiento, &pcb->ultima_rafaga, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	
	memcpy(stream + desplazamiento, &size_instrucciones, sizeof(size_t));
	desplazamiento += sizeof(size_t);
	
	while(desplazamiento_lista < size_instrucciones)
	{
		t_instruccion* instruccion = (t_instruccion*) list_get(pcb->instrucciones, indice);
		log_info(logger, "Instruccion a serializar: %d", instruccion->op);
		memcpy(stream + desplazamiento + desplazamiento_lista, instruccion, sizeof(t_instruccion));
		desplazamiento_lista += sizeof(t_instruccion);
		indice ++;
	}

    return stream;
}

t_pcb* recibir_pcb_test(int socket_cliente, t_log* logger) {

	//size_t size_payload = sizeof(uint32_t) * 5;
 	size_t size;
    if (recv(socket_cliente, &size, sizeof(size_t), 0) != sizeof(size_t)) {
        log_error(logger, "Los datos no se recibieron correctamente"); 
    }

    void* stream = malloc(size);
	log_info(logger, "Size payload: %zu", size);
	/*if (recv(socket_cliente, &size_payload, sizeof(size_t), 0) != sizeof(size_t))
        return false;*/

    if (recv(socket_cliente, stream, size, 0) != size) {
		log_error(logger, "Los datos no se recibieron correctamente");
    }

    t_pcb* pcb = deserializar_pcb_test(stream);
    free(stream); 
	return pcb;
}

t_pcb* deserializar_pcb_test(void* stream) {
	
	t_pcb* pcb = malloc(sizeof(t_pcb));
	size_t size_instrucciones;
	int desplazamiento = 0;
	int desplazamiento_lista = 0;

	memcpy(&pcb->tam_proceso, stream, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(&pcb->id, stream + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(&pcb->program_counter, stream + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(&pcb->estimacion_anterior, stream + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(&pcb->ultima_rafaga, stream + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(&size_instrucciones, stream + desplazamiento, sizeof(size_t));
	desplazamiento += sizeof(size_t);
	printf("Size instrucciones: %zu", size_instrucciones);
	
	pcb->instrucciones = list_create();

	while(desplazamiento_lista < size_instrucciones)
	{
		t_instruccion* instruccion = malloc(sizeof(t_instruccion));
		memcpy(instruccion, stream + desplazamiento + desplazamiento_lista, sizeof(t_instruccion));
		printf("Instruccion deserializada: %d", instruccion);
		list_add(pcb->instrucciones, instruccion);
		desplazamiento_lista += sizeof(t_instruccion);
	}

	//free(instruccion);

	return pcb;
}

void enviar_pcb_con_tiempo_bloqueo(op_code cod_op, t_pcb* pcb,int tiempo_bloqueo, int socket_cliente, t_log* logger) {

	size_t bytes;
	void* a_enviar = serializar_pcb_test(cod_op, pcb, &bytes, logger);
	if(send(socket_cliente, a_enviar, bytes, 0) <= 0)
		log_error(logger, "Los datos no se enviaron correctamente");

	free(a_enviar);
}

void* serializar_pcb_con_tiempo_bloqueo(op_code cod_op, t_pcb* pcb,int tiempo_bloqueo, size_t* bytes) {

    *bytes = sizeof(op_code) + // cop
			 sizeof(int) + // tamanio proceso
 			 sizeof(int)       // tamanio id
			  /*
		 	 sizeof(t_list) +  // tamanio instrucciones
			 sizeof(int) +     // tamanio program counter
			 sizeof(int) +     // tamanio tabla de páginas
			 sizeof(int)       // tamanio estimacion de ráfaga
			 */
    ;

	//size_t sz_proceso = *bytes - sizeof(op_code) - sizeof(ssize_t);

    void* stream = malloc(*bytes);
	int desplazamiento = 0;

    memcpy(stream + desplazamiento, &cod_op, sizeof(op_code));
	desplazamiento += sizeof(op_code);
	//memcpy(stream + desplazamiento, &sz_proceso, sizeof(ssize_t));
	memcpy(stream + desplazamiento, &(pcb->tam_proceso), sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(stream + desplazamiento, &(pcb->id), sizeof(int));
	//desplazamiento += sizeof(int);
	/*
	memcpy(stream + desplazamiento, &(pcb->instrucciones), sizeof(t_list));
	desplazamiento += sizeof(t_list);

	memcpy(stream + desplazamiento, &(pcb->program_counter), sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(stream + desplazamiento, &(pcb->tabla_paginas), sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(stream + desplazamiento, &(pcb->estimacion_rafaga), sizeof(int));
	desplazamiento += sizeof(int);

	*/
    return stream;
}

t_pcb* recibir_pcb_con_tiempo_bloqueo(int socket_cliente, t_log* logger, int* tiempo_bloqueo) {

	t_pcb* pcb = malloc(sizeof(t_pcb));
	int size;
	int desplazamiento = 0;
	void* buffer;
	//int tamanio;

	log_info(logger, "Antes de recibir_buffer");

	buffer = recibir_buffer(&size, socket_cliente, logger);

	//memcpy(&tamanio, buffer + desplazamiento, sizeof(ssize_t));
	//desplazamiento += sizeof(ssize_t);

	log_info(logger, "Despues de recibir_buffer");

	memcpy(pcb->tam_proceso, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);

	//memcpy(&tamanio, buffer + desplazamiento,sizeof(int));
	//desplazamiento += sizeof(int);
	log_info(logger, "Despues de recibir tam");

	memcpy(pcb->id, buffer + desplazamiento, sizeof(int));
	//desplazamiento += tamanio;

	/*memcpy(&tamanio, buffer + desplazamiento, sizeof(t_list));
	desplazamiento += sizeof(t_list);

	memcpy(&(pcb->instrucciones), buffer + desplazamiento, tamanio);
	desplazamiento += tamanio;

	memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(&(pcb->program_counter), buffer + desplazamiento, tamanio);
	desplazamiento += tamanio;

	memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(&(pcb->tabla_paginas), buffer + desplazamiento, tamanio);
	desplazamiento += tamanio;

	memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);
	
	memcpy(&(pcb->estimacion_rafaga), buffer + desplazamiento, tamanio);
	desplazamiento += tamanio;*/

	free(buffer);
	return pcb; 
}

