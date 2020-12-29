transporte: main.o listaord.o transporte.o
	gcc -o transporte main.o listaord.o transporte.o

main.o: main.c listaord.h transporte.h
	gcc -c main.c
	
listaord.o: listaord.c listaord.h
	gcc -c listaord.c

transporte.o: transporte.c listaord.h
	gcc -c transporte.c		
	
