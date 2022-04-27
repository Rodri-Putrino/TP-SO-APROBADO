#ifndef HEADERS_PROCESOS_H_
#define HEADERS_PROCESOS_H_

#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include "./config.h"
#include "../../shared/include/shared_log.h"
#include "../../shared/include/shared_utils.h"
#include <commons/collections/list.h>

void iniciar_estructuras_de_estados_de_procesos();

void encolar_proceso_en_nuevos(t_pcb*);
t_pcb* desencolar_proceso_nuevo();

void encolar_proceso_en_listos(t_pcb*);
t_pcb* desencolar_proceso_listo();

void encolar_proceso_en_ejecucion(t_pcb*);
t_pcb* desencolar_proceso_en_ejecucion();

void encolar_proceso_en_bloqueados(t_pcb*);
t_pcb* desencolar_proceso_bloqueado();

void encolar_proceso_en_suspendidos_bloqueados(t_pcb*);
t_pcb* desencolar_proceso_suspendido_bloqueado();

void encolar_proceso_en_suspendidos_listos(t_pcb*);
t_pcb* desencolar_proceso_suspendido_listo();

void encolar_proceso_en_terminados(t_pcb*);
t_pcb* desencolar_t_pcberminado();

#endif /* HEADERS_PROCESOS_H_ */