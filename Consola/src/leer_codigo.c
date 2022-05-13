#include "../include/leer_codigo.h"

static char* codigos_instrucciones[6] = {
    "NO_OP",
    "I/O",
    "READ",
    "WRITE",
    "COPY",
    "EXIT"
};

t_paquete* leer_archivo(char *path)
{
    FILE *codigo = fopen(path, "r");
    char *linea = (char*)malloc(LARGO_LINEA_MAX);
    //t_instruccion *inst;


    t_paquete *paquete = crear_paquete(NUEVO_PROCESO);
    while(!feof(codigo))
    {
        fgets(linea, LARGO_LINEA_MAX, codigo);
        if(strcmp(linea, "") != 0)
        {
            agregar_instruccion_a_paquete(linea, paquete);
            //inst = string_a_instruccion(linea);
            //agregar_a_paquete(paquete, inst, sizeof(t_instruccion));
            //free(inst);
        }
    }

    free(linea);
    fclose(codigo);

    return paquete;
}

void agregar_instruccion_a_paquete(char* linea, t_paquete* paquete) {
    t_instruccion *instruccion = malloc(sizeof(t_instruccion));

    char **palabras_linea = string_split(linea, " ");

    int op;
    for(op = 0; strcmp(palabras_linea[0], codigos_instrucciones[op]) != 0 && op < 6; op++);

    //if(op == 6)
    //{
        //ERROR: OPERACION NO VALIDA
        //return NULL;
    //}

    instruccion->op = op;
    switch(instruccion->op)
    {
        case NO_OP:
            instruccion->arg[0] = -1;
            instruccion->arg[1] = -1;
            //NOTA: en vez de enviar NO_OP 5 enviar 5 veces el codigo NO_OP? Yes
            for(int i = 0; i< atoi(palabras_linea[1]); i++) {
                agregar_a_paquete(paquete, instruccion, sizeof(t_instruccion));
            }
            free(palabras_linea[0]);
            free(palabras_linea[1]);
            break;

        case I_O:
            instruccion->arg[0] = atoi(palabras_linea[1]);
            instruccion->arg[1] = -1;
            free(palabras_linea[0]);
            free(palabras_linea[1]);
            agregar_a_paquete(paquete, instruccion, sizeof(t_instruccion));
            
            break;

        case READ:
            instruccion->arg[0] = atoi(palabras_linea[1]);
            instruccion->arg[1] = -1;
            free(palabras_linea[0]);
            free(palabras_linea[1]);
            agregar_a_paquete(paquete, instruccion, sizeof(t_instruccion));
            break;

        case WRITE:
            instruccion->arg[0] = atoi(palabras_linea[1]);
            instruccion->arg[1] = atoi(palabras_linea[2]);
            free(palabras_linea[0]);
            free(palabras_linea[1]);
            free(palabras_linea[2]);
            agregar_a_paquete(paquete, instruccion, sizeof(t_instruccion));
            break;

        case COPY:
            instruccion->arg[0] = atoi(palabras_linea[1]);
            instruccion->arg[1] = atoi(palabras_linea[2]);
            free(palabras_linea[0]);
            free(palabras_linea[1]);
            free(palabras_linea[2]);
            agregar_a_paquete(paquete, instruccion, sizeof(t_instruccion));
            break;

        case EXIT_INST:
            instruccion->arg[0] = -1;
            instruccion->arg[1] = -1;
            free(palabras_linea[0]);
            agregar_a_paquete(paquete, instruccion, sizeof(t_instruccion));

            //SIN ARGUMENTOS
            break;

        default:
            //ERROR: OPERACION NO VALIDA
            break;
    }

    
    free(palabras_linea);
}

/* VERSION ANTERIOR
t_paquete* leer_archivo(char *path)
{
    FILE *codigo = fopen(path, "r");
    char *linea = (char*)malloc(LARGO_LINEA_MAX);

    t_paquete *paquete = crear_paquete(NUEVO_PROCESO);
    
    while(!feof(codigo))
    {
        fgets(linea, LARGO_LINEA_MAX, codigo);
        agregar_a_paquete(paquete, linea, strlen(linea) + 1);
    }

    free(linea);
    fclose(codigo);

    return paquete;
}*/