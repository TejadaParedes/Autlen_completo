#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minimiza.h"

struct _AA{
    int **matriz_a;
    int **matriz_b;
    Conjunto **conjuntos;
    int num_conjuntos;
    int tam;
    int num_estados_accesibles;
};

Status creando_conjuntos(AFND * p_afnd, int *tabla, AA *aa, int flag, int tabla_tam);
Bool comprobar_clase(AFND *afnd, Conjunto **c, int a, int b, int nConjuntos);
Status pintar_matriz(AFND *afnd, AA *aa, int i, int j);
Status nuevas_clases(AA *aa, int *tabla, int tabla_tam, AFND *p_afnd);
void liberar_conjuntos(AA *aa, int tam);
Status ordenar_tabla(int * tabla, int tam_tabla);
char *nombre_conjunto(int *tabla, AFND * afnd, int tam);
Status obtener_transiciones1(AA *aa, AFND * afnd, AFND *new_afnd);
AFND *declarar1(AA *aa, AFND * afnd);

Status print_conjunto_cociente(AA *aa, AFND * afnd){

    int i = 0;
    printf("{ ");
    for(i = 0; i < aa->num_conjuntos; i++){
        conjunto_print_nombre(aa->conjuntos[i], afnd);
        if(i != aa->num_conjuntos -1) printf(", ");
    }
    printf(" }\n");
    return OK;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que reserva memoria e inicializa la estructura del Automata determinista.
 */

AA *AA_ini(int tam, int tam_s){

    int numEstados = 0, i = 0, j = 0;
    AA *aa = 0;

    aa = (AA*)calloc(1, sizeof(AA));

    aa->conjuntos = (Conjunto**)calloc(tam, sizeof(Conjunto*));

    aa->matriz_a = (int**)calloc(tam, sizeof(int*));

    for(i = 0; i < tam; i++){
        aa->matriz_a[i] = (int*)calloc(tam, sizeof(int));
    }

    aa->matriz_b = (int**)calloc(tam, sizeof(int*));

    for(i = 0; i < tam; i++){
        aa->matriz_b[i] = (int*)calloc(tam, sizeof(int));
    }

    for ( i = 1; i < tam; i++){
        for (j = 0; j < i; j++){
            aa->matriz_b[i][j] = -1;
        }
    }

    aa->tam = tam;

    return aa;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que libera la memoria del Autómata.
 */
void liberar_AA(AA * aa){

    int i = 0;

    if(aa == NULL){
        return;
    }

    for(i = 0; i < aa->num_conjuntos; i++){
        conjunto_liberar(aa->conjuntos[i]);
    }

    free(aa->conjuntos);

    for(i = 0; i < aa->tam; i++){
        free(aa->matriz_a[i]);
    }

    free(aa->matriz_a);

    for(i = 0; i < aa->tam; i++){
        free(aa->matriz_b[i]);
    }

    free(aa->matriz_b);

    free(aa);
    return;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que añade una clase o conjunto al array de conjuntos o clases del autómata.
 */
Status AA_set_conjuntos(AA *aa, Conjunto *c){

    aa->conjuntos[aa->num_conjuntos] = c;
    aa->num_conjuntos++;

    return OK;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que se encarga de comparar las dos matrices del Autómata.
 */
int cmp_matriz(AA *aa){

    int i = 0, j = 0; 

    for ( i = 1; i < aa->tam; i++){
        for (j = 0; j < i; j++){
            if(aa->matriz_a[i][j] != aa->matriz_b[i][j]) return 1;
        }
    }

    return 0;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que se encarga de pasar los valores de una matriz a otra matriz.
 */
Status copy_matriz(AA * aa){

    int i = 0, j = 0, aux =0; 

    for ( i = 1; i < aa->tam; i++){
        for (j = 0; j < i; j++){
            aux = aa->matriz_a[i][j];
            aa->matriz_b[i][j] = aux;
        }
    }

    return OK;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que imprime las matrices del Autómata.
 */
int matriz_print(AA *aa){

    int i = 0, j = 0;

    printf("\n");
    printf("Matriz A\n");

    for ( i = 1; i < aa->tam; i++){
        for (j = 0; j < i; j++){
            printf("%d ", aa->matriz_a[i][j]);
        }
        printf("\n");
    }

    printf("\n");
    printf("Matriz B\n");

    for ( i = 1; i < aa->tam; i++){
        for (j = 0; j < i; j++){
            printf("%d ", aa->matriz_b[i][j]);
        }
        printf("\n");
    }
    
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que elimina los conjuntos o clases 
 * que tienen más de un elemento del array de conjuntos o clases del autómata.
 */
void liberar_conjuntos(AA *aa, int tam){
    int i = 0, cont= 0, numC=0;
    Conjunto *c = NULL;
    Conjunto ** aux = NULL;



    for(i = 0; i < aa->num_conjuntos; i++){
        numC = conjunto_get_nEstados(aa->conjuntos[i]);
        if(numC == 1){
            aa->conjuntos[cont] = aa->conjuntos[i];
            cont++;
        }
        else{
            conjunto_liberar(aa->conjuntos[i]);
            aa->conjuntos[i] = NULL; 
        }
         
    }
    
    if(cont){
        aa->num_conjuntos = cont;
    }
    else{
        aa->num_conjuntos = 0;
    }

    return ;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que se encarga de ordenar una tabla en orden ascendente. 
 */
Status ordenar_tabla(int * tabla, int tam_tabla){

    int i = 0, j = 0, *tabla_aux = NULL, position = 0, swap = 0;

    for (i = 0 ; i < ( tam_tabla - 1 ) ; i++){
      position = i;
 
      for (j = i + 1 ; j < tam_tabla ; j++){
        if ( tabla[position] > tabla[j] )
            position = j;
      }

      if (position != i){
         swap = tabla[i];
         tabla[i] = tabla[position];
         tabla[position] = swap;
      }
   }

    return OK;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que crea los conjuntos iniciales, y pinta la matriz para los pares de estados indistinguibles.
 */
Status creando_conjuntos(AFND * p_afnd, int *tabla, AA *aa, int flag, int tabla_tam){

    int numEstados = 0, tipoEstado = 0, i = 0, finales = 0, *conjunto = NULL, j = 0, numC = 0, z = 0;
    Conjunto *c1 = NULL, *c2 = NULL; 
    Bool aux = FALSE;

    for ( i = 1; i < aa->tam; i++){
        for (j = 0; j < i; j++){
            aa->matriz_a[i][j] = 0;
        }
    }
    numEstados = AFNDNumEstados(p_afnd);

    if(!flag){
        c1 = conjunto_ini(p_afnd);
        c2 = conjunto_ini(p_afnd);
        for ( i = 0; i < tabla_tam; i++){
            tipoEstado = AFNDTipoEstadoEn(p_afnd, tabla[i]);
            if( tipoEstado == INICIAL || tipoEstado == NORMAL){
                conjunto_set_cEstados(c1, tabla[i]);
            }
            else if (tipoEstado == FINAL || tipoEstado == INICIAL_Y_FINAL){
                conjunto_set_cEstados(c2, tabla[i]);
            }   
        }
        AA_set_conjuntos(aa, c1);
        AA_set_conjuntos(aa, c2);
        /* printf("HOLAAAAAAAA\n");
        printf("Q%d = ", p);
        print_conjunto_cociente(aa, p_afnd);*/
        return OK;
    }
    else if(flag){
        for (i = 0; i < aa->num_conjuntos; i++){
            if((numC = conjunto_get_nEstados(aa->conjuntos[i])) > 1){
                conjunto = conjunto_get_cEstados(aa->conjuntos[i]);
                for(j = 0; j < numC-1; j++){
                    for(z = j+1; z < numC; z++){
                        aux = comprobar_clase(p_afnd, aa->conjuntos, conjunto[j], conjunto[z], aa->num_conjuntos);
                        if(aux == TRUE){
                            pintar_matriz(p_afnd, aa, conjunto[z], conjunto[j]);
                        }
                    }
                }
            }
        }
        return OK;
    }

    return ERROR;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que comprueba que un par de estados se encuentra en la misma clase o conjunto. 
 */
Bool comprobar_clase(AFND *afnd, Conjunto **c, int a, int b, int nConjuntos){

    int i = 0,  j = 0, k = 0, x = 0, num1 = 0, num2 = 0, *tabla = NULL, flag = 0;
    Bool flag1 = FALSE, flag2 = FALSE;

    for(i = 0; (i < AFNDNumSimbolos(afnd)); i++){
        for ( j = 0; j < AFNDNumEstados(afnd); j++){
            if(AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, a, i, j)) num1 = j;
            if(AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, b, i, j)) num2 = j;
        }

        for ( k = 0; k < nConjuntos; k++){
            flag1 = esta_en_conjunto(c[k], num1);
            flag2 = esta_en_conjunto(c[k], num2);
            if(flag1 == TRUE && flag2 == TRUE){ flag++; break;}
            if(flag1 == FALSE && flag2 == FALSE) continue;
            if(flag1 == FALSE || flag2 == FALSE) return FALSE;
        }

        if(flag == 2) return TRUE;        
    }

    return FALSE;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que pinta en la matriz aquellos pares de estados indistinguibles.
 */
Status pintar_matriz(AFND *afnd, AA *aa, int i, int j){

    int a = 0;

    for(a = 0; a < aa-> tam; a++){
        if(aa->matriz_a[i][a] == 1) return ERROR;
    }

    aa->matriz_a[i][j] = 1;

    return OK; 
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo utilizado para crear las nuevas clases de conjuntos.
 */
Conjunto* crear_nuevosConjuntos(AFND *afnd, AA *aa, int i, int flag){

    int a = 0;
    Conjunto *c = NULL; 

    for(a = 0; a < aa-> tam; a++){
        if(aa->matriz_a[a][i] == 1) {
            if(flag == 0){
                    
                c = conjunto_ini(afnd);
                flag = 1;
            }
            
            conjunto_set_cEstados(c, i);
            conjunto_set_cEstados(c, a);
        }
    }
    return c; 
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que declara las nuevas clases de conjuntos para la siguiente relación de equivalencia Q/E.
 */
Status nuevas_clases(AA *aa, int *tabla, int tabla_tam, AFND *p_afnd){

    int i = 0, j = 0, flag = 0, *conjunto = NULL, z = 0, numC = 0;
    Conjunto *c = NULL, *unico = NULL;

    liberar_conjuntos(aa, aa->tam);

    for (j = 0; j < aa->tam; j++){

        c = crear_nuevosConjuntos(p_afnd, aa, j, flag);
    
        if(c){
            AA_set_conjuntos(aa, c);
        }
    }

    flag = 0;

    for ( i = 0; i < tabla_tam; i++){
        for ( j = 0; j < aa->num_conjuntos; j++){
            if(esta_en_conjunto(aa->conjuntos[j], tabla[i]) == TRUE){ 
                flag = 1; 
                break;
            }
        }
        if(!flag){
            c = conjunto_ini(p_afnd);
            conjunto_set_cEstados(c, tabla[i]);
            AA_set_conjuntos(aa, c);
            flag = 0;
        }

        flag = 0;
    }
   
    return OK;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que devuelve una tabla con los índices de todos los estados accesibles del Autómata no determinista.
 */
int *estados_accesibles(AFND_INFO *info, AFND * p_afnd, AA *aa){

    int *tabla = NULL, *estados = NULL, *simbolos = NULL, indice = 0, numEstados = 0, numSimbolos = 0, i = 0, j = 0, k = 0, flag = 0, x = 0;

    numEstados = info_get_n_estados(info);

    numSimbolos = info_get_n_simbolos(info);

    estados = info_get_i_estados(info);

    simbolos = info_get_i_simbolos(info);

    tabla = (int*)calloc(numEstados, sizeof(int));

    for(i = 0; i < numEstados; i++){
        tabla[i] = -1;
    }

    tabla[indice] = AFNDIndiceEstadoInicial(p_afnd); 
    indice++;

    for(i = 0; i < indice && i < numEstados ; i++){
        for(x = 0; x < numEstados; x++){
            for(j = 0; j < numSimbolos; j++){
                if(AFNDTransicionIndicesEstadoiSimboloEstadof(p_afnd, tabla[i], simbolos[j], estados[x])){
                    k = 0;
                    while(k < indice){
                        if(tabla[k] == estados[x]){
                            flag = 1;
                        }
                        k++;
                    }
                    if(!flag){
                        tabla[indice] = estados[x];
                        indice++;
                    }
                }
            }
            flag = 0;
        }
    }

    aa->num_estados_accesibles = k;

    return tabla;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que se encarga de crear los diferentes estados y trasiciones del Autómata.
 */
AFND *declarar1(AA *aa, AFND * afnd){

    int i = 0, j = 0, *tabla = NULL, tam = 0, flag = NORMAL;
    char *nombre_final = NULL;
    AFND *new_afnd = NULL;

    new_afnd = AFNDNuevo("Min", aa->num_conjuntos, AFNDNumSimbolos(afnd));

    for (i = 0; i < AFNDNumSimbolos(afnd); i++){
        AFNDInsertaSimbolo(new_afnd, AFNDSimboloEn(afnd, i));
    }
    
    for(i = 0; i < aa->num_conjuntos; i++){
        tabla = conjunto_get_cEstados(aa->conjuntos[i]);
        if ((tam = conjunto_get_nEstados(aa->conjuntos[i])) > 1){
            nombre_final = nombre_conjunto(tabla, afnd, tam);
            for(j = 0; j < tam; j++){
                if(AFNDTipoEstadoEn(afnd, tabla[j]) == INICIAL){
                    flag = INICIAL;
                }
                else if (AFNDTipoEstadoEn(afnd, tabla[j]) == FINAL && flag != INICIAL_Y_FINAL){
                    flag = FINAL;
                }
                else if (AFNDTipoEstadoEn(afnd, tabla[j]) == INICIAL_Y_FINAL){
                    flag = INICIAL_Y_FINAL;
                }
            }
            AFNDInsertaEstado(new_afnd, nombre_final, flag);
        }
        else{
            AFNDInsertaEstado(new_afnd, AFNDNombreEstadoEn(afnd, tabla[0]), AFNDTipoEstadoEn(afnd, tabla[0]));
        }
        free(nombre_final);
        flag = NORMAL;
    }

    obtener_transiciones1(aa, afnd, new_afnd);

    return new_afnd;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que se encarga de obtener los nombres de los estados de un conjunto y concatenarlos.
 */
char *nombre_conjunto(int *tabla, AFND * afnd, int tam){

    int i = 0;
    char *nombre_final = NULL, *nombre = NULL;

    nombre_final = (char*)calloc(100, sizeof(char));

    for(i = 0; i < tam; i++){
        nombre = (char*)calloc(strlen(AFNDNombreEstadoEn(afnd, tabla[i]))+1, sizeof(char));
        strcpy(nombre, AFNDNombreEstadoEn(afnd, tabla[i]));
        strcat(nombre_final, nombre);
        free(nombre);
    }
    
    return nombre_final;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que obtiene las transiciones de cada uno de los conjuntos o clases
 * existentes en el array de conjuntos, para construir el autómata minimizado.
 */
Status obtener_transiciones1(AA *aa, AFND * afnd, AFND *new_afnd){
    
    int i = 0, j = 0, k = 0, *tabla1 = NULL, x = 0, tam1 = 0, tam2 = 0, *tabla2 = NULL;
    char * nombre_final1 = NULL, *nombre_final2 = NULL;

    for ( i = 0; i < aa->num_conjuntos; i++){
        tabla1 = conjunto_get_cEstados(aa->conjuntos[i]);
        for ( k = 0; k < AFNDNumEstados(afnd); k++){
            for ( j = 0; j < AFNDNumSimbolos(afnd); j++){
                if(AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, tabla1[0], j, k)){
                    for ( x = 0; x < aa->num_conjuntos; x++){
                        tabla2 = conjunto_get_cEstados(aa->conjuntos[x]);
                        if (esta_en_conjunto(aa->conjuntos[x], k) == TRUE){
                            if ((tam2 = conjunto_get_nEstados(aa->conjuntos[x])) > 1 && (tam1 = conjunto_get_nEstados(aa->conjuntos[i])) > 1){
                                nombre_final1 = nombre_conjunto(tabla1, afnd, tam1);
                                nombre_final2 = nombre_conjunto(tabla2, afnd, tam2);
                                AFNDInsertaTransicion(new_afnd, nombre_final1, AFNDSimboloEn(afnd, j), nombre_final2);
                                free(nombre_final1);
                                free(nombre_final2);
                            }
                            else if ((tam1 = conjunto_get_nEstados(aa->conjuntos[i])) > 1){
                                nombre_final1 = nombre_conjunto(tabla1, afnd, tam1);
                                AFNDInsertaTransicion(new_afnd, nombre_final1, AFNDSimboloEn(afnd, j), AFNDNombreEstadoEn(afnd, tabla2[0]));
                                free(nombre_final1);
                            }
                            else if ((tam2 = conjunto_get_nEstados(aa->conjuntos[x])) > 1){
                                nombre_final2 = nombre_conjunto(tabla2, afnd, tam2);
                                AFNDInsertaTransicion(new_afnd, AFNDNombreEstadoEn(afnd, tabla1[0]), AFNDSimboloEn(afnd, j), nombre_final2);
                                free(nombre_final2);
                            }
                            else{
                                AFNDInsertaTransicion(new_afnd, AFNDNombreEstadoEn(afnd, tabla1[0]), AFNDSimboloEn(afnd, j), AFNDNombreEstadoEn(afnd, tabla2[0]));
                            }
                        }
                    }
                }
            }
        }
    }
    
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que se encarga de llamar a las funciones anteriores,
 * para minimizar el autómata no determinista. 
 */
AFND * AFNDMinimiza(AFND * afnd){

    int * e = NULL, i = 0, j = 0, *tabla = NULL, *tablaB = NULL, tam_tabla = 0, flag = 0, flagm = 0, p = 0;
    AFND_INFO *info = NULL;
    int numEstados = 0;
    AA *aa = NULL;
    AFND * new_afnd = NULL;

    info = afdn_info_ini();

    info = get_AFDN_INFO(afnd, info);

    numEstados = info_get_n_estados(info);

    aa = AA_ini(numEstados, info_get_n_simbolos(info));

    e = estados_accesibles(info, afnd, aa);

    tam_tabla = aa->num_estados_accesibles;

    tabla = (int*)calloc(tam_tabla, sizeof(int));

    for(j = 0; j < tam_tabla; j++){
        tabla[j] = e[j];
    }

    ordenar_tabla(tabla, tam_tabla);

    while(!flagm){

        if(!flag){
            creando_conjuntos(afnd, tabla, aa, 0, tam_tabla);
            flag = 1;
            printf("\n");
            printf("Q%d = ", p);
            print_conjunto_cociente(aa, afnd);
        }
        else{
            creando_conjuntos(afnd, tabla, aa, 1, tam_tabla);
            nuevas_clases(aa, tabla, tam_tabla, afnd);
            if(!cmp_matriz(aa)){
                flagm = 1;
            }else{
                copy_matriz(aa);
            }
            printf("Q%d = ", p);
            print_conjunto_cociente(aa, afnd);  
        }
        p++;    
    }

    new_afnd = declarar1(aa, afnd);

    afdn_info_destroy(info);
    free(e);
    free(tabla);
    liberar_AA(aa);
    return new_afnd;
}
