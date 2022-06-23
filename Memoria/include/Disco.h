#ifndef HEADERS_DISCO_H_
#define HEADERS_DISCO_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
//#include <conio.h>
#include <sys/types.h>
#include "./config.h"
#include "./server.h"
#include "./crear_estructuras.h"
#include "../../shared/include/shared_log.h"
#include "../../shared/include/shared_utils.h"
#include "../../shared/include/shared_mensajes.h"
#include "../../shared/include/shared_servidor.h"


void generar_nuevo_archivo(int);
void eliminar_archivo(int);
char* devolver_path_archivo(int);
void escribir_en_archivo(int PID, int direccion_pagina, int pagina);
void enviar_pagina_a_memoria(int PID, int direccion_pagina, int pagina);
void suspender_paginas(int PID, int dir_tablaN1);

#endif /* HEADERS_DISCO_H_ */