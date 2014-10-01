CC=gcc
INC=-I../
BIN=bin
OBJ=obj

LIBS=-lncurses -lhiredis -levent -lsqlite3
LDFLAGS=-I./src
CFLAGS=-Wall -g

$(shell mkdir -p $(BIN) $(OBJ)) 

all: utilities.o db_redis.o non_blocking_socket.o score.o buzzer.o link_list.o quiz_server.o server_cb.o score_db.o
	$(CC) $(OBJ)/*.o -o $(BIN)/quiz_server $(CFLAGS) $(LDFLAGS) $(LIBS) $(CFLAGS)

utilities.o: src/utilities.c	
	$(CC) -c src/utilities.c -o $(OBJ)/utilities.o $(CFLAGS)

db_redis.o: src/db_redis.c
	$(CC) -c src/db_redis.c -o $(OBJ)/db_redis.o $(CFLAGS)

non_blocking_socket.o: src/non_blocking_socket.c
	$(CC) -c src/non_blocking_socket.c -o $(OBJ)/non_blocking_socket.o $(CFLAGS)

# pending for removal
score.o: src/score.c
	$(CC) -c src/score.c -o $(OBJ)/score.o -std=c99 $(CFLAGS)

buzzer.o: src/buzzer.c
	$(CC) -c src/buzzer.c -o $(OBJ)/buzzer.o $(CFLAGS)

link_list.o: src/link_list.c
	$(CC) -c src/link_list.c -o $(OBJ)/link_list.o $(CFLAGS)

server_cb.o: src/server_cb.c
	$(CC) -c src/server_cb.c -o $(OBJ)/server_cb.o $(CFLAGS)

quiz_server.o: src/quiz_server.c
	$(CC) -c src/quiz_server.c -o $(OBJ)/quiz_server.o $(CFLAGS)

score_db.o: src/score_db.c
	$(CC) -c src/score_db.c -o $(OBJ)/score_db.o $(CFLAGS)

clean:
	rm $(OBJ)/*.o $(BIN)/*
