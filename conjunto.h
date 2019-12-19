#ifndef CONJUNTO_H
#define CONJUNTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "afnd_info.h"
#include "afnd.h"
#include "types.h"

typedef struct _Conjunto Conjunto;


Conjunto * conjunto_ini(AFND * p_afnd);
void conjunto_liberar(Conjunto *c);
int  conjunto_set_cEstados(Conjunto * c, int estados);
Conjunto * conjunto_set_nEstados(Conjunto * c, int numEstados);
int conjunto_get_nEstados(Conjunto * c);
int * conjunto_get_cEstados(Conjunto * c);
Bool esta_en_conjunto(Conjunto *c, int indice);
int conjunto_print(Conjunto *c);
int conjunto_print_nombre(Conjunto *c, AFND * afnd);

#endif