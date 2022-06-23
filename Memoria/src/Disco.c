#include "../include/Disco.h"


void generar_nuevo_archivo(int PID){
    
    struct stat buffer;
    //int exist = stat(filename,&buffer);
    
    if(stat(path_swap,&buffer)!=0){
        log_error(logger,"El directorio %s no existe",path_swap);

        system("mkdir /home/utnso/swap");

        //mkdir (char *filename);
    }    


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

void suspender_paginas(int PID, int dir_tablaN1)
{
    t_list *marcos = conseguir_marcos_proceso(dir_tablaN1);
    for(int i = 0; i < list_size(marcos); i++)
    {
        entrada_tabla_N2 *e = list_get(marcos, i);
        if(e->bit_modificacion == 1)
        {
            escribir_en_archivo(PID, e->dir, e->num_pag);
            log_info(logger, "se guardaron los cambios de la pagina %d correctamente del proceso %d",e->num_pag,PID);
        }
            
        
        e->bit_modificacion = 0;
        e->bit_presencia = 0;
    }
    list_destroy(marcos);
}

void escribir_en_archivo(int PID, int direccion_pagina, int pagina){

    char * path = devolver_path_archivo(PID);

    FILE * file;
    file = fopen(path, "r+");
    
    fseek(file,tam_pagina*pagina,SEEK_SET);

    fwrite(memoria + direccion_pagina,tam_pagina,1,file);    
    
    fclose(file);
    free(path);
} //TODO: testear cuando podamos cargar cosas en memoria :D

void enviar_pagina_a_memoria(int PID, int direccion_pagina, int pagina){

    char * path = devolver_path_archivo(PID);

    FILE * file;

    file = fopen(path, "r");

    fseek(file,tam_pagina*pagina,SEEK_SET);

    fread(memoria + direccion_pagina,tam_pagina,1,file);

    log_info(logger,"pagina %d enviada a memoria correctamente",pagina);

    fclose(file);
    free(path);
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

