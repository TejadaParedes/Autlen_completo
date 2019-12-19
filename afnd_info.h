#ifndef AFND_INFO_H
#define AFND_INFO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "afnd.h"
#include "types.h"

typedef struct _AFND_INFO AFND_INFO;


AFND_INFO *afdn_info_ini();

/**
 * Funcion que libera la memoria para la estructra que almacena toda la información de la info. 
 */
void afdn_info_destroy(AFND_INFO *info);

/**
 * Metodo que obtinene toda la informacio que almacena el automata. 
 */
AFND_INFO *get_AFDN_INFO(AFND *afnd, AFND_INFO *info);

char **info_get_simbolos(AFND_INFO *info);

int *info_get_i_simbolos(AFND_INFO *info);

int info_get_n_simbolos(AFND_INFO *info);

char **info_get_estados(AFND_INFO *info);

int *info_get_i_estados(AFND_INFO *info);

int *info_get_t_estados(AFND_INFO *info);

int info_get_n_estados(AFND_INFO *info);







#endif
