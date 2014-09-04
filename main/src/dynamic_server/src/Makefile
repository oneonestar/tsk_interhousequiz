CC=gcc
BIN=../bin
OBJ=../obj

all: 
	$(CC) -o $(OBJ)/link_list.o -c link_list.c -g
	$(CC) -o $(OBJ)/server.o -c server.c -g
	$(CC) -o $(BIN)/server $(OBJ)/server.o $(OBJ)/link_list.o -levent -lncurses -g
clean:
	rm $(OBJ)/*.o
