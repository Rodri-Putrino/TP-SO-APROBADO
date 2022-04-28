#include "../include/leer_codigo.h"

t_instruccion* string_a_instruccion(char* linea)
{
    t_instruccion *ret = malloc(sizeof(t_instruccion));

    char **palabras_linea = string_split(linea, " ");

    int op;
    for(op = 0; strcmp(palabras_linea[0], codigos_instrucciones[op]) != 0 && op < 6; op++);

    //if(op == 6)
    //{
        //ERROR: OPERACION NO VALIDA
        //return NULL;
    //}

    ret->op = op;
    switch(ret->op)
    {
        case NO_OP:
            ret->arg[0] = atoi(palabras_linea[1]);
            ret->arg[1] = -1;
            //NOTA: en vez de enviar NO_OP 5 enviar 5 veces el codigo NO_OP? Yes
            free(palabras_linea[0]);
            free(palabras_linea[1]);
            break;

        case I_O:
            ret->arg[0] = atoi(palabras_linea[1]);
            ret->arg[1] = -1;
            free(palabras_linea[0]);
            free(palabras_linea[1]);
            break;

        case READ:
            ret->arg[0] = atoi(palabras_linea[1]);
            ret->arg[1] = -1;
            free(palabras_linea[0]);
            free(palabras_linea[1]);
            break;

        case WRITE:
            ret->arg[0] = atoi(palabras_linea[1]);
            ret->arg[1] = atoi(palabras_linea[2]);
            free(palabras_linea[0]);
            free(palabras_linea[1]);
            free(palabras_linea[2]);
            break;

        case COPY:
            ret->arg[0] = atoi(palabras_linea[1]);
            ret->arg[1] = atoi(palabras_linea[2]);
            free(palabras_linea[0]);
            free(palabras_linea[1]);
            free(palabras_linea[2]);
            break;

        case EXIT_INST:
            ret->arg[0] = -1;
            ret->arg[1] = -1;
            free(palabras_linea[0]);
            //SIN ARGUMENTOS
            break;

        default:
            //ERROR: OPERACION NO VALIDA
            break;
    }

    free(palabras_linea);
    return ret;
}

t_paquete* leer_archivo(char *path)
{
    FILE *codigo = fopen(path, "r");
    char *linea = (char*)malloc(LARGO_LINEA_MAX);
    t_instruccion *inst;


    t_paquete *paquete = crear_paquete(NUEVO_PROCESO);
    while(!feof(codigo))
    {
        fgets(linea, LARGO_LINEA_MAX, codigo);
        if(strcmp(linea, "") != 0)
        {
            inst = string_a_instruccion(linea);
            agregar_a_paquete(paquete, inst, sizeof(t_instruccion));
            free(inst);
        }
    }

    free(linea);
    fclose(codigo);

    return paquete;
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