#ifndef SHARED_UTILS_H
#define SHARED_UTILS_H

#include <stdio.h>
#include <commons/log.h>
#include "./shared_config.h"
#include <stdbool.h>

void finalizar_programa(t_log*, t_config*);
void destruir_logger(t_log*);

#endif