#include "../include/leer_codigo.h"

char* leer_instruccion(FILE *f)
{
    char *linea = (char*)malloc(LARGO_LINEA_MAX);
    int i = 0;
    char c = fgetc(f);
    while(c != '\n' && c != EOF)
    {
        linea[i] = c;
        i++;
        c = fgetc(f);
    }

    linea[i + 1] = '\0';

    string_trim_right(&linea);

    return linea;
}

t_paquete* leer_archivo(char *path)
{
    FILE *codigo = fopen(path, "r");

    t_paquete *p = crear_paquete();

    while(!feof(codigo))
    {
        char *instruccion = leer_instruccion(codigo);
        agregar_a_paquete(p, instruccion, string_length(instruccion) +1);
    }
    fclose(codigo);

    return p;
}