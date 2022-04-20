#ifndef MENSAJES
#define MENSAJES

#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <string.h>
#include <stdlib.h>

typedef enum
{
	MENSAJE,
	PAQUETE,
	FINALIZAR_PROCESO,
	NUEVO_PROCESO,
	INICIALIZAR_ESTRUCTURAS,
	RECIBIR_PCB,
	INTERRUPCION,
	SUSPENDER_PROCESO,
	PROCESO_SUSPENDIDO,
	EXIT,
	IO,
	DESALOJAR_PROCESO,
	ENVIAR_HANDSHAKE,
	PEDIDO_LECTURA,
	PEDIDO_ESCRITURA,
	PEDIDO_COPIA,
	ACTUALIZAR_PCB,
	SOLICITUD_TABLA_PAGINAS,
	SOLICITUD_MARCO,
	SOLICITUD_DIRECCION_FISICA
}op_code;

typedef struct
{
	int size;
	void* stream;
} t_buffer;


typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

//ESTRUCTURAS
void crear_buffer(t_paquete* paquete);
t_paquete* crear_paquete(void);

//ENVIAR
int enviar_num(int coneccion, int num, t_log *logger);
int recibir_num(int coneccion, t_log* logger);
void* serializar_paquete(t_paquete* paquete, int bytes);
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
void enviar_paquete(t_paquete* paquete, int socket_cliente, t_log *logger);
void eliminar_paquete(t_paquete* paquete);
void enviar_mensaje(char* mensaje, int socket_cliente, t_log *logger);

//RECIBIR
void* recibir_buffer(int* size, int socket_cliente, t_log *logger);
char* recibir_mensaje(int socket_cliente, t_log *logger);
t_list* recibir_paquete(int socket_cliente, t_log *logger);


#endif