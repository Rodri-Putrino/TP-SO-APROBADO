#ifndef HEADERS_PLANIFICADOR_LARGO_PLAZO_H_
#define HEADERS_PLANIFICADOR_LARGO_PLAZO_H_

#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include "./config.h"
#include "../../shared/include/shared_log.h"
#include "../../shared/include/shared_utils.h"
#include <commons/collections/list.h>

void iniciar_planificador_largo_plazo();
void controlar_grado_de_multiprogramacion();
void finalizar_planificador_largo_plazo();
bool grado_de_multiprogramacion_completo();

#endif /* HEADERS_PLANIFICADOR_LARGO_PLAZO_H_ */