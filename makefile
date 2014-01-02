all:
	gcc -c ipc_scoring/score.c -std=c99
	gcc -c quiz_server.c -std=c99
	gcc -o quiz_server score.o quiz_server.o

clean:
	rm *.o quiz_server
