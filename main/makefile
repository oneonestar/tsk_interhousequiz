INC=-I../
all:
	@test -d obj || mkdir obj
	@echo Create obj directory
	gcc -c src/score.c -o obj/score.o -std=c99 -g $(INC)
	#gcc -c src/db_connector.c -o obj/db_connector.o -std=c99 -g $(INC)
	gcc -c src/database_dblinker.c -o obj/db_connector.o `mysql_config --cflags`
	gcc -c src/mysocket.c -o obj/mysocket.o -std=c99 -g $(INC)
	gcc -c src/quiz_server.c -o obj/quiz_server.o -g $(INC) `mysql_config --cflags`
	#gcc -o quiz_server -ljson-c -lmongoc obj/*.o
	gcc -o quiz_server -ljson-c obj/*.o -std=c99 `mysql_config --libs --cflags`

clean:
	rm *.o quiz_server
