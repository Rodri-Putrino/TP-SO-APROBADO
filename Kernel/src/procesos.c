#include "../include/procesos.h"

static t_list *cola_nuevos;
static t_list *cola_listos;
static t_list *cola_ejecucion;
static t_list *cola_bloqueados;
static t_list *cola_suspendidos_bloqueados;
static t_list *cola_suspendidos_listos;
static t_list *cola_terminados;

pthread_mutex_t proceso_mutex;
pthread_mutex_t procesos_nuevos_mutex;
pthread_mutex_t procesos_listos_mutex;
pthread_mutex_t procesos_ejecutando_mutex;
pthread_mutex_t procesos_bloqueados_mutex;
pthread_mutex_t procesos_suspendidos_bloqueados_mutex;
pthread_mutex_t procesos_suspendidos_listos_mutex;
pthread_mutex_t procesos_terminados_mutex;

void iniciar_estructuras_de_estados_de_procesos() {
    cola_nuevos = list_create();
    cola_listos = list_create();
    cola_ejecucion = list_create();
    cola_bloqueados = list_create();
    cola_suspendidos_bloqueados = list_create();
    cola_suspendidos_listos = list_create();
    cola_terminados = list_create();

    pthread_mutex_init(&proceso_mutex, NULL);
    pthread_mutex_init(&procesos_nuevos_mutex, NULL);
    pthread_mutex_init(&procesos_listos_mutex, NULL);
    pthread_mutex_init(&procesos_ejecutando_mutex, NULL);
    pthread_mutex_init(&procesos_bloqueados_mutex, NULL);
    pthread_mutex_init(&procesos_suspendidos_bloqueados_mutex, NULL);
    pthread_mutex_init(&procesos_suspendidos_listos_mutex, NULL);
    pthread_mutex_init(&procesos_terminados_mutex, NULL);
}

t_pcb* crear_proceso(int id, ssize_t tam, t_list* lista_instrucciones) {
    
    pthread_mutex_lock(&proceso_mutex);

    t_pcb* pcb_nuevo = malloc(sizeof(t_pcb));

    pcb_nuevo->id = id;
    pcb_nuevo->tam_proceso = tam;
    pcb_nuevo->instrucciones = list_create();
    pcb_nuevo->program_counter = 0;
    //pcb_nuevo->tabla_paginas = ...
    pcb_nuevo->estimacion_rafaga = estimacion_inicial;

    t_list_iterator *iterador_proceso = list_iterator_create(lista_instrucciones);
    while(list_iterator_has_next(iterador_proceso))
    {
        char *i = list_iterator_next(iterador_proceso);
        list_add(pcb_nuevo->instrucciones, i);
    }
    list_iterator_destroy(iterador_proceso);

    pthread_mutex_unlock(&proceso_mutex);

    return pcb_nuevo;
}

/* --------------- Funciones Procesos Nuevos --------------- */

void encolar_proceso_en_nuevos(t_pcb* proceso) {

    pthread_mutex_lock(&procesos_nuevos_mutex);

    list_add(cola_nuevos, proceso);

    pthread_mutex_unlock(&procesos_nuevos_mutex);

    //sem_post(&sem_proceso_nuevo);
}

t_pcb* desencolar_proceso_nuevo() {

    pthread_mutex_lock(&procesos_nuevos_mutex);

    t_pcb* proceso = list_remove(cola_nuevos, 0);

    pthread_mutex_unlock(&procesos_nuevos_mutex);

  return proceso;
}

/* --------------- Funciones Procesos Listos --------------- */

void encolar_proceso_en_listos(t_pcb* proceso) {

    pthread_mutex_lock(&procesos_listos_mutex);

    list_add(cola_listos, proceso);

    pthread_mutex_unlock(&procesos_listos_mutex);

}

t_pcb* desencolar_proceso_listo() {

    pthread_mutex_lock(&procesos_listos_mutex);

    t_pcb* proceso = list_remove(cola_listos, 0);

    pthread_mutex_unlock(&procesos_listos_mutex);

  return proceso;
}

/* --------------- Funciones Procesos en Ejecución --------------- */

void encolar_proceso_en_ejecucion(t_pcb* proceso) {
 
    pthread_mutex_lock(&procesos_ejecutando_mutex);

    list_add(cola_ejecucion, proceso);

    pthread_mutex_unlock(&procesos_ejecutando_mutex);

}

t_pcb* desencolar_proceso_en_ejecucion() {

    pthread_mutex_lock(&procesos_ejecutando_mutex);

    t_pcb* proceso = list_remove(cola_ejecucion, 0);

    pthread_mutex_unlock(&procesos_ejecutando_mutex);

  return proceso;
}

/* --------------- Funciones Procesos Bloqueados --------------- */

void encolar_proceso_en_bloqueados(t_pcb* proceso) {

    pthread_mutex_lock(&procesos_bloqueados_mutex);

    list_add(cola_bloqueados, proceso);

    pthread_mutex_unlock(&procesos_bloqueados_mutex);

}

t_pcb* desencolar_proceso_bloqueado() {

    pthread_mutex_lock(&procesos_bloqueados_mutex);

    t_pcb* proceso = list_remove(cola_bloqueados, 0);

    pthread_mutex_unlock(&procesos_bloqueados_mutex);

  return proceso;
}

/* --------------- Funciones Procesos Suspendidos Bloqueados --------------- */

void encolar_proceso_en_suspendidos_bloqueados(t_pcb* proceso) {

    pthread_mutex_lock(&procesos_suspendidos_bloqueados_mutex);

    list_add(cola_suspendidos_bloqueados, proceso);

    pthread_mutex_unlock(&procesos_suspendidos_bloqueados_mutex);

}

t_pcb* desencolar_proceso_suspendido_bloqueado() {

    pthread_mutex_lock(&procesos_suspendidos_bloqueados_mutex);

    t_pcb* proceso = list_remove(cola_suspendidos_bloqueados, 0);

    pthread_mutex_unlock(&procesos_suspendidos_bloqueados_mutex);

  return proceso;
}

/* --------------- Funciones Procesos Suspendidos Listos --------------- */

void encolar_proceso_en_suspendidos_listos(t_pcb* proceso) {

    pthread_mutex_lock(&procesos_suspendidos_listos_mutex);

    list_add(cola_suspendidos_listos, proceso);

    pthread_mutex_unlock(&procesos_suspendidos_listos_mutex);

}

t_pcb* desencolar_proceso_suspendido_listo() {

    pthread_mutex_lock(&procesos_suspendidos_listos_mutex);

    t_pcb* proceso = list_remove(cola_suspendidos_listos, 0);
 
    pthread_mutex_unlock(&procesos_suspendidos_listos_mutex);

  return proceso;
}

/* --------------- Funciones Procesos Terminados --------------- */

void encolar_proceso_en_terminados(t_pcb* proceso) {

    pthread_mutex_lock(&procesos_terminados_mutex);

    list_add(cola_terminados, proceso);

    pthread_mutex_unlock(&procesos_terminados_mutex);

}

t_pcb* desencolar_proceso_terminado() {

    pthread_mutex_lock(&procesos_terminados_mutex);

    t_pcb* proceso = list_remove(cola_terminados, 0);

    pthread_mutex_unlock(&procesos_terminados_mutex);

  return proceso;
}