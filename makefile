ver = release

ifeq ($(ver), debug)
FLAGS = -c -g
else
FLAGS = -c
endif

build: obj main.exe
obj:
	mkdir obj
main.exe: obj/main.o obj/libman.o obj/aes256.o
	gcc obj/main.o obj/libman.o obj/aes256.o -o main.exe
obj/aes256.o: aes256.c
	gcc -std=c89 $(FLAGS) aes256.c -o obj/aes256.o
obj/main.o: main.c
	gcc -std=c89 $(FLAGS) main.c -o obj/main.o
obj/libman.o: libman.c libman.h
	gcc -std=c89 $(FLAGS) libman.c -o obj/libman.o
clean:
	-del /q obj\*
	-rm -rf obj