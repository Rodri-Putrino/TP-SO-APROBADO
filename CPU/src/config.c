#include "../include/config.h"

void procesar_archivo_config_cpu(t_config* config_consola) {
    
    ip_escucha = config_get_string_value(config_consola, "IP_ESCUCHA");
    puerto_escucha_dispatch = config_get_string_value(config_consola, "PUERTO_ESCUCHA_DISPATCH");
    puerto_escucha_interrupt = config_get_string_value(config_consola, "PUERTO_ESCUCHA_INTERRUPT");
    ip_memoria = config_get_string_value(config_consola, "IP_MEMORIA");
    puerto_memoria = config_get_string_value(config_consola, "PUERTO_MEMORIA");
    //entradas_tlb = config_get_int_value(config_consola, "ENTRADAS_TLB");
    //reemplazo_tlb = config_get_string_value(config_consola, "REEMPLAZO_TLB");
    //retardo_noop = config_get_int_value(config_consola, "RETARDO_NOOP");

    printf("IP_ESCUCHA: %s\n", ip_escucha);
    printf("puerto_escucha_dispatch: %s\n", puerto_escucha_dispatch);
    printf("puerto_escucha_interrupt: %s\n", puerto_escucha_interrupt);
    printf("ip_memoria: %s\n", ip_memoria);
    printf("puerto_memoria: %s\n", puerto_memoria);
}