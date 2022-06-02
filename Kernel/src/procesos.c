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

    sem_init(&sem_multiprogramacion, 0, grado_multiprogramacion);
    sem_init(&sem_proceso_nuevo, 0, 0);
    sem_init(&sem_proceso_listo, 0, 0);
    sem_init(&sem_proceso_suspendido_listo, 0, 0);
    sem_init(&sem_proceso_a_suspender,0,0);
}

t_pcb* crear_proceso(uint32_t id, uint32_t tam, t_list* lista_instrucciones) {
    
    pthread_mutex_lock(&proceso_mutex);

    t_pcb* pcb_nuevo = malloc(sizeof(t_pcb));

    pcb_nuevo->id = 5; //TODO igualar al id recibido por parámetro
    pcb_nuevo->tam_proceso = tam;
    pcb_nuevo->instrucciones = list_create();
    pcb_nuevo->program_counter = 0;
    //pcb_nuevo->tabla_paginas = ...
    pcb_nuevo->estimacion_anterior = estimacion_inicial;
    pcb_nuevo->ultima_rafaga = 0;
    pcb_nuevo->rafaga = malloc(sizeof(rango_tiempo_t));
    pcb_nuevo->tiempo_bloqueado = malloc(sizeof(rango_tiempo_t));

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

    sem_post(&sem_proceso_nuevo);
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

    sem_post(&sem_proceso_listo);
}

t_pcb* desencolar_proceso_listo() {

    pthread_mutex_lock(&procesos_listos_mutex);

    t_pcb* proceso = list_remove(cola_listos, 0);

    pthread_mutex_unlock(&procesos_listos_mutex);

  return proceso;
}

void ordenar_cola_listos() {

	//list_iterate(cola_listos, (void*) iterator);

    pthread_mutex_lock(&procesos_listos_mutex);

    list_sort(cola_listos, (void*)mayor_prioridad);

    //list_iterate(cola_listos, (void*) actualizar_estimacion_anterior);

    pthread_mutex_unlock(&procesos_listos_mutex);
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

int hay_proceso_en_ejecucion() {

    pthread_mutex_lock(&procesos_ejecutando_mutex);

    int resultado = (int) list_is_empty(cola_ejecucion);
 
    pthread_mutex_unlock(&procesos_ejecutando_mutex);

    return resultado;
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

int hay_proceso_suspendido_listo() {

    pthread_mutex_lock(&procesos_suspendidos_listos_mutex);

    int resultado = (int) list_is_empty(cola_suspendidos_listos);
 
    pthread_mutex_unlock(&procesos_suspendidos_listos_mutex);

    return !resultado;
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

/* --------------- Funciones Generales --------------- */

int cantidad_procesos_en_sistema() {

    int cantidad_listos, cantidad_ejecutando, cantidad_bloqueados, cantidad_total;

    pthread_mutex_lock(&procesos_listos_mutex);
    pthread_mutex_lock(&procesos_ejecutando_mutex);
    pthread_mutex_lock(&procesos_bloqueados_mutex);

    cantidad_listos = list_size(cola_listos);
    cantidad_ejecutando = list_size(cola_ejecucion);
    cantidad_bloqueados = list_size(cola_bloqueados);

    pthread_mutex_unlock(&procesos_listos_mutex);
    pthread_mutex_unlock(&procesos_ejecutando_mutex);
    pthread_mutex_unlock(&procesos_bloqueados_mutex);

    cantidad_total = cantidad_listos + cantidad_ejecutando + cantidad_bloqueados;

    log_info(logger, "Cantidad de procesos en sistema: %d", cantidad_total);

    return cantidad_total;
}

void proceso_iniciar_rafaga(t_pcb *pcb) {
    gettimeofday(&pcb->rafaga->inicio, NULL);
}

void proceso_finalizar_rafaga(t_pcb* pcb) {
    gettimeofday(&pcb->rafaga->fin, NULL);
    pcb->ultima_rafaga = timedifference_msec(pcb->rafaga->inicio, pcb->rafaga->fin);
    //printf("\n\nTiempo de ráfaga: %d\n\n", pcb->ultima_rafaga);
}

void actualizar_estimacion_anterior(t_pcb* pcb)
{
	pcb->estimacion_anterior = proxima_rafaga_estimada(pcb);
}

int proxima_rafaga_estimada(t_pcb* pcb) {

  // Fórmula SJF	pr = ur * alpha + (1 - alpha) * t estimado ur

  return pcb->ultima_rafaga * alfa + (1 - alfa) * pcb->estimacion_anterior;
}

int mayor_prioridad(t_pcb *pcb1, t_pcb *pcb2) {
    return proxima_rafaga_estimada(pcb1) <= proxima_rafaga_estimada(pcb2);
}

int puede_suspenderse(t_pcb* pcb) {

    int tiempo_bloqueado;
    //tiempo_bloqueado = timedifference_msec(pcb->tiempo_bloqueado->inicio, pcb->tiempo_bloqueado->fin);
   //return tiempo_bloqueado > tiempo_max_bloqueado;
   return 1;
}

float timedifference_msec(struct timeval t0, struct timeval t1) {
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}