INC=-I../
all:
	@test -d obj || mkdir obj
	@echo Create obj directory
	gcc -c src/score.c -o obj/score.o -std=c99 -g $(INC)
	gcc -c src/db_connector.c -o obj/db_connector.o -std=c99 -g $(INC)
	gcc -c src/mysocket.c -o obj/mysocket.o -std=c99 -g $(INC)
	gcc -c src/quiz_server.c -o obj/quiz_server.o -std=c99 -g $(INC)
	gcc -o quiz_server -ljson-c -lmongoc obj/*.o

clean:
	rm *.o quiz_server
