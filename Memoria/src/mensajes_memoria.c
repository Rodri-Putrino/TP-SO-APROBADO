#include "../include/mensajes_memoria.h"

void gestionarMensaje (t_paquete* paqueteRecibido, int socketCliente, t_log* logger) {

		switch(paqueteRecibido->codigo_operacion) {

			case ENVIAR_HANDSHAKE:
				log_info(logger, "Se recibió un handshake");
				break;

			case INICIALIZAR_ESTRUCTURAS:
				log_info(logger, "Se recibio el mensaje INICIALIZAR_ESTRUCTURAS");
				break;

			case SUSPENDER_PROCESO:
				log_info(logger,"Se recibió el mensaje SUSPENDER_PROCESO");
				break;

			case PEDIDO_LECTURA:
				log_info(logger,"Se recibió el mensaje PEDIDO_LECTURA");
				break;

			case PEDIDO_ESCRITURA:
				log_info(logger,"Se recibió el mensaje PEDIDO_ESCRITURA");
				break;

			case PEDIDO_COPIA:
				log_info(logger,"Se recibió el mensaje PEDIDO_COPIA");
				break;

			case SOLICITUD_TABLA_PAGINAS:
				log_info(logger,"Se recibió el mensaje SOLICITUD_TABLA_PAGINAS");
				break;

			case SOLICITUD_MARCO:
				log_info(logger,"Se recibió el mensaje SOLICITUD_MARCO");
				break;

			case SOLICITUD_DIRECCION_FISICA:
				log_info(logger,"Se recibió el mensaje SOLICITUD_DIRECCION_FISICA");
				break;

			default:
				printf("default de gestionarMensajes");
				break;
		}
	}