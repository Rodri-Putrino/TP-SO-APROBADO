#include "../include/mmu.h"

int traducir_dir_logica(int dir, t_pcb *proceso, t_log *logger, int socket_memoria)
{
	int numero_pagina = floor(dir / tam_pagina);

	//BUSCAR EN TLB
	int result_tlb = buscar_pagina_tlb(numero_pagina);
	//SI NO ES PAGE FAULT, RETORNAR RESULTADO
	if(result_tlb != -1)
	{
		log_info(logger,"TLB Hit");
		return result_tlb;
	}
	log_info(logger, "TLB Miss");

	int dir_tablaN1 = proceso->tabla_paginas;
	log_info(logger,"Dir tabla de proceso: %d", proceso->tabla_paginas);
	int dir_entradaN1 = floor(numero_pagina / paginas_por_tabla);

	//(SOLICITUD TABLA)
	//ENVIAR DIR TABLA N1 Y NUM ENTRADA TABLA NIVEL 1
	t_paquete *primer_acceso = crear_paquete(SOLICITUD_TABLA_PAGINAS);
	agregar_a_paquete(primer_acceso, &dir_tablaN1, sizeof(int));
	agregar_a_paquete(primer_acceso, &dir_entradaN1, sizeof(int));
	enviar_paquete(primer_acceso, socket_memoria, logger);
	eliminar_paquete(primer_acceso);

	//RECIBIR TABLA N2
	recibir_operacion(socket_memoria);
	t_tablaN2 *tablaN2 = recibir_tabla_N2(socket_memoria, logger);

	for(int i = 0; i < list_size(tablaN2); i++)
	{
		entrada_tabla_N2 *e = list_get(tablaN2, i);
		log_info(logger,"Entrada num: %d\nDir pag: %d\nBit presencia: %d",
		e->num_pag,
		e->dir,
		e->bit_presencia
		);
	}

	int dir_entradaN2 = numero_pagina % paginas_por_tabla;
	entrada_tabla_N2 *e2 = list_get(tablaN2, dir_entradaN2);
	
	//(SOLICITUD MARCO)
	//ENVIAR DIR PAGINA
	t_paquete *segundo_acceso = crear_paquete(SOLICITUD_MARCO);
	agregar_a_paquete(segundo_acceso, &(proceso->id), sizeof(int));
	agregar_a_paquete(segundo_acceso, e2, sizeof(entrada_tabla_N2));
	enviar_paquete(primer_acceso, socket_memoria, logger);
	//SI BIT PRESENCIA == 0 → TRAER PAGINA
	//RECIBIR DIR MARCO
	recibir_operacion(socket_memoria);
	t_list *datos2 = recibir_paquete(socket_memoria, logger);

	int *dir_marco = list_get(datos2, 0);
	//NO HUBO TLB HIT, ENTONCES LA AGREGA
	if(result_tlb == -1)
		agregar_entrada_tlb(e2);

	int desplazamiento = *dir_marco - numero_pagina * tam_pagina;
	return (*dir_marco) + desplazamiento;
}

int exceso_de_dato_en_pagina(int dir_logica)
{
	int desplazamiento = dir_logica % tam_pagina;
	//RETORNA EXCESO O CERO
	return max(desplazamiento + sizeof(int) - tam_pagina, 0);
}

int resto_pagina(int dir_logica)
{
	return tam_pagina - (dir_logica % tam_pagina);
}

void *resto_dato(int *dato, int bytes_por_procesar)
{
	int desplazamiento = sizeof(int) - bytes_por_procesar;
	return (dato + desplazamiento);
}

void pedido_escritura(int valor, int dir_logica, t_pcb *proceso, t_log *logger, int socket_memoria)
{
	int resto_pag = resto_pagina(dir_logica);
	int bytes_por_procesar = sizeof(int);
	while(bytes_por_procesar > 0)
	{
		int dir_fisica = traducir_dir_logica(dir_logica, proceso, logger, socket_memoria);
		if(resto_pag >= bytes_por_procesar)
		{
			//ENVIAR DIR CON PEDIDO Y TAMAÑO bytes_por_procesar
			t_paquete *pedido = crear_paquete(PEDIDO_ESCRITURA);
			agregar_a_paquete(pedido, &dir_fisica, sizeof(int));
			agregar_a_paquete(pedido, &bytes_por_procesar, sizeof(int));
			agregar_a_paquete(pedido, resto_dato(&valor, bytes_por_procesar), bytes_por_procesar);
			
			enviar_paquete(pedido, socket_memoria, logger);
			eliminar_paquete(pedido);
			bytes_por_procesar = 0;
		}
		else
		{
			//ENVIAR DIR CON PEDIDO Y TAMANIO resto_pag
			t_paquete *pedido = crear_paquete(PEDIDO_ESCRITURA);
			agregar_a_paquete(pedido, &dir_fisica, sizeof(int));
			agregar_a_paquete(pedido, &resto_pag, sizeof(int));
			agregar_a_paquete(pedido, resto_dato(&valor, bytes_por_procesar), resto_pag);

			enviar_paquete(pedido, socket_memoria, logger);
			eliminar_paquete(pedido);
			bytes_por_procesar -= resto_pag;
			resto_pag = resto_pagina(dir_logica + resto_pag);
		}
	}
}

int pedido_lectura(int dir_logica, t_pcb *proceso, t_log *logger, int socket_memoria)
{
	int resto_pag = resto_pagina(dir_logica);
	int bytes_por_procesar = sizeof(int);

	int *dato = malloc(sizeof(int));
	int desplazamiento = 0;
	while(bytes_por_procesar > 0) {
		int dir_fisica = traducir_dir_logica(dir_logica, proceso, logger, socket_memoria);
		if(resto_pag >= bytes_por_procesar) {
			//ENVIAR DIR CON PEDIDO Y TAMAÑO bytes_por_procesar
			t_paquete *pedido = crear_paquete(PEDIDO_ESCRITURA);
			agregar_a_paquete(pedido, &dir_fisica, sizeof(int));
			agregar_a_paquete(pedido, &bytes_por_procesar, sizeof(int));

			//RECIBIR VALOR
			recibir_operacion(socket_memoria);
			t_list *respuesta = recibir_paquete(socket_memoria, logger);
			void *aux = list_get(respuesta, 0);
			memcpy(dato + desplazamiento, aux, bytes_por_procesar);

			bytes_por_procesar = 0;
		}
		else {
			//ENVIAR DIR CON PEDIDO Y TAMANIO resto_pag
			t_paquete *pedido = crear_paquete(PEDIDO_ESCRITURA);
			agregar_a_paquete(pedido, &dir_fisica, sizeof(int));
			agregar_a_paquete(pedido, &resto_pag, sizeof(int));

			//RECIBIR VALOR
			recibir_operacion(socket_memoria);
			t_list *respuesta = recibir_paquete(socket_memoria, logger);
			void *aux = list_get(respuesta, 0);
			memcpy(dato + desplazamiento, aux, resto_pag);

			desplazamiento += resto_pag;
			bytes_por_procesar -= resto_pag;
			resto_pag = resto_pagina(dir_logica + resto_pag);
		}
	}
	int ret = *dato;
	free(dato);
	return ret;
}

int max(int a,int b) {
    if(a > b)
        return a;
    return b;
}