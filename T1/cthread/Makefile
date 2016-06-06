#
# Makefile ESQUELETO
#
# OBRIGATÓRIO ter uma regra "all" para geração da biblioteca e de uma
# regra "clean" para remover todos os objetos gerados.
#
# NECESSARIO adaptar este esqueleto de makefile para suas necessidades.
#  1. Cuidado com a regra "clean" para não apagar o "fila2.o"
#
# OBSERVAR que as variáveis de ambiente consideram que o Makefile está no diretótio "cthread"
#

CC=gcc
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src

all: regra1

regra1: #dependências para a regra1
	$(CC) -c $(SRC_DIR)/cthread.c -o $(BIN_DIR)/cthread.o -Wall
	ar crs $(LIB_DIR)/libcthread.a $(BIN_DIR)/cthread.o
	$(CC) $(SRC_DIR)/main.c $(BIN_DIR)/fila2.o -L $(LIB_DIR)/ -lcthread -o $(BIN_DIR)/main

#regra2: #dependências para a regra2
#	$(CC) -o $(BIN_DIR)regra2 $(SRC_DIR)regra2.c -Wall

#regran: #dependências para a regran
#	$(CC) -o $(BIN_DIR)regran $(SRC_DIR)regran.c -Wall

#clean:
#	rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/*.o $(SRC_DIR)/*~ $(INC_DIR)/*~ *~
