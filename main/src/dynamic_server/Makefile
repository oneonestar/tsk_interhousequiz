INC=-I../
all:
	@test -d obj || mkdir obj
	@test -d bin || mkdir bin
	@echo Create obj directory
	@echo Create bin directory
	gcc -c src/db_redis.c -o obj/db_redis.o -g
	gcc -c src/non_blocking_socket.c -o obj/non_blocking_socket.o -g $(INC)
	gcc -c src/score.c -o obj/score.o -std=c99 -g $(INC)
	gcc -c src/buzzer.c -o obj/buzzer.o -g $(INC)
	gcc -c src/link_list.c -o obj/link_list.o -g $(INC)
	gcc -c src/quiz_server.c -o obj/quiz_server.o -g $(INC)
	gcc -o bin/quiz_server obj/*.o -std=c99 -I./src -levent_core -lhiredis -lncurses

clean:
	rm obj/*.o quiz_server
