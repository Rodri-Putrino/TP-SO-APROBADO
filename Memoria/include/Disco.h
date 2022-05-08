#ifndef HEADERS_MAIN_H_
#define HEADERS_MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include "./config.h"
#include "./server.h"
#include "../../shared/include/shared_log.h"
#include "../../shared/include/shared_utils.h"
#include "../../shared/include/shared_mensajes.h"
#include "../../shared/include/shared_servidor.h"

void generar_nuevo_archivo(int);
void eliminar_archivo(int);
char* devolver_path_archivo(int);

#endif /* HEADERS_MAIN_H_ */