#ifndef HEADERS_PROCESOS_H_
#define HEADERS_PROCESOS_H_

#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "./config.h"
#include "../../shared/include/shared_log.h"
#include "../../shared/include/shared_utils.h"
#include <commons/collections/list.h>

void iniciar_estructuras_de_estados_de_procesos();
t_pcb* crear_proceso(uint32_t, uint32_t, t_list*);
void modificar_identificador_tabla_de_paginas_del_proceso(t_pcb* pcb, uint32_t id_tabla);
void destruir_proceso(t_pcb* pcb);

void encolar_proceso_en_nuevos(t_pcb* pcb);
t_pcb* desencolar_proceso_nuevo();

void encolar_proceso_en_listos(t_pcb* pcb);
t_pcb* desencolar_proceso_listo();

void encolar_proceso_en_ejecucion(t_pcb* pcb);
t_pcb* desencolar_proceso_en_ejecucion();
int hay_proceso_en_ejecucion();

void encolar_proceso_en_bloqueados(t_pcb* pcb);
t_pcb* desencolar_proceso_bloqueado();

void encolar_proceso_en_suspendidos_bloqueados(t_pcb* pcb);
t_pcb* desencolar_proceso_suspendido_bloqueado();

void encolar_proceso_en_suspendidos_listos(t_pcb* pcb);
t_pcb* desencolar_proceso_suspendido_listo();
int hay_proceso_suspendido_listo();

void encolar_proceso_en_terminados(t_pcb* pcb);
t_pcb* desencolar_proceso_terminado();

int cantidad_procesos_en_sistema();
void ordenar_cola_listos();
void proceso_iniciar_rafaga(t_pcb* pcb);
void proceso_finalizar_rafaga(t_pcb* pcb);
void actualizar_estimacion_anterior(t_pcb* pcb);
int proxima_rafaga_estimada(t_pcb* pcb);
int mayor_prioridad(t_pcb *pcb1, t_pcb *pcb2);
int puede_suspenderse(t_pcb*); 
float timedifference_msec(struct timeval, struct timeval);

sem_t sem_multiprogramacion;
sem_t sem_proceso_nuevo;
sem_t sem_proceso_listo;
sem_t sem_proceso_suspendido_listo;
sem_t sem_proceso_a_suspender;

#endif /* HEADERS_PROCESOS_H_ */