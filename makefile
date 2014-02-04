all:
	gcc -c ipc_scoring/score.c -std=c99 -g
	gcc -c c_nonblock_server/mysocket.c -std=c99 -g
	gcc -c quiz_server.c -std=c99 -g
	gcc -o quiz_server score.o quiz_server.o mysocket.o

clean:
	rm *.o quiz_server
