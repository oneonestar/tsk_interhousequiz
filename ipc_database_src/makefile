all:
	gcc -c json_string.c
	gcc -c -g `mysql_config --cflags` database_dblinker.c 
	gcc -o database_dblinker database_dblinker.o `mysql_config --libs` json_string.o -ljson-c

clean:
	rm *.o database_dblinker
