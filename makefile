ver = release

ifeq ($(ver), debug)
FLAGS = -c -g
else
FLAGS = -c
endif

libman.exe: main.o libman.o
	gcc obj/main.o obj/libman.o -o main.exe
main.o: main.c
	gcc -std=c89 $(FLAGS) main.c -o obj/main.o
libman.o: libman.c libman.h
	gcc -std=c89 $(FLAGS) libman.c -o obj/libman.o
clean:
	del /q obj\*