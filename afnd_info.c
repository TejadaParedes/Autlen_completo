#include "afnd_info.h"

struct _AFND_INFO{  
    char **simbolos;
    int * i_simbolos;
    int n_simbolos;
    char ** estados;
    int * i_estados;
    int * t_estados;
    int n_estados;
};

/**
 * Funcion que reserva memoria para la estructra que almacena toda la información de la info. 
 */
AFND_INFO *afdn_info_ini(){
    AFND_INFO *info = NULL;

    info = calloc(1, sizeof(AFND_INFO));
    if(!info){
        fprintf(stderr, "Error a la hora de resevar memoria para la info\n");
        return NULL;
    }

    return info;
}

/**
 * Funcion que libera la memoria para la estructra que almacena toda la información de la info. 
 */
void afdn_info_destroy(AFND_INFO *info){

    if(!info) return;

    free(info->simbolos);
    free(info->i_simbolos);
    free(info->estados);
    free(info->i_estados);
    free(info->t_estados);
    free(info);

    return;
}

/**
 * Metodo que obtinene toda la información que almacena el automata. 
 */
AFND_INFO *get_AFDN_INFO(AFND *afnd, AFND_INFO *info){

    int i = 0;

    if(!info || !afnd) return NULL;

    /*-------------------------Simbolos-------------------------------------- */

    info->n_simbolos = AFNDNumSimbolos(afnd);

    info->simbolos = (char**)malloc(info->n_simbolos*sizeof(char*));
    if(!info->simbolos){
        printf("Error al reservar memoria: afd->simbolos");
        return NULL;
    }

    info->i_simbolos = (int*)malloc(info->n_simbolos*sizeof(int));

    if(!info->i_simbolos){
        printf("Error al reservar memoria: afd->i_simbolos");
        return NULL;
    }

    for (i = 0; i < info->n_simbolos; i++) {
        info->simbolos[i] = AFNDSimboloEn(afnd,i);
        info->i_simbolos[i] = AFNDIndiceDeSimbolo(afnd, AFNDSimboloEn(afnd,i));
    }

    /*-------------------------Estados-------------------------------------*/

    info->n_estados = AFNDNumEstados(afnd);

    info->estados = (char**)malloc(info->n_estados*sizeof(char*));
        if(!info->estados){
        printf("Error al reservar memoria: afd->estados");
        return NULL;
    }

    info->i_estados = (int*)malloc(info->n_estados*sizeof(int));
        if(!info->i_estados){
        printf("Error al reservar memoria: afd->i_estados");
        return NULL;
    }

    info->t_estados = (int*)malloc(info->n_estados*sizeof(int));
        if(!info->t_estados){
        printf("Error al reservar memoria: afd->t_estados");
        return NULL;
    } 

    for (i = 0; i < info->n_estados; i++) {
        info->estados[i] = AFNDNombreEstadoEn(afnd,i);
        info->i_estados[i] = AFNDIndiceDeEstado(afnd, AFNDNombreEstadoEn(afnd,i));
        info->t_estados[i] = AFNDTipoEstadoEn(afnd, i);
    }

 return info;   
}

char **info_get_simbolos(AFND_INFO *info){
    
    if(!info){
        fprintf(stderr, "Error en el metodo info_get_simbolos\n");
        return NULL;
    }
    
    return info->simbolos;
}

int *info_get_i_simbolos(AFND_INFO *info){

    if(!info){
        fprintf(stderr, "Error en el metodo info_get_i_simbolos\n");
        return NULL;
    }

    return info->i_simbolos;
}

int info_get_n_simbolos(AFND_INFO *info){

    if(!info){
        fprintf(stderr, "Error en el metodo info_get_n_simbolos\n");
        return -1;
    }

    return info->n_simbolos;
}

char **info_get_estados(AFND_INFO *info){
    
    if(!info){
        fprintf(stderr, "Error en el metodo info_get_estados\n");
        return NULL;
    }
    
    return info->estados;
}

int *info_get_i_estados(AFND_INFO *info){
    
    if(!info){
        fprintf(stderr, "Error en el metodo info_get_i_estados\n");
        return NULL;
    }
    
    return info->i_estados;
}

int *info_get_t_estados(AFND_INFO *info){
    
    if(!info){
        fprintf(stderr, "Error en el metodo info_get_t_estados\n");
        return NULL;
    }
    
    return info->t_estados;
}

int info_get_n_estados(AFND_INFO *info){
    
    if(!info){
        fprintf(stderr, "Error en el metodo info_get_n_estados\n");
        return -1;
    }
    
    return info->n_estados;
}