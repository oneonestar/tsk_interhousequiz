all:
	gcc -c ipc_scoring/score.c -std=c99 -g
	gcc -c -g `mysql_config --cflags` ipc_database_src/database_dblinker.c
	gcc -c c_nonblock_server/mysocket.c -std=c99 -g
	gcc -c `mysql_config --cflags` quiz_server.c -std=c99 -g
	gcc -o quiz_server `mysql_config --libs` database_dblinker.o score.o quiz_server.o mysocket.o

clean:
	rm */*.o quiz_server
