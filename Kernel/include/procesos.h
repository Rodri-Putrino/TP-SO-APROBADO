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
t_pcb* crear_proceso(int, ssize_t, t_list*);

void encolar_proceso_en_nuevos(t_pcb*);
t_pcb* desencolar_proceso_nuevo();

void encolar_proceso_en_listos(t_pcb*);
t_pcb* desencolar_proceso_listo();

void encolar_proceso_en_ejecucion(t_pcb*);
t_pcb* desencolar_proceso_en_ejecucion();
bool hay_proceso_en_ejecucion();

void encolar_proceso_en_bloqueados(t_pcb*);
t_pcb* desencolar_proceso_bloqueado();

void encolar_proceso_en_suspendidos_bloqueados(t_pcb*);
t_pcb* desencolar_proceso_suspendido_bloqueado();

void encolar_proceso_en_suspendidos_listos(t_pcb*);
t_pcb* desencolar_proceso_suspendido_listo();
bool hay_proceso_suspendido_listo();

void encolar_proceso_en_terminados(t_pcb*);
t_pcb* desencolar_proceso_terminado();

int cantidad_procesos_en_sistema();

sem_t sem_multiprogramacion;
sem_t sem_proceso_nuevo;
sem_t sem_proceso_listo;
sem_t sem_proceso_suspendido_ready;

#endif /* HEADERS_PROCESOS_H_ */