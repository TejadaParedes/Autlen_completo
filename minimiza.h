#ifndef AFD_H
#define AFD_H

#include <stdio.h>
#include "afnd.h"
#include "afnd_info.h"
#include "types.h"
#include "conjunto.h"

typedef struct _AA AA;


AFND * AFNDMinimiza(AFND * afnd);

int *estados_accesibles(AFND_INFO *info, AFND * p_afnd, AA *aa);
void liberar_AA(AA * aa);
Status AA_set_conjuntos(AA *aa, Conjunto *c);
int cmp_matriz(AA *aa);
Status copy_matriz(AA * aa);
int matriz_print(AA *aa);






#endif