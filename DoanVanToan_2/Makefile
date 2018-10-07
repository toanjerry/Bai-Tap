# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -Wall

# the list of header files
#INCL   = server.h
#the list of source files
SRC = sv_2.c cl_2.c
OBJ = $(SRC:.c=.o)

# the build target executable:
#TARGET = sv_2
build:
	$(CC) sv_2.c -o sv_2
	$(CC) cl_2.c -o cl_2
