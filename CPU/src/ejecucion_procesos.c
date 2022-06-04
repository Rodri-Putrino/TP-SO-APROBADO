#include "../include/ejecucion_procesos.h"

void realizar_ciclo_de_instruccion(t_pcb* pcb, int conexion_kernel) { 

    t_instruccion* instruccion = buscar_proxima_instruccion(pcb);
    interpretar_instruccion_y_ejecutar_pcb(instruccion, pcb, conexion_kernel);

    // while(instruccion != EXIT)
    // {
    //      if(instruccion == COPY)
    //      buscar_valor_en_memoria();
    //      ejecutar()
    //      incrementar_program_counter();
    //      hay_interrupcion_para_atender()
    //      instruccion = buscar_proxima_instruccion();
    //      interpretar_instruccion_a_ejecutar(instruccion);
    // }
}

t_instruccion* buscar_proxima_instruccion(t_pcb* pcb) { 
    log_info(logger_CPU, "Etapa FETCH iniciada");
    
    t_instruccion* instruccion = list_get(pcb->instrucciones, pcb->program_counter);

    pcb->program_counter += 1;

    return instruccion;
}

void interpretar_instruccion_y_ejecutar_pcb(t_instruccion* instruccion, t_pcb* pcb, int conexion_kernel) {
    log_info(logger_CPU, "Etapa DECODE iniciada");

    switch(instruccion->op) {
        case NO_OP:

            log_info(logger_CPU, "Instruccion NO_OP");
            log_info(logger_CPU, "Etapa EXECUTE iniciada");

            usleep(retardo_noop * 1000);

            if(hay_interrupcion_para_atender()) {
                enviar_pcb(ACTUALIZAR_PCB, pcb, conexion_kernel, logger_CPU);

                liberar_conexion(conexion_kernel);
            }
            else {
                realizar_ciclo_de_instruccion(pcb, conexion_kernel);
            }

            break;

        case I_O:

            log_info(logger_CPU, "Instruccion I_O");
            log_info(logger_CPU, "Etapa EXECUTE iniciada");

            int segundos_bloqueado = instruccion->arg[0];
            enviar_pcb_con_tiempo_bloqueo(IO, pcb, segundos_bloqueado, conexion_kernel, logger_CPU);

            liberar_conexion(conexion_kernel);
         
            break;

        case READ:
         
            log_info(logger_CPU, "Instruccion READ");
            log_info(logger_CPU, "Etapa EXECUTE iniciada");

            //Pasar a mmu:
            //pedido_lectura()

            if(hay_interrupcion_para_atender())
            {
                //devolver PCB
            }
            else 
                realizar_ciclo_de_instruccion(pcb, conexion_kernel);

            break;

        case WRITE:
         
            log_info(logger_CPU, "Instruccion WRITE");
            log_info(logger_CPU, "Etapa EXECUTE iniciada");

            int dir_logica = instruccion->arg[0];
            int valor = instruccion->arg[1];
            //Pasar a mmu:
            //pedido_escritura(valor, dir_logica, pcb, logger_CPU) //Sacar logger pq es global

            if(hay_interrupcion_para_atender())
            {
                //devolver PCB
            }
            else 
                realizar_ciclo_de_instruccion(pcb, conexion_kernel);

            break;

        case COPY:
         
            log_info(logger_CPU, "Instruccion COPY");
            log_info(logger_CPU, "Etapa FETCH OPERANDS iniciada");
            //buscar_valor_en_memoria();
            log_info(logger_CPU, "Etapa EXECUTE iniciada");

            if(hay_interrupcion_para_atender())
            {
                //devolver PCB
            }
            else 
                realizar_ciclo_de_instruccion(pcb, conexion_kernel);

            break;

        case EXIT:
         
            log_info(logger_CPU, "Instruccion EXIT");
            log_info(logger_CPU, "Etapa EXECUTE iniciada");

            enviar_pcb(EXIT, pcb, conexion_kernel, logger_CPU);

            //list_destroy_and_destroy_elements(pcb, free);
            liberar_conexion(conexion_kernel);

            break;
        
        default:
            log_error(logger_CPU, "Instruccion inválida");
    }
}

int hay_interrupcion_para_atender() {
    return 0; //TODO
}