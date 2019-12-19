CC=gcc
FLAGS = -Wall -ansi
OBJECTSEx = examen.o afnd.o minimiza.o conjunto.o afnd_info.o transforma.o new_estado.o

#Ejecutable
all: examen

examen: $(OBJECTSEx)
	$(CC) -g -Wall -ansi -pedantic -o examen $(OBJECTSEx)


#Objetos

examen.o: examen.c afnd.h minimiza.h transforma.h
	$(CC)  -g -c examen.c
afnd.o: afnd.c afnd.h
	$(CC) -g -c afnd.c
minimiza.o: minimiza.c minimiza.h afnd.h afnd_info.h types.h
	$(CC)  -g -c minimiza.c
transforma.o: transforma.c transforma.h afnd.h types.h
	$(CC)  -g -c transforma.c
conjunto.o: conjunto.c conjunto.h types.h afnd_info.h afnd.h
	$(CC) -g -c conjunto.c
new_estado.o: new_estado.c new_estado.h types.h afnd_info.h afnd.h
	$(CC) -g -c new_estado.c
afnd_info.o: afnd_info.c afnd_info.h afnd.h types.h
	$(CC) -g -c afnd_info.c
#Limpieza
clean:
	rm -rf *.o *.dot examen

valgrindexamen:
	valgrind -v --leak-check=full ./examen

run_examen:
	./examen
