# default:
# 	g++ ../src/x.cpp -o w -O2 -Wall -Wno-missing-braces -I ../include/ -L ../lib/ -lraylib -lopengl32 -lgdi32 -lwinmm

EXE=..\ganhou
src=..\src
inc=..\include

all:	${EXE}

${EXE}:	Aposta.o Db.o Interface.o main.o
		g++ Aposta.o Db.o Interface.o main.o -o ${EXE} -O2 -I ..\include\ -L ..\lib\ -lraylib -lopengl32 -lgdi32 -lwinmm

Aposta.o:	${inc}\Aposta.hpp ${src}\Aposta.cpp
		g++ -c ${src}\Aposta.cpp

Interface.o:	${inc}\Interface.hpp ${src}\Interface.cpp
		g++ -c ${src}\Interface.cpp

Db.o:		${inc}\Db.hpp ${src}\Db.cpp
		g++ -c ${src}\Db.cpp

main.o:		${inc}\Aposta.hpp ${inc}\Db.hpp ${inc}\Interface.hpp ${src}\main.cpp
		g++ -c ${src}\main.cpp -Wall