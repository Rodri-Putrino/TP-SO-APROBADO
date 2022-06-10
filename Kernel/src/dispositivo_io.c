#include "../include/dispositivo_io.h"

static pthread_t dispositivo_io;

void iniciar_dispositivo_io() {

    pthread_create(&dispositivo_io, NULL, (void*) atender_procesos, NULL);
    pthread_detach(dispositivo_io);
}

void atender_procesos() {

    while (1) 
    {
        sem_wait(&sem_proceso_bloqueado);

        t_pcb* pcb = desencolar_proceso_bloqueado();
        //log_info(logger, "Tiempo a bloquearse: %u", tiempo_bloqueo);
        //usleep(tiempo_bloqueo * 1000);
    }
}