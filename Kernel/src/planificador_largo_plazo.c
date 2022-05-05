#include "../include/planificador_largo_plazo.h"

static pthread_t planificador_largo_plazo;

void iniciar_planificador_largo_plazo() {

  pthread_create(&planificador_largo_plazo, NULL, (void*) controlar_grado_de_multiprogramacion, NULL);
  pthread_detach(planificador_largo_plazo);
}

void controlar_grado_de_multiprogramacion() {

  log_info(logger, "Planificador de largo plazo iniciado");

  /*while (1) {

    //sem_wait(&sem_proceso_nuevo);

    if (!grado_multiprogramacion_completo()) {

      mover_proceso_a_listo();

    } else if (grado_de_multiprogramacion_ocupado_por_procesos_bloqueados()) {

      sem_post(&sem_grado_multiprogramacion_completo);
      sem_wait(&sem_proceso_suspendido);
      sem_post(&sem_proceso_nuevo);

    }
  }*/
}
