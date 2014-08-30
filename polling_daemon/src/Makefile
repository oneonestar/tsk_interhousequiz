CC=gcc
OBJ=../obj/
BIN=../bin/

daemonmake: daemon.c stack.c

	$(CC) -o $(OBJ)/stack.o -c stack.c -g
	$(CC) -o $(OBJ)/daemon.o -c daemon.c -g
	$(CC) -o $(OBJ)/ajax.o -c ajax.c -g
	$(CC) -o $(BIN)/daemon $(OBJ)/daemon.o $(OBJ)/stack.o $(OBJ)/ajax.o -lcurl
	$(CC) -o $(BIN)/client client.c

clean:
	rm $(OBJ)/*.o
