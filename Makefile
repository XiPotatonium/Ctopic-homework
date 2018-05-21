# Project: graphics
# Makefile created by Dev-C++ 5.5.2

CPP      = g++.exe -D__DEBUG__
CC       = gcc.exe -D__DEBUG__
WINDRES  = windres.exe
OBJ      = obj/exceptio.o obj/genlib.o obj/graphics.o obj/random.o obj/simpio.o obj/strlib.o obj/main.o obj/linkedlist.o
LINKOBJ  = obj/exceptio.o obj/genlib.o obj/graphics.o obj/random.o obj/simpio.o obj/strlib.o obj/main.o obj/linkedlist.o
#LIBS     = -L"D:\mingw-w64\x86_64-7.3.0-posix-seh-rt_v5-rev0\mingw64\lib" -L"D:\mingw-w64\x86_64-7.3.0-posix-seh-rt_v5-rev0\mingw64\x86_64-w64-mingw32\lib" -static-libstdc++ -static-libgcc -mwindows -g3 -lgdi32
LIBS     = -static-libstdc++ -static-libgcc -mwindows -g3 -lgdi32
#INCS     = -I"D:\mingw-w64\x86_64-7.3.0-posix-seh-rt_v5-rev0\mingw64\include" -I"./include"
#CXXINCS  = -I"D:\mingw-w64\x86_64-7.3.0-posix-seh-rt_v5-rev0\mingw64\include" -I"D:\mingw-w64\x86_64-7.3.0-posix-seh-rt_v5-rev0\mingw64\x86_64-w64-mingw32\include" -I"./include"
BIN      = bin/main.exe
CXXFLAGS = $(CXXINCS) -Wfatal-errors -g3
CFLAGS   = $(INCS) -Wfatal-errors -g3
RM       = del /q

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

clean:
	$(RM) obj\* bin\*

$(BIN): $(OBJ)
	$(CC) $(LINKOBJ) -o $(BIN) $(LIBS)

obj/exceptio.o: libgraphics/exceptio.c
	$(CC) -c libgraphics/exceptio.c -o obj/exceptio.o $(CFLAGS)

obj/genlib.o: libgraphics/genlib.c
	$(CC) -c libgraphics/genlib.c -o obj/genlib.o $(CFLAGS)

obj/graphics.o: libgraphics/graphics.c
	$(CC) -c libgraphics/graphics.c -o obj/graphics.o $(CFLAGS)

obj/random.o: libgraphics/random.c
	$(CC) -c libgraphics/random.c -o obj/random.o $(CFLAGS)

obj/simpio.o: libgraphics/simpio.c
	$(CC) -c libgraphics/simpio.c -o obj/simpio.o $(CFLAGS)

obj/strlib.o: libgraphics/strlib.c
	$(CC) -c libgraphics/strlib.c -o obj/strlib.o $(CFLAGS)

obj/main.o: src/main.c
	$(CC) -c src/main.c -o obj/main.o $(CFLAGS)

obj/linkedlist.o: src/linkedlist.c
	$(CC) -c src/linkedlist.c -o obj/linkedlist.o $(CFLAGS)
