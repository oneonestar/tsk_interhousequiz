CC=gcc
INC=-I../
BIN=bin
OBJ=obj

LDFLAGS=-lncurses -levent -lsqlite3
CFLAGS=-Wall -g

# for linking against glib
GLIB_CFLAGS=`pkg-config --cflags glib-2.0`
GLIB_LDFLAGS=`pkg-config --libs glib-2.0`
JSON_CFLAGS=`pkg-config --cflags json-glib-1.0`
JSON_LDFLAGS=`pkg-config --libs json-glib-1.0`

$(shell mkdir -p $(BIN) $(OBJ)) 

all: utilities.o non_blocking_socket.o buzzer.o link_list.o quiz_server.o json_msg.o server_cb.o score_db.o layout.o question_db.o
	$(CC) $(OBJ)/*.o -o $(BIN)/quiz_server $(CFLAGS) $(LDFLAGS) $(CFLAGS) $(GLIB_CFLAGS) $(GLIB_LDFLAGS) $(JSON_CFLAGS) $(JSON_LDFLAGS)

utilities.o: src/utilities.c	
	$(CC) -c src/utilities.c -o $(OBJ)/utilities.o $(CFLAGS)

question_db.o: src/question_db.c
	$(CC) -c src/question_db.c -o $(OBJ)/question_db.o $(CFLAGS) -lsqite3

non_blocking_socket.o: src/non_blocking_socket.c
	$(CC) -c src/non_blocking_socket.c -o $(OBJ)/non_blocking_socket.o $(CFLAGS)

buzzer.o: src/buzzer.c
	$(CC) -c src/buzzer.c -o $(OBJ)/buzzer.o $(CFLAGS)

link_list.o: src/link_list.c
	$(CC) -c src/link_list.c -o $(OBJ)/link_list.o $(CFLAGS)

json_msg.o: src/json_msg.c
	$(CC) -c src/json_msg.c -o $(OBJ)/json_msg.o $(JSON_CFLAGS) $(JSON_LDFLAGS)

server_cb.o: src/server_cb.c
	$(CC) -c src/server_cb.c -o $(OBJ)/server_cb.o $(CFLAGS) $(GLIB_CFLAGS) $(GLIB_LDFLAGS) $(JSON_CFLAGS) $(JSON_LDFLAGS)

quiz_server.o: src/quiz_server.c
	$(CC) -c src/quiz_server.c -o $(OBJ)/quiz_server.o $(CFLAGS)

score_db.o: src/score_db.c
	$(CC) -c src/score_db.c -o $(OBJ)/score_db.o $(CFLAGS)

layout.o: src/layout.c
	$(CC) -c src/layout.c -o $(OBJ)/layout.o $(CFLAGS) -lncurses

clean:
	rm $(OBJ)/*.o $(BIN)/*
