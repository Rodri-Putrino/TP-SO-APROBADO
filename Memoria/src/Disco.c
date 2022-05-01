#include "../include/Disco.h"


void generar_nuevo_archivo(int PID){
    
    char * aux  = string_new();
    char * idAux = string_itoa(PID);

    string_append(&aux, path_swap);
    string_append(&aux, "/");
    string_append(&aux, idAux);
    string_append(&aux, ".swap.txt");

    FILE * new_file;

    new_file = fopen(aux,"w+");

    log_info(logger, "se genero el archivo: %s\n", aux);

    free(idAux);
    free(aux);
    fclose(new_file);
}

void eliminar_archivo(int PID){

    char* aux = devolver_path_archivo(PID);

    if(remove(aux)==0){
        log_info(logger, "el archivo %s fue eliminado correctamente", aux);
    }
    else{
        log_error(logger, "el archivo no se pudo eliminar");
    }

    free(aux);
}


//RECORDAR LIBERAR AUX
char* devolver_path_archivo(int PID){

    char * aux = string_new();
    char * idAux = string_itoa(PID);


    string_append(&aux, path_swap);
    string_append(&aux, "/");
    string_append(&aux, idAux);
    string_append(&aux, ".swap.txt");

    free(idAux);
    return aux;  //TODO ver como liberar aux y poder devolverlo
}


