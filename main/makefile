INC=-I../
all:
	@test -d obj || mkdir obj
	@echo Create obj directory
	gcc -c src/non_blocking_socket.c -o obj/non_blocking_socket.o -g $(INC)
	gcc -c src/score.c -o obj/score.o -std=c99 -g $(INC)
	gcc -c src/db_mysql.c -o obj/db_connector.o -I./src `mysql_config --cflags` 
	gcc -c src/quiz_server.c -o obj/quiz_server.o -g $(INC) `mysql_config --cflags`
	gcc -c src/buzzer.c -o obj/buzzer.o -g $(INC)
	gcc -o quiz_server obj/*.o -std=c99 -I./src `mysql_config --libs` -levent_core

clean:
	rm *.o quiz_server
