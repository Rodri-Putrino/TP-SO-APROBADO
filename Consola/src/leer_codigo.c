#include "../include/leer_codigo.h"

char* leer_instruccion(FILE *f)
{
    char *linea = (char*)malloc(LARGO_LINEA_MAX);
    int i = 0;

    /*char c = fgetc(f);
    while(c != '\n' && c != EOF)
    {
        linea[i] = c;
        i++;
        c = fgetc(f);
    }

    linea[i + 1] = '\0';*/

    //string_trim_right(&linea);

    return linea;
}

t_paquete* leer_archivo(char *path)
{
    FILE *codigo = fopen(path, "r");
    char *linea = (char*)malloc(LARGO_LINEA_MAX);

    t_paquete *paquete = crear_paquete();

    fgets(linea, LARGO_LINEA_MAX, codigo);
    
    while(!feof(codigo))
    {
        agregar_a_paquete(paquete, linea, strlen(linea) + 1);
        fgets(linea, LARGO_LINEA_MAX, codigo);
        printf("Linea: %s\n", linea);
    }

    free(linea);
    fclose(codigo);

    return paquete;
}