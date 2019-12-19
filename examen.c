#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "afnd.h"
#include "minimiza.h"
#include "transforma.h"

int main(int argc, char ** argv)
{

	AFND * p_afnd;
	AFND * trans;
    AFND * min;

	p_afnd= AFNDNuevo("af1", 4, 2);

	AFNDInsertaSimbolo(p_afnd,"a");
	AFNDInsertaSimbolo(p_afnd, "b");

	AFNDInsertaEstado(p_afnd, "P",INICIAL);
	AFNDInsertaEstado(p_afnd, "Q", NORMAL);
	AFNDInsertaEstado(p_afnd, "R", FINAL);
	AFNDInsertaEstado(p_afnd, "S", FINAL);

	AFNDInsertaTransicion(p_afnd, "P", "b", "P");
	AFNDInsertaTransicion(p_afnd, "P", "b", "R");

	AFNDInsertaTransicion(p_afnd, "Q", "a", "P");
	AFNDInsertaTransicion(p_afnd, "Q", "b", "P");
	AFNDInsertaTransicion(p_afnd, "Q", "a", "R");

	AFNDInsertaTransicion(p_afnd, "R", "b", "P");
	AFNDInsertaTransicion(p_afnd, "R", "a", "R");

	AFNDInsertaTransicion(p_afnd, "S", "a", "S");
	AFNDInsertaTransicion(p_afnd, "S", "b", "S");

    AFNDImprime(stdout,p_afnd);
	AFNDADot(p_afnd);


    trans = AFNDTransforma(p_afnd);
    AFNDImprime(stdout,trans);
	AFNDADot(trans);

    min = AFNDMinimiza(trans);
	AFNDImprime(stdout,min);
	AFNDADot(min);

	AFNDElimina(min);
    AFNDElimina(trans);
	AFNDElimina(p_afnd);

	return 0;
}