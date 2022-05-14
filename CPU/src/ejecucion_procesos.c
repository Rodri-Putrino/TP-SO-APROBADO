#include "../include/ejecucion_procesos.h"

void realizar_ciclo_de_instruccion(t_list* pcb, int conexion_kernel) { //TODO Modificar a t_pcb*

    int instruccion = buscar_proxima_instruccion(pcb);
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

int buscar_proxima_instruccion(t_list* pcb) { //TODO Modificar a t_pcb*
    log_info(logger_CPU, "Etapa FETCH iniciada");

    return 5;
}

void interpretar_instruccion_y_ejecutar_pcb(int instruccion, t_list* pcb, int conexion_kernel) {
    log_info(logger_CPU, "Etapa DECODE iniciada");

    switch(instruccion) 
    {
        case NO_OP:

            log_info(logger_CPU, "Instruccion NO_OP");
            log_info(logger_CPU, "Etapa EXECUTE iniciada");

            if(hay_interrupcion_para_atender())
            {
                //devolver PCB
            }
            else 
                realizar_ciclo_de_instruccion(pcb, conexion_kernel);

            break;

        case I_O:

            log_info(logger_CPU, "Instruccion I_O");
            log_info(logger_CPU, "Etapa EXECUTE iniciada");

            if(hay_interrupcion_para_atender())
            {
                //devolver PCB
            }
            else 
                realizar_ciclo_de_instruccion(pcb, conexion_kernel); //En realidad no va, sólo para test
         
            break;

        case READ:
         
            log_info(logger_CPU, "Instruccion READ");
            log_info(logger_CPU, "Etapa EXECUTE iniciada");

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

            sleep(3);

            t_paquete* paquete = crear_paquete(EXIT);
            agregar_a_paquete(paquete, pcb, sizeof(t_pcb));
            enviar_paquete(paquete, conexion_kernel, logger_CPU);
            eliminar_paquete(paquete);

            list_destroy_and_destroy_elements(pcb, free);
            liberar_conexion(conexion_kernel);

            break;
        
        default:
            log_error(logger_CPU, "Instruccion inválida");
    }
}

bool hay_interrupcion_para_atender() {
    return false; //TODO
}