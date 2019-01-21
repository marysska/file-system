all: main


main: main.c fsystem.c fsystem.h
	gcc -o main main.c fsystem.c fsystem.h -I .


