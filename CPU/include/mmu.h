#ifndef MMU
#define MMU

#include "../../shared/include/shared_utils.h"
#include "../../shared/include/shared_mensajes.h"
#include <commons/collections/list.h>
#include <math.h>

int traducir_dir_logica(int dir, t_pcb *proceso, t_log *logger);
int exceso_de_dato_en_pagina(int dir_logica);
int resto_pagina(dir_logica);
void pedido_escritura(int valor, int dir_logica, t_pcb *proceso, t_log *logger);
int max(int a,int b);

#endif