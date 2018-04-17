ver = release

ifeq ($(ver), debug)
FLAGS = -c -g
else
FLAGS = -c
endif

build: obj main.exe
obj:
	mkdir obj
main.exe: obj/main.o obj/libman.o
	gcc obj/main.o obj/libman.o -o main.exe
obj/main.o: main.c
	gcc -std=c89 $(FLAGS) main.c -o obj/main.o
obj/libman.o: libman.c libman.h
	gcc -std=c89 $(FLAGS) libman.c -o obj/libman.o
clean:
	-del /q obj\*
	-rm -rf obj