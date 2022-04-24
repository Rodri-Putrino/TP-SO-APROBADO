#ifndef SERVER
#define SERVER

#include <stdio.h>
#include <stdbool.h>
#include "./config.h"
#include "../../shared/include/shared_servidor.h"
#include "../../shared/include/shared_mensajes.h"
#include <commons/collections/list.h>

void servidor_interrupt();
void servidor_dispatch();
void atender_peticiones_interrupt(void*);
void atender_peticiones_dispatch(void*);

#endif