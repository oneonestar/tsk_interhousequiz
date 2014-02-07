INC=-I../
all:
	@test -d obj || mkdir obj
	@echo Create obj directory
	gcc -c src/score.c -o obj/score.o -std=c99 -g $(INC)
	gcc -c src/database_dblinker.c -o obj/db_connector.o -I../mysql_database_src `mysql_config --cflags`
	gcc -c src/mysocket.c -o obj/mysocket.o -std=c99 -g $(INC) -Wall
	gcc -c src/json_string.c -o obj/json_string.o -g $(INC)
	gcc -c src/quiz_server.c -o obj/quiz_server.o -g $(INC) `mysql_config --cflags`
	gcc -o quiz_server obj/*.o -std=c99 `mysql_config --libs --cflags` -ljson-c

clean:
	rm *.o quiz_server
