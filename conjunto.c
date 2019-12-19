#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conjunto.h"

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Estructura de las clases del conjunto cociente
 */
struct _Conjunto{
    int * cEstados;
    int numEstados;
}; 

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que reserva memoria suficiente para una clase
 */
Conjunto * conjunto_ini(AFND * p_afnd){
    
    Conjunto * c = NULL;
    int i = 0;

    c = (Conjunto*)calloc(1, sizeof(Conjunto));
    if(!c){
        fprintf(stderr, "Error a a hora de reservar memoria para un conjunto\n");
        return NULL;
    }

    /*Reservamos memoria para un conjunto de estados que siempre será el numero de estados del automata */
    c->cEstados = (int*)calloc(AFNDNumEstados(p_afnd), sizeof(int));
    if(!c->cEstados){
        fprintf(stderr, "Error a a hora de reservar memoria para los estados de un conjunto\n");
        return NULL;
    }

    for(i = 0; i < AFNDNumEstados(p_afnd); i++){
        c->cEstados[i] = -1;
    }

    c->numEstados = 0;

    return c;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que libera la memoria reservada para una clase
 */
void conjunto_liberar(Conjunto *c){

    if(!c) return;

    if(c->cEstados) free(c->cEstados);

    free(c);

    return;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que añade un indice de algun estado del automata a la clase
 */
int conjunto_set_cEstados(Conjunto * c, int estados){

    if(!c || estados < 0){
        fprintf(stderr, "Error, los parametros conjunto y estados son nulos\n");
        return -1;
    }

    if(esta_en_conjunto(c, estados) == TRUE) return -1;

    c->cEstados[c->numEstados] = estados;

    c->numEstados++;

    return 0;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que modifica el numero de esatdos de la clase
 */
Conjunto * conjunto_set_nEstados(Conjunto * c, int numEstados){

    if(!c || numEstados < 0){
        fprintf(stderr, "Error, los parametros conjunto y numEstados son nulos\n");
        return NULL;
    }

    c->numEstados = numEstados;

    return c;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que obtiene el numero de indices que tiene la clase
 */
int conjunto_get_nEstados(Conjunto * c){

    if(!c){
        fprintf(stderr, "Error, el parametro conjunto es nulo\n");
        return -1;
    }

    return c->numEstados;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que obtiene los indices de los estados que estan en la clase
 */
int * conjunto_get_cEstados(Conjunto * c){

    if(!c){
        fprintf(stderr, "Error, el parametro conjunto es nulo\n");
        return NULL;
    }

    return c->cEstados;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que comprueba si un indice ya esta en la clase
 */
Bool esta_en_conjunto(Conjunto *c, int indice){

    int i = 0;

    for ( i = 0; i < c->numEstados; i++){
        if(c->cEstados[i] == indice) return TRUE;
    }

    return FALSE;  
}
/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que imprime la clase
 */
int conjunto_print(Conjunto *c){

    int i = 0;

    if(!c) return -1;

    for(i = 0; i < c->numEstados ; i++){
        printf("%d ", c->cEstados[i]);
    }
    printf("\n");

    return 0;
}

int conjunto_print_nombre(Conjunto *c, AFND * afnd){

    int i = 0;

    if(!c) return -1;

    printf("{");
    for(i = 0; i < c->numEstados ; i++){
        
        if(i == c->numEstados-1){
            printf("%s", AFNDNombreEstadoEn(afnd, c->cEstados[i]));
        }
        else{
            printf("%s,", AFNDNombreEstadoEn(afnd, c->cEstados[i]));
        }
    }
    printf("}");

    return 0;
}





