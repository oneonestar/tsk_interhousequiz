CC=gcc
INC=-I../
BIN=bin
OBJ=obj

LIBS=-lncurses -lhiredis -levent
LDFLAGS=-I./src
CFLAGS=-Wall

$(shell mkdir -p $(BIN) $(OBJ)) 

all: utilities.o db_redis.o non_blocking_socket.o score.o buzzer.o link_list.o quiz_server.o server_cb.o
	$(CC) $(OBJ)/*.o -o $(BIN)/quiz_server $(CFLAGS) $(LDFLAGS) $(LIBS) -Wall

utilities.o: src/utilities.c	
	$(CC) -c src/utilities.c -o $(OBJ)/utilities.o -Wall -g

db_redis.o: src/db_redis.c
	$(CC) -c src/db_redis.c -o $(OBJ)/db_redis.o -Wall -g

non_blocking_socket.o: src/non_blocking_socket.c
	$(CC) -c src/non_blocking_socket.c -o $(OBJ)/non_blocking_socket.o -Wall -g

score.o: src/score.c
	$(CC) -c src/score.c -o $(OBJ)/score.o -std=c99 -Wall -g

buzzer.o: src/buzzer.c
	$(CC) -c src/buzzer.c -o $(OBJ)/buzzer.o -Wall -g

link_list.o: src/link_list.c
	$(CC) -c src/link_list.c -o $(OBJ)/link_list.o -Wall -g

server_cb.o: src/server_cb.o
	$(CC) -c src/server_cb.c -o $(OBJ)/server_cb.o -DHASH_DEBUG=1 -Wall -g

quiz_server.o: src/quiz_server.o
	$(CC) -c src/quiz_server.c -o $(OBJ)/quiz_server.o -Wall -g

clean:
	rm $(OBJ)/*.o $(BIN)/*
