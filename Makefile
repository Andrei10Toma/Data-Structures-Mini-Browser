all: build run clear

build: stiva.c main.c lista_generica.c coada.c utils.c functii.c
	gcc stiva.c main.c lista_generica.c coada.c utils.c functii.c -o tema2 -Wall -Wextra -g

run:
	./tema2 input2 output

valgrind:
	valgrind --leak-check=full ./tema2 input2 output

clear:
	rm -rf tema2
