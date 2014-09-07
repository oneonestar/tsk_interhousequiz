all:
	gcc -o link_list.o -c link_list.c -g
	gcc -o server.o -c server.c -g
	gcc -o server server.o link_list.c -levent -lncurses
