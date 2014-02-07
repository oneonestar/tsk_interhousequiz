INC=-I../
all:
	@test -d obj || mkdir obj
	@echo Create obj directory
	gcc -c src/non_blocking_socket.c -o obj/non_blocking_socket.o -g $(INC)
	gcc -c src/score.c -o obj/score.o -std=c99 -g $(INC)
	gcc -c src/database_dblinker.c -o obj/db_connector.o -I./src -I../mysql_database_src `mysql_config --cflags` -std=c11
	gcc -c src/json_string.c -o obj/json_string.o -g $(INC)
	gcc -c src/quiz_server.c -std=c11 -o obj/quiz_server.o -g $(INC) `mysql_config --cflags`
	gcc -o quiz_server obj/*.o -std=c99 `mysql_config --libs --cflags` -ljson-c -levent

clean:
	rm *.o quiz_server
